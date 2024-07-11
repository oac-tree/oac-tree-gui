# Tests for sequencer-gui-components library.
set(test testsequencer-gui-components)

find_package(sup-epics REQUIRED)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})

target_link_libraries(${test} PRIVATE sequencer-gui-components test-utils PRIVATE sup-epics::sup-epics-test)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_MODE PRE_TEST TEST_PREFIX Components.)
