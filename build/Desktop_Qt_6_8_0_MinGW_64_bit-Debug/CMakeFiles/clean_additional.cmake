# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MyIDE_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MyIDE_autogen.dir\\ParseCache.txt"
  "MyIDE_autogen"
  )
endif()
