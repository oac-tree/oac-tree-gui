# Tests for sequencer-gui-components library, which require QApplication.

set(test testsequencer-gui-integration)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})

target_link_libraries(${test} PRIVATE sequencer-gui-components test-utils sup-epics::sup-epics-test)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_TIMEOUT 10 DISCOVERY_MODE PRE_TEST TEST_PREFIX Integration.)
