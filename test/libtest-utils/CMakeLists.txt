set(library_name oac-tree-gui-test-utils)

add_library(${library_name} STATIC "")

add_subdirectory(testutils)

target_link_libraries(${library_name} PUBLIC pthread GTest::gtest GTest::gmock Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test oac-tree-gui-components sup-mvvm::test)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)
