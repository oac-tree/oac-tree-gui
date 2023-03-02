set(library_name sup-gui-core)

add_library(${library_name} SHARED "")

if(CODAC)
    add_definitions(-DSEQUENCERGUI_CODAC)
endif()

target_link_libraries(${library_name} PUBLIC pthread sup-mvvm::view sup-dto::sup-dto)

add_subdirectory(sup)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)

install(TARGETS ${library_name} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
