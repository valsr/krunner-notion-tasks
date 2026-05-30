#include "notionrunner.h"

#include <KRunner/QueryMatch>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KNotification>

#include <QDate>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutexLocker>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>

K_PLUGIN_CLASS_WITH_JSON(NotionRunner, "notionrunner.json")


static constexpr int PROJECT_TTL = 1800;

static const QString CFG_FILE = QStringLiteral("krunner-notion-tasks.conf");
static const QString CFG_NOTION = QStringLiteral("notion");
static const QString CFG_RUNNER = QStringLiteral("runner");
static const QString NOTION_VER = QStringLiteral("2025-09-03");
static const QString NOTIFY_COMP = QStringLiteral("krunner-notion-tasks");

static const QString DEF_DB_ID = QStringLiteral("");
static const QString DEF_PROJECTS_ID = QStringLiteral("");
static const QString DEF_PROJECT = QStringLiteral("Todos");
static const QString DEF_TEMPLATE = QStringLiteral("");
static const QString DEF_KEYWORD = QStringLiteral("task,todo,add task,add todo");

NotionRunner::NotionRunner(QObject *parent, const KPluginMetaData &data)
    : KRunner::AbstractRunner(parent, data)
{
}

void NotionRunner::reloadConfiguration()
{
    QMutexLocker lock(&m_cacheMutex);
    m_cache.clear();
}

NotionRunner::Config NotionRunner::loadConfig() const
{
    const auto cfg = KSharedConfig::openConfig(CFG_FILE);
    const auto notion = cfg->group(CFG_NOTION);
    const auto runner = cfg->group(CFG_RUNNER);
    const QString kwRaw = runner.readEntry("keyword", DEF_KEYWORD);
    QStringList keywords;
    for (const QString &k : kwRaw.split(QLatin1Char(','), Qt::SkipEmptyParts))
        keywords << k.trimmed();
    if (keywords.isEmpty())
        keywords << QStringLiteral("todo");

    return {
        .token = notion.readEntry("token", QString()),
        .databaseId = notion.readEntry("database_id", DEF_DB_ID),
        .projectsDbId = notion.readEntry("projects_database_id", DEF_PROJECTS_ID),
        .keywords = keywords,
        .defaultProject = runner.readEntry("default_project", DEF_PROJECT),
        .templateId = runner.readEntry("template_id", DEF_TEMPLATE),
    };
}

NotionRunner::ParsedQuery NotionRunner::parseQuery(const QString &text) const
{
    static const QHash<QString, QString> priorityMap = {
        {QStringLiteral("high"), QStringLiteral("High")},
        {QStringLiteral("h"), QStringLiteral("High")},
        {QStringLiteral("medium"), QStringLiteral("Medium")},
        {QStringLiteral("med"), QStringLiteral("Medium")},
        {QStringLiteral("m"), QStringLiteral("Medium")},
        {QStringLiteral("low"), QStringLiteral("Low")},
        {QStringLiteral("l"), QStringLiteral("Low")},
    };

    ParsedQuery pq;
    QString working = text;

    static const QRegularExpression rePriority(QStringLiteral("!([A-Za-z]+)"));
    const auto pm = rePriority.match(working);
    if (pm.hasMatch())
    {
        pq.priority = priorityMap.value(pm.captured(1).toLower());
        working.remove(pm.capturedStart(), pm.capturedLength());
    }

    static const QRegularExpression reDue(QStringLiteral("@(\\S+)"));
    const auto dm = reDue.match(working);
    if (dm.hasMatch())
    {
        const QString d = dm.captured(1).toLower();
        const QDate today = QDate::currentDate();
        if (d == QStringLiteral("today"))
        {
            pq.dueDate = today.toString(Qt::ISODate);
        }
        else if (d == QStringLiteral("tomorrow") || d == QStringLiteral("tmr"))
        {
            pq.dueDate = today.addDays(1).toString(Qt::ISODate);
        }
        else
        {
            const QDate parsed = QDate::fromString(d, Qt::ISODate);
            if (parsed.isValid())
                pq.dueDate = d;
        }
        working.remove(dm.capturedStart(), dm.capturedLength());
    }

    pq.taskName = working.simplified();
    return pq;
}

QString NotionRunner::toUuid(const QString &id) const
{
    QString s = id;
    s.remove(QLatin1Char('-'));
    if (s.length() != 32)
        return id;
    return QStringLiteral("%1-%2-%3-%4-%5")
        .arg(s.mid(0, 8), s.mid(8, 4), s.mid(12, 4), s.mid(16, 4), s.mid(20));
}

