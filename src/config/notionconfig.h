#pragma once

#include <KCModule>

namespace Ui { class NotionConfig; }

class NotionConfig : public KCModule
{
    Q_OBJECT

public:
    explicit NotionConfig(QObject *parent, const KPluginMetaData &data);
    ~NotionConfig() override;

    void load() override;
    void save() override;
    void defaults() override;

private Q_SLOTS:
    void testConnection();

private:
    Ui::NotionConfig *m_ui;
};
