set(test testanyvalue-editor-core)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})

target_link_libraries(${test} PRIVATE anyvalue-editor-core testmachinery)

gtest_discover_tests(${test})
