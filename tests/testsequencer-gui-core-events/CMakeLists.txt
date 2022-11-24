set(test testsequencer-gui-core-events)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})

target_link_libraries(${test} PRIVATE sequencer-gui-core testmachinery)

gtest_discover_tests(${test} DISCOVERY_TIMEOUT 10)
