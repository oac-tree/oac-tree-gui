set(test testsequencergui)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

find_package(Qt5 COMPONENTS Widgets Core Test REQUIRED)
find_package(Threads)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})
target_link_libraries(${test} PRIVATE sequencerguicore pthread gtest gmock Qt5::Core Qt5::Test)

target_include_directories(${test} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../source)

gtest_discover_tests(${test} DISCOVERY_TIMEOUT 30)


