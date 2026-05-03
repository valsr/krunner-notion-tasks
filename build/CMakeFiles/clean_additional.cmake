# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles/kcm_notion_tasks_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/kcm_notion_tasks_autogen.dir/ParseCache.txt"
  "CMakeFiles/krunner_notion_tasks_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/krunner_notion_tasks_autogen.dir/ParseCache.txt"
  "kcm_notion_tasks_autogen"
  "krunner_notion_tasks_autogen"
  )
endif()
