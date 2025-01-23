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
add_subdirectory(testoac-tree-gui-core)
add_subdirectory(testoac-tree-gui-components)
add_subdirectory(testoac-tree-gui-components-app)
add_subdirectory(testoac-tree-gui-views)
if (COA_BUILD_INTEGRATION_TESTS)
  add_subdirectory(testoac-tree-gui-integration)
endif()
add_subdirectory(parasoft)

# -----------------------------------------------------------------------------
# Add custom target `make testall` to run test executables one after another.
# -----------------------------------------------------------------------------

# This can be used instead of usual `make test` or `ctest` while running tests via an ssh
# connection. In certain cases `make testall` will be faster than `ctest -j4` because of fewer
# instantiations of QApplication.

add_custom_target(testall DEPENDS testall-cmd)
add_custom_command(OUTPUT testall-cmd
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testoac-tree-gui-core
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testoac-tree-gui-components
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testoac-tree-gui-components-app
  COMMAND ${TEST_OUTPUT_DIRECTORY}/testoac-tree-gui-views
)

if (COA_BUILD_INTEGRATION_TESTS)
  add_custom_command(OUTPUT testall-cmd APPEND
    COMMAND ${TEST_OUTPUT_DIRECTORY}/testoac-tree-gui-integration
  )
endif()

# -----------------------------------------------------------------------------
# Generate test.sh for Maven
# -----------------------------------------------------------------------------
file(WRITE ${TEST_OUTPUT_DIRECTORY}/test.sh
"#!/bin/bash
export QT_QPA_PLATFORM=offscreen
fail=0
" ${TEST_OUTPUT_DIRECTORY}/testoac-tree-gui-core " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testoac-tree-gui-components " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testoac-tree-gui-components-app " || fail=1
" ${TEST_OUTPUT_DIRECTORY}/testoac-tree-gui-views " || fail=1
")
if (COA_BUILD_INTEGRATION_TESTS)
  file(APPEND ${TEST_OUTPUT_DIRECTORY}/test.sh "${TEST_OUTPUT_DIRECTORY}/testoac-tree-gui-integration || fail=1
")
endif()
file(APPEND ${TEST_OUTPUT_DIRECTORY}/test.sh "exit $fail")
configure_file(${TEST_OUTPUT_DIRECTORY}/test.sh ${TEST_OUTPUT_DIRECTORY}/test.sh 
               FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
