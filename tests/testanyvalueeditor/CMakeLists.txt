set(test testanyvalueeditorcore)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})

target_link_libraries(${test} PRIVATE anyvalueeditorcore testmachinery)

gtest_discover_tests(${test})
