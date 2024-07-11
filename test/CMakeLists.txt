if(NOT COA_BUILD_TESTS)
  return()
endif()

enable_testing()

find_package(GTest 1.10 QUIET CONFIG)
if (NOT GTest_FOUND)  
  message(WARNING "GTest with version >= \"1.10\" was not found, fetching from internet" )
  fetch_googletest()
endif()
find_package_message(
  GTest_DETAILS
  "Found GTest: ${GTest_DIR} (version \"${GTest_VERSION}\")"
  "[${GTest_FOUND}][${GTest_DIR}][v${GTest_VERSION}]")

include(GoogleTest)

add_subdirectory(libtest-utils)
add_subdirectory(testsequencer-gui-core)
add_subdirectory(testsequencer-gui-components)
add_subdirectory(testsequencer-gui-components-app)
add_subdirectory(testsequencer-gui-views)

# Add custom target `make testall` which will run test executables one after another.
# This can be used instead of usual `make test` or `ctest` while running tests via an ssh
# connection. In certain cases `make testall` will be faster than `ctest -j4` because of fewer
# instantiations of QApplication.

add_custom_target(testall
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-core
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-components
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-components-app
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-views
)

file(WRITE ${TEST_OUTPUT_DIRECTORY}/test.sh
"#!/bin/bash
export QT_QPA_PLATFORM=offscreen
fail=0
" ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-core " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-components " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-components-app " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testsequencer-gui-views " || fail=1
exit $fail
")

configure_file(${TEST_OUTPUT_DIRECTORY}/test.sh ${TEST_OUTPUT_DIRECTORY}/test.sh 
               FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
