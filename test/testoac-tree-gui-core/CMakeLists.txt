# Tests for oac-tree-gui-core library.

set(test testoac-tree-gui-core)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})

target_link_libraries(${test} PRIVATE oac-tree-gui-core oac-tree-gui-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_TIMEOUT 5 DISCOVERY_MODE PRE_TEST TEST_PREFIX Core.)
