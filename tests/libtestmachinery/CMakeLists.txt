set(library_name testmachinery)

file(GLOB source_files "*.cpp")
file(GLOB include_files "*.h")

add_library(${library_name} STATIC ${source_files} ${include_files})
target_link_libraries(${library_name} PUBLIC pthread gtest gmock Qt5::Core Qt5::Test)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)