ApiResult<QByteArray> NotionRunner::notionRequest(const QString &method, const QString &path,
                                                  const QByteArray &body, const QString &token) const
{
    QNetworkAccessManager nam;
    nam.setTransferTimeout(10000);

    const QString trimPath = path.startsWith(QLatin1Char('/')) ? path.mid(1) : path;
    QNetworkRequest req(QUrl(QStringLiteral("https://api.notion.com/v1/") + trimPath));
    req.setRawHeader("Authorization", QByteArrayLiteral("Bearer ") + token.toUtf8());
    req.setRawHeader("Notion-Version", NOTION_VER.toUtf8());
    req.setRawHeader("Content-Type", "application/json");

    QNetworkReply *reply = (method == QStringLiteral("GET"))
                               ? nam.get(req)
                               : nam.sendCustomRequest(req, method.toUtf8(), body);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    const QByteArray result = reply->readAll();
    if (reply->error() != QNetworkReply::NoError)
    {
        const QJsonDocument errDoc = QJsonDocument::fromJson(result);
        const QString msg = errDoc.object().value(QStringLiteral("message")).toString();
        const QString fallback = reply->errorString();
        reply->deleteLater();
        const QString err = msg.isEmpty() ? fallback : msg;
        return ApiResult<QByteArray>::err(err);
    }
    reply->deleteLater();
    return ApiResult<QByteArray>::ok(result);
}

ApiResult<QJsonArray> NotionRunner::dbQuery(const QString &dbId, const QJsonObject &filter,
                                            const QString &token) const
{
    const QByteArray body = QJsonDocument(QJsonObject{{QStringLiteral("filter"), filter}})
                                .toJson(QJsonDocument::Compact);
    const auto res = notionRequest(
        QStringLiteral("POST"),
        QStringLiteral("databases/") + toUuid(dbId) + QStringLiteral("/query"),
        body, token);
    if (res.isError())
        return ApiResult<QJsonArray>::err(res.message);
    return ApiResult<QJsonArray>::ok(
        QJsonDocument::fromJson(res.value).object().value(QStringLiteral("results")).toArray());
}

template <typename Fn>
QVariant NotionRunner::cached(const QString &key, int ttlSecs, Fn &&fn)
{
    {
        QMutexLocker lock(&m_cacheMutex);
        const auto it = m_cache.constFind(key);
        if (it != m_cache.constEnd() && it->expiry > QDateTime::currentDateTime())
            return it->value;
    }
    const QVariant value = fn();
    QMutexLocker lock(&m_cacheMutex);
    m_cache[key] = {value, QDateTime::currentDateTime().addSecs(ttlSecs)};
    return value;
}

QString NotionRunner::resolveProjectId(const Config &cfg, const QString &name)
{
    return cached(
               QStringLiteral("project:") + cfg.projectsDbId + QLatin1Char(':') + name,
               PROJECT_TTL,
               [&]() -> QVariant
               {
                   const QJsonObject filter{
                       {QStringLiteral("property"), QStringLiteral("Project name")},
                       {QStringLiteral("title"),
                        QJsonObject{{QStringLiteral("equals"), name}}},
                   };
                   const auto res = dbQuery(cfg.projectsDbId, filter, cfg.token);
                   if (res.isOk() && !res.value.isEmpty())
                       return res.value[0].toObject().value(QStringLiteral("id")).toString();
                   return QString();
               })
        .toString();
}

ApiResult<bool> NotionRunner::addNotionTask(const Config &cfg, const ParsedQuery &pq)
{
    QJsonObject properties;
    properties[QStringLiteral("Task name")] = QJsonObject{
        {QStringLiteral("title"), QJsonArray{QJsonObject{
                                      {QStringLiteral("text"),
                                       QJsonObject{{QStringLiteral("content"), pq.taskName}}}}}}};
    properties[QStringLiteral("Status")] = QJsonObject{
        {QStringLiteral("status"),
         QJsonObject{{QStringLiteral("name"), QStringLiteral("Not Started")}}}};

    if (!pq.priority.isEmpty())
        properties[QStringLiteral("Priority")] = QJsonObject{
            {QStringLiteral("select"),
             QJsonObject{{QStringLiteral("name"), pq.priority}}}};

    if (!pq.dueDate.isEmpty())
        properties[QStringLiteral("Due")] = QJsonObject{
            {QStringLiteral("date"),
             QJsonObject{{QStringLiteral("start"), pq.dueDate}}}};

    if (!cfg.defaultProject.isEmpty() && !cfg.projectsDbId.isEmpty())
    {
        const QString id = resolveProjectId(cfg, cfg.defaultProject);
        if (!id.isEmpty())
            properties[QStringLiteral("Project")] = QJsonObject{
                {QStringLiteral("relation"),
                 QJsonArray{QJsonObject{{QStringLiteral("id"), id}}}}};
    }

    QJsonObject payload{
        {QStringLiteral("parent"),
         QJsonObject{{QStringLiteral("type"), QStringLiteral("database_id")},
                     {QStringLiteral("database_id"), toUuid(cfg.databaseId)}}},
        {QStringLiteral("properties"), properties},
    };

    if (!cfg.templateId.isEmpty())
        payload[QStringLiteral("template")] = QJsonObject{
            {QStringLiteral("type"), QStringLiteral("template_id")},
            {QStringLiteral("template_id"), toUuid(cfg.templateId)}};

    const auto res = notionRequest(QStringLiteral("POST"), QStringLiteral("pages"),
                                   QJsonDocument(payload).toJson(QJsonDocument::Compact), cfg.token);
    if (res.isError())
        return ApiResult<bool>::err(res.message);
    return ApiResult<bool>::ok(true);
}

