if(CODAC_FOUND)
  add_definitions(-DSEQUENCERGUI_CODAC)
endif()

add_subdirectory(app)
add_subdirectory(lib)
