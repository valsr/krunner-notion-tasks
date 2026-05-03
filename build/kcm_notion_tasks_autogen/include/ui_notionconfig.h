/********************************************************************************
** Form generated from reading UI file 'notionconfig.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTIONCONFIG_H
#define UI_NOTIONCONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NotionConfig
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *connectionGroup;
    QFormLayout *connForm;
    QLabel *tokenLabel;
    QLineEdit *tokenEdit;
    QLabel *dbIdLabel;
    QLineEdit *dbIdEdit;
    QGroupBox *behaviourGroup;
    QFormLayout *runForm;
    QLabel *keywordLabel;
    QLineEdit *keywordEdit;
    QLabel *projectLabel;
    QLineEdit *projectEdit;
    QLabel *templateLabel;
    QLineEdit *templateEdit;
    QGroupBox *advancedGroup;
    QFormLayout *advForm;
    QLabel *projectsDbLabel;
    QLineEdit *projectsDbEdit;
    QHBoxLayout *testLayout;
    QPushButton *testDbBtn;
    QLabel *statusLabel;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *NotionConfig)
    {
        if (NotionConfig->objectName().isEmpty())
            NotionConfig->setObjectName("NotionConfig");
        NotionConfig->resize(560, 440);
        verticalLayout = new QVBoxLayout(NotionConfig);
        verticalLayout->setObjectName("verticalLayout");
        connectionGroup = new QGroupBox(NotionConfig);
        connectionGroup->setObjectName("connectionGroup");
        connForm = new QFormLayout(connectionGroup);
        connForm->setObjectName("connForm");
        connForm->setRowWrapPolicy(QFormLayout::WrapLongRows);
        tokenLabel = new QLabel(connectionGroup);
        tokenLabel->setObjectName("tokenLabel");

        connForm->setWidget(0, QFormLayout::ItemRole::LabelRole, tokenLabel);

        tokenEdit = new QLineEdit(connectionGroup);
        tokenEdit->setObjectName("tokenEdit");
        tokenEdit->setEchoMode(QLineEdit::Password);

        connForm->setWidget(0, QFormLayout::ItemRole::FieldRole, tokenEdit);

        dbIdLabel = new QLabel(connectionGroup);
        dbIdLabel->setObjectName("dbIdLabel");

        connForm->setWidget(1, QFormLayout::ItemRole::LabelRole, dbIdLabel);

        dbIdEdit = new QLineEdit(connectionGroup);
        dbIdEdit->setObjectName("dbIdEdit");

        connForm->setWidget(1, QFormLayout::ItemRole::FieldRole, dbIdEdit);


        verticalLayout->addWidget(connectionGroup);

        behaviourGroup = new QGroupBox(NotionConfig);
        behaviourGroup->setObjectName("behaviourGroup");
        runForm = new QFormLayout(behaviourGroup);
        runForm->setObjectName("runForm");
        runForm->setRowWrapPolicy(QFormLayout::WrapLongRows);
        keywordLabel = new QLabel(behaviourGroup);
        keywordLabel->setObjectName("keywordLabel");

        runForm->setWidget(0, QFormLayout::ItemRole::LabelRole, keywordLabel);

        keywordEdit = new QLineEdit(behaviourGroup);
        keywordEdit->setObjectName("keywordEdit");

        runForm->setWidget(0, QFormLayout::ItemRole::FieldRole, keywordEdit);

        projectLabel = new QLabel(behaviourGroup);
        projectLabel->setObjectName("projectLabel");

        runForm->setWidget(1, QFormLayout::ItemRole::LabelRole, projectLabel);

        projectEdit = new QLineEdit(behaviourGroup);
        projectEdit->setObjectName("projectEdit");

        runForm->setWidget(1, QFormLayout::ItemRole::FieldRole, projectEdit);

        templateLabel = new QLabel(behaviourGroup);
        templateLabel->setObjectName("templateLabel");

        runForm->setWidget(2, QFormLayout::ItemRole::LabelRole, templateLabel);

        templateEdit = new QLineEdit(behaviourGroup);
        templateEdit->setObjectName("templateEdit");

        runForm->setWidget(2, QFormLayout::ItemRole::FieldRole, templateEdit);


        verticalLayout->addWidget(behaviourGroup);

        advancedGroup = new QGroupBox(NotionConfig);
        advancedGroup->setObjectName("advancedGroup");
        advancedGroup->setCheckable(true);
        advancedGroup->setChecked(false);
        advForm = new QFormLayout(advancedGroup);
        advForm->setObjectName("advForm");
        advForm->setRowWrapPolicy(QFormLayout::WrapLongRows);
        projectsDbLabel = new QLabel(advancedGroup);
        projectsDbLabel->setObjectName("projectsDbLabel");

        advForm->setWidget(0, QFormLayout::ItemRole::LabelRole, projectsDbLabel);

        projectsDbEdit = new QLineEdit(advancedGroup);
        projectsDbEdit->setObjectName("projectsDbEdit");

        advForm->setWidget(0, QFormLayout::ItemRole::FieldRole, projectsDbEdit);


        verticalLayout->addWidget(advancedGroup);

        testLayout = new QHBoxLayout();
        testLayout->setObjectName("testLayout");
        testDbBtn = new QPushButton(NotionConfig);
        testDbBtn->setObjectName("testDbBtn");

        testLayout->addWidget(testDbBtn);

        statusLabel = new QLabel(NotionConfig);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setWordWrap(true);

        testLayout->addWidget(statusLabel);


        verticalLayout->addLayout(testLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(NotionConfig);

        QMetaObject::connectSlotsByName(NotionConfig);
    } // setupUi

    void retranslateUi(QWidget *NotionConfig)
    {
        connectionGroup->setTitle(QCoreApplication::translate("NotionConfig", "Connection", nullptr));
        tokenLabel->setText(QCoreApplication::translate("NotionConfig", "Notion Token:", nullptr));
        tokenEdit->setPlaceholderText(QCoreApplication::translate("NotionConfig", "ntn_\342\200\246", nullptr));
        dbIdLabel->setText(QCoreApplication::translate("NotionConfig", "Tasks Data Source ID:", nullptr));
        behaviourGroup->setTitle(QCoreApplication::translate("NotionConfig", "Behaviour", nullptr));
        keywordLabel->setText(QCoreApplication::translate("NotionConfig", "Trigger keywords:", nullptr));
        keywordEdit->setPlaceholderText(QCoreApplication::translate("NotionConfig", "task,todo", nullptr));
        projectLabel->setText(QCoreApplication::translate("NotionConfig", "Default project:", nullptr));
        projectEdit->setPlaceholderText(QCoreApplication::translate("NotionConfig", "Todos  (leave blank to skip)", nullptr));
        templateLabel->setText(QCoreApplication::translate("NotionConfig", "Template page ID:", nullptr));
        templateEdit->setPlaceholderText(QCoreApplication::translate("NotionConfig", "32-char page ID  (leave blank to skip)", nullptr));
        advancedGroup->setTitle(QCoreApplication::translate("NotionConfig", "Advanced (only change for a different workspace)", nullptr));
        projectsDbLabel->setText(QCoreApplication::translate("NotionConfig", "Projects Database ID:", nullptr));
        testDbBtn->setText(QCoreApplication::translate("NotionConfig", "Test connection", nullptr));
        statusLabel->setText(QString());
        (void)NotionConfig;
    } // retranslateUi

};

namespace Ui {
    class NotionConfig: public Ui_NotionConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTIONCONFIG_H
