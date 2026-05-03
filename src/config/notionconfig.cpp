#include "notionconfig.h"
#include "ui_notionconfig.h"

#include "../apiresult.h"

#include <KSharedConfig>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KPluginFactory>

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

K_PLUGIN_CLASS_WITH_JSON(NotionConfig, "notionconfig.json")

static const QString CFG_FILE        = QStringLiteral("krunner-notion-tasksrc");
static const QString CFG_NOTION      = QStringLiteral("notion");
static const QString CFG_RUNNER      = QStringLiteral("runner");
static const QString NOTION_VER      = QStringLiteral("2022-06-28");

static const QString DEF_DB_ID       = QStringLiteral("5a53f65190c44c81ac64552a6dd584dc");
static const QString DEF_PROJECTS_ID = QStringLiteral("72a3073d-a63e-4445-9244-2cbc1f651a80");
static const QString DEF_PROJECT     = QStringLiteral("Todos");
static const QString DEF_TEMPLATE    = QStringLiteral("353e2db9c65e80b697c3c8e3abbef67b");
static const QString DEF_KEYWORD     = QStringLiteral("task,todo");

static QString toUuid(const QString &id)
{
    QString s = id;
    s.remove(QLatin1Char('-'));
    if (s.length() != 32) return id;
    return QStringLiteral("%1-%2-%3-%4-%5")
        .arg(s.mid(0,8), s.mid(8,4), s.mid(12,4), s.mid(16,4), s.mid(20));
}

static ApiResult<QByteArray> notionGet(const QString &token, const QString &path)
{
    QNetworkAccessManager nam;
    nam.setTransferTimeout(10000);
    QNetworkRequest req(QUrl(QStringLiteral("https://api.notion.com/v1/") + path));
    req.setRawHeader("Authorization", QByteArrayLiteral("Bearer ") + token.toUtf8());
    req.setRawHeader("Notion-Version", NOTION_VER.toUtf8());
    QNetworkReply *reply = nam.get(req);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    const QByteArray result = reply->readAll();
    if (reply->error() != QNetworkReply::NoError) {
        const QJsonDocument doc = QJsonDocument::fromJson(result);
        const QString msg = doc.object().value(QStringLiteral("message")).toString();
        const QString fallback = reply->errorString();
        reply->deleteLater();
        return ApiResult<QByteArray>::err(msg.isEmpty() ? fallback : msg);
    }
    reply->deleteLater();
    return ApiResult<QByteArray>::ok(result);
}

