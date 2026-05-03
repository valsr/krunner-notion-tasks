# Install script for directory: /work/krunner-notion-tasks

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/rado/.local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "krunner-notion-tasks" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/krunner_notion_tasks.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/krunner_notion_tasks.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/krunner_notion_tasks.so"
         RPATH "/home/rado/.local/lib")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/rado/.local/lib/qt6/plugins/kf6/krunner/krunner_notion_tasks.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/rado/.local/lib/qt6/plugins/kf6/krunner" TYPE MODULE FILES "/work/krunner-notion-tasks/build/bin/kf6/krunner/krunner_notion_tasks.so")
  if(EXISTS "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/krunner_notion_tasks.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/krunner_notion_tasks.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/krunner_notion_tasks.so"
         OLD_RPATH ":::::::::::::::::::::"
         NEW_RPATH "/home/rado/.local/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/krunner_notion_tasks.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "krunner-notion-tasks" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/work/krunner-notion-tasks/build/CMakeFiles/krunner_notion_tasks.dir/install-cxx-module-bmi-Release.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "krunner-notion-tasks" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/kcms/kcm_notion_tasks.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/kcms/kcm_notion_tasks.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/kcms/kcm_notion_tasks.so"
         RPATH "/home/rado/.local/lib")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/rado/.local/lib/qt6/plugins/kf6/krunner/kcms/kcm_notion_tasks.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/rado/.local/lib/qt6/plugins/kf6/krunner/kcms" TYPE MODULE FILES "/work/krunner-notion-tasks/build/bin/kf6/krunner/kcms/kcm_notion_tasks.so")
  if(EXISTS "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/kcms/kcm_notion_tasks.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/kcms/kcm_notion_tasks.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/kcms/kcm_notion_tasks.so"
         OLD_RPATH ":::::::::::::::::::::"
         NEW_RPATH "/home/rado/.local/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/rado/.local/lib/qt6/plugins/kf6/krunner/kcms/kcm_notion_tasks.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "krunner-notion-tasks" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/work/krunner-notion-tasks/build/CMakeFiles/kcm_notion_tasks.dir/install-cxx-module-bmi-Release.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "krunner-notion-tasks" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/rado/.local/share/knotifications6/krunner-notion-tasks.notifyrc")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/rado/.local/share/knotifications6" TYPE FILE FILES "/work/krunner-notion-tasks/krunner-notion-tasks.notifyrc")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/work/krunner-notion-tasks/build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/work/krunner-notion-tasks/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