void NotionRunner::match(KRunner::RunnerContext &context)
{
    const Config cfg = loadConfig();
    const QString query = context.query();

    QString taskText;
    bool matched = false;
    for (const QString &kw : cfg.keywords)
    {
        if (query == kw || query.startsWith(kw + QLatin1Char(' ')))
        {
            taskText = query.mid(kw.length()).trimmed();
            matched = true;
            break;
        }
    }
    if (!matched)
    {
        return;
    }

    if (taskText == QStringLiteral(":config") ||
        taskText == QStringLiteral(":configure") ||
        taskText == QStringLiteral(":settings"))
    {
        KRunner::QueryMatch m(this);
        m.setText(i18n("Configure Notion Tasks"));
        m.setSubtext(i18n("Open the Notion Tasks configuration dialog"));
        m.setIconName(QStringLiteral("configure"));
        m.setRelevance(1.0);
        m.setData(QStringLiteral("::config"));
        context.addMatch(m);
        return;
    }

    if (taskText.isEmpty())
        return;

    const ParsedQuery pq = parseQuery(taskText);
    if (pq.taskName.isEmpty())
    {
        return;
    }

    QString subtext;
    if (cfg.token.isEmpty())
    {
        subtext = i18n("⚠ Notion token not configured — open KRunner settings to configure");
    }
    else
    {
        QStringList parts;
        if (!cfg.defaultProject.isEmpty())
            parts << cfg.defaultProject;
        if (!pq.priority.isEmpty())
            parts << i18n("Priority: %1", pq.priority);
        if (!pq.dueDate.isEmpty())
            parts << i18n("Due: %1", pq.dueDate);
        subtext = parts.isEmpty() ? i18n("Add to Notion Tasks")
                                  : parts.join(QStringLiteral(" · "));
    }

    KRunner::QueryMatch m(this);
    m.setText(i18n("Add: \"%1\"", pq.taskName));
    m.setSubtext(subtext);
    m.setIconName(QStringLiteral("task-new"));
    m.setRelevance(1.0);
    m.setData(taskText);
    context.addMatch(m);
}

void NotionRunner::run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match)
{
    Q_UNUSED(context)
    if (match.data().toString() == QStringLiteral("::config"))
        return;

    const Config cfg = loadConfig();
    if (cfg.token.isEmpty())
    {
        auto *n = new KNotification(QStringLiteral("taskError"), KNotification::CloseOnTimeout);
        n->setComponentName(NOTIFY_COMP);
        n->setTitle(i18n("Notion Tasks — not configured"));
        n->setText(i18n("Open KRunner settings → Notion Tasks → wrench icon"));
        n->setIconName(QStringLiteral("dialog-warning"));
        n->sendEvent();
        return;
    }

    const ParsedQuery pq = parseQuery(match.data().toString());
    if (pq.taskName.isEmpty())
    {
        return;
    }

    const auto result = addNotionTask(cfg, pq);
    if (result.isOk())
    {
        QStringList parts;
        if (!cfg.defaultProject.isEmpty())
            parts << cfg.defaultProject;
        if (!pq.priority.isEmpty())
            parts << pq.priority;
        if (!pq.dueDate.isEmpty())
            parts << pq.dueDate;

        auto *n = new KNotification(QStringLiteral("taskAdded"), KNotification::CloseOnTimeout);
        n->setComponentName(NOTIFY_COMP);
        n->setTitle(i18n("Task added: \"%1\"", pq.taskName));
        n->setText(parts.join(QStringLiteral(" · ")));
        n->setIconName(QStringLiteral("task-new"));
        n->sendEvent();
    }
    else
    {
        auto *n = new KNotification(QStringLiteral("taskError"), KNotification::CloseOnTimeout);
        n->setComponentName(NOTIFY_COMP);
        n->setTitle(i18n("Notion Tasks — error"));
        n->setText(result.message.left(200));
        n->setIconName(QStringLiteral("dialog-error"));
        n->sendEvent();
    }
}

#include "notionrunner.moc"
