# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/ChoreDecide_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ChoreDecide_autogen.dir/ParseCache.txt"
  "ChoreDecide_autogen"
  )
endif()
