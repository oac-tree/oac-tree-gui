set(library_name sup-pvmonitor-core)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} SHARED "")

add_subdirectory(suppvmonitor)

target_link_libraries(${library_name} PUBLIC pthread sup-mvvm::view sup-dto::sup-dto)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)
