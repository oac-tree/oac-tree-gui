set(test testsequencergui)

enable_testing()
find_package(GTest REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core Test REQUIRED)
find_package(Threads)
include(GoogleTest)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_executable(${test} ${source_files} ${include_files})

if (SEQUENCERGUI_CODAC)
  target_link_libraries(${test} PRIVATE sequencerguicore pthread gtest gmock Qt5::Core Qt5::Test)
  target_include_directories(${test} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../source)
  gtest_discover_tests(${test} DISCOVERY_TIMEOUT 30)
else()
  target_link_libraries(${test} PRIVATE sequencerguicore testmachinery)
  target_include_directories(${test} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../source)
  gtest_discover_tests(${test})
endif()
