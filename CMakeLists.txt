cmake_minimum_required(VERSION 3.16)
project(krunner-notion-tasks VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(ECM 6.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs6)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)
find_package(KF6 REQUIRED COMPONENTS Runner KCMUtils Config I18n Notifications)

# ── Runner plugin ──────────────────────────────────────────────────────────────
kcoreaddons_add_plugin(krunner_notion_tasks
    SOURCES src/notionrunner.cpp
    INSTALL_NAMESPACE "kf6/krunner"
)
target_link_libraries(krunner_notion_tasks PRIVATE
    Qt6::Core
    Qt6::Network
    KF6::Runner
    KF6::ConfigCore
    KF6::I18n
    KF6::Notifications
)

# ── KCM config plugin ──────────────────────────────────────────────────────────
kcoreaddons_add_plugin(kcm_notion_tasks
    SOURCES src/config/notionconfig.cpp
    INSTALL_NAMESPACE "kf6/krunner/kcms"
)
target_link_libraries(kcm_notion_tasks PRIVATE
    Qt6::Widgets
    KF6::KCMUtils
    KF6::ConfigCore
    KF6::I18n
)
set_target_properties(kcm_notion_tasks PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src/config"
)

# ── Notification config ────────────────────────────────────────────────────────
install(FILES krunner-notion-tasks.notifyrc
    DESTINATION ${KDE_INSTALL_DATADIR}/knotifications6
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
