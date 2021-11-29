set(test testsequencergui)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE sequencerguicore)
target_include_directories(${test} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

gtest_discover_tests(${test} TEST_PREFIX "SequencerGUI.")