static ApiResult<QByteArray> notionPost(const QString &token, const QString &path,
                                         const QByteArray &body)
{
    QNetworkAccessManager nam;
    nam.setTransferTimeout(10000);
    QNetworkRequest req(QUrl(QStringLiteral("https://api.notion.com/v1/") + path));
    req.setRawHeader("Authorization", QByteArrayLiteral("Bearer ") + token.toUtf8());
    req.setRawHeader("Notion-Version", NOTION_VER.toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    QNetworkReply *reply = nam.post(req, body);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    const QByteArray result = reply->readAll();
    if (reply->error() != QNetworkReply::NoError) {
        const QJsonDocument doc = QJsonDocument::fromJson(result);
        const QString msg = doc.object().value(QStringLiteral("message")).toString();
        const QString fallback = reply->errorString();
        reply->deleteLater();
        return ApiResult<QByteArray>::err(msg.isEmpty() ? fallback : msg);
    }
    reply->deleteLater();
    return ApiResult<QByteArray>::ok(result);
}

NotionConfig::NotionConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
    , m_ui(new Ui::NotionConfig)
{
    m_ui->setupUi(widget());

    connect(m_ui->testDbBtn, &QPushButton::clicked, this, &NotionConfig::testConnection);

    const auto markChanged = [this] { setNeedsSave(true); };
    connect(m_ui->tokenEdit,       &QLineEdit::textChanged,  this, markChanged);
    connect(m_ui->dbIdEdit,        &QLineEdit::textChanged,  this, markChanged);
    connect(m_ui->keywordEdit,     &QLineEdit::textChanged,  this, markChanged);
    connect(m_ui->projectEdit,     &QLineEdit::textChanged,  this, markChanged);
    connect(m_ui->templateEdit,    &QLineEdit::textChanged,  this, markChanged);
    connect(m_ui->projectsDbEdit,  &QLineEdit::textChanged,  this, markChanged);
}

NotionConfig::~NotionConfig()
{
    delete m_ui;
}

void NotionConfig::load()
{
    const auto cfg    = KSharedConfig::openConfig(CFG_FILE);
    const auto notion = cfg->group(CFG_NOTION);
    const auto runner = cfg->group(CFG_RUNNER);

    m_ui->tokenEdit->setText(      notion.readEntry("token",                QString()));
    m_ui->dbIdEdit->setText(       notion.readEntry("data_source_id",       DEF_DB_ID));
    m_ui->projectsDbEdit->setText( notion.readEntry("projects_database_id", DEF_PROJECTS_ID));
    m_ui->keywordEdit->setText(    runner.readEntry("keyword",              DEF_KEYWORD));
    m_ui->projectEdit->setText(    runner.readEntry("default_project",      DEF_PROJECT));
    m_ui->templateEdit->setText(   runner.readEntry("template_id",          DEF_TEMPLATE));

    setNeedsSave(false);
}

void NotionConfig::save()
{
    const auto cfg    = KSharedConfig::openConfig(CFG_FILE);
    auto notion       = cfg->group(CFG_NOTION);
    auto runner       = cfg->group(CFG_RUNNER);

    notion.writeEntry("token",                m_ui->tokenEdit->text().trimmed());
    notion.writeEntry("data_source_id",       m_ui->dbIdEdit->text().trimmed());
    notion.writeEntry("projects_database_id", m_ui->projectsDbEdit->text().trimmed());

    const QString kw = m_ui->keywordEdit->text().trimmed();
    runner.writeEntry("keyword",         kw.isEmpty() ? DEF_KEYWORD : kw);
    runner.writeEntry("default_project", m_ui->projectEdit->text().trimmed());
    runner.writeEntry("template_id",     m_ui->templateEdit->text().trimmed());

    cfg->sync();
    setNeedsSave(false);
}

void NotionConfig::defaults()
{
    m_ui->tokenEdit->clear();
    m_ui->dbIdEdit->setText(      DEF_DB_ID);
    m_ui->projectsDbEdit->setText(DEF_PROJECTS_ID);
    m_ui->keywordEdit->setText(   DEF_KEYWORD);
    m_ui->projectEdit->setText(   DEF_PROJECT);
    m_ui->templateEdit->setText(  DEF_TEMPLATE);
    setNeedsSave(true);
}

void NotionConfig::testConnection()
{
    const QString token = m_ui->tokenEdit->text().trimmed();
    const QString dbId  = m_ui->dbIdEdit->text().trimmed();

    if (token.isEmpty()) {
        m_ui->statusLabel->setStyleSheet(QStringLiteral("color: red;"));
        m_ui->statusLabel->setText(i18n("✗ Token is empty"));
        return;
    }

    m_ui->testDbBtn->setEnabled(false);
    m_ui->statusLabel->setStyleSheet(QString());
    m_ui->statusLabel->setText(i18n("Testing…"));
    qApp->processEvents();

    const QByteArray body = QJsonDocument(QJsonObject{
        {QStringLiteral("page_size"), 1}
    }).toJson(QJsonDocument::Compact);
    const auto res = notionPost(token,
        QStringLiteral("databases/") + toUuid(dbId) + QStringLiteral("/query"), body);
    if (res.isOk()) {
        m_ui->statusLabel->setStyleSheet(QStringLiteral("color: green;"));
        m_ui->statusLabel->setText(i18n("✓ Connected — data source accessible"));
    } else {
        m_ui->statusLabel->setStyleSheet(QStringLiteral("color: red;"));
        m_ui->statusLabel->setText(i18n("✗ %1", res.message));
    }

    m_ui->testDbBtn->setEnabled(true);
}

#include "notionconfig.moc"
