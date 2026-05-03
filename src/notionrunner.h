#pragma once

#include "apiresult.h"

#include <KRunner/AbstractRunner>
#include <QDateTime>
#include <QHash>
#include <QJsonArray>
#include <QMutex>
#include <QVariant>

class NotionRunner : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    NotionRunner(QObject *parent, const KPluginMetaData &data);

    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;
    void reloadConfiguration() override;

private:
    struct Config {
        QString     token;
        QString     databaseId;
        QString     projectsDbId;
        QStringList keywords;
        QString     defaultProject;
        QString     templateId;
    };

    struct ParsedQuery {
        QString taskName;
        QString priority;
        QString dueDate;
    };

    struct CacheEntry {
        QVariant  value;
        QDateTime expiry;
    };

    Config      loadConfig() const;
    ParsedQuery parseQuery(const QString &text) const;
    QString     toUuid(const QString &id) const;

    ApiResult<QByteArray> notionRequest(const QString &method, const QString &path,
                                        const QByteArray &body, const QString &token) const;
    ApiResult<QJsonArray> dbQuery(const QString &dbId, const QJsonObject &filter,
                                   const QString &token) const;

    QString resolveProjectId(const Config &cfg, const QString &name);

    ApiResult<bool> addNotionTask(const Config &cfg, const ParsedQuery &pq);

    template<typename Fn>
    QVariant cached(const QString &key, int ttlSecs, Fn &&fn);

    mutable QMutex             m_cacheMutex;
    QHash<QString, CacheEntry> m_cache;
};
