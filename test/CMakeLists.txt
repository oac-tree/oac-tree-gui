enable_testing()

find_package(GTest 1.10 QUIET CONFIG)
if (GTest_FOUND)
  message(STATUS "Found GTest: suitable version \"${GTest_VERSION}\", with minimum required \"1.10\"" )
else()
  message(STATUS "GTest with version >= \"1.10\" was not found, fetching from internet" )
  fetch_googletest()
endif()


find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Test REQUIRED)
find_package(Threads)
include(GoogleTest)

add_subdirectory(libtest-utils)
add_subdirectory(testsequencer-gui-core)
add_subdirectory(testsequencer-gui-integration)
add_subdirectory(testsequencer-gui-components)

# Add custom target `make testall` which will run test executables one after another.
# This can be used instead of usual `make test` or `ctest` while running tests via an ssh
# connection. In certain cases `make testall` will be faster than `ctest -j4` because of fewer
# instantiations of QApplication.

add_custom_target(testall
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-core
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-components
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-integration
)

file(WRITE ${TEST_OUTPUT_DIRECTORY}/test.sh
"#!/bin/bash
export QT_QPA_PLATFORM=offscreen
fail=0
" ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-core " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-components " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-integration " || fail=1
exit $fail
")

configure_file(${TEST_OUTPUT_DIRECTORY}/test.sh ${TEST_OUTPUT_DIRECTORY}/test.sh 
               FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
