set(test testsup-pvmonitor-core)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})

target_link_libraries(${test} PRIVATE sup-pvmonitor-core test-utils)

gtest_discover_tests(${test} DISCOVERY_MODE PRE_TEST)
