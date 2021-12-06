set(test testsequencergui)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE sequencerguicore testmachinery)
target_include_directories(${test} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

target_include_directories(${test}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../../source/libsequencer/cli
    ${CMAKE_CURRENT_LIST_DIR}/../../../source/libsequencer/daemon
    ${CMAKE_CURRENT_LIST_DIR}/../../../source/libsequencer/include
    ${CMAKE_CURRENT_LIST_DIR}/../../../source/libsequencer/instructions
    ${CMAKE_CURRENT_LIST_DIR}/../../../source/libsequencer/variables
    ${CMAKE_CURRENT_LIST_DIR}/../../../source/libsequencer/parser
    ${CMAKE_CURRENT_LIST_DIR}/../../../source/libsequencer/procedure
    )

gtest_discover_tests(${test} TEST_PREFIX "Examples.")

