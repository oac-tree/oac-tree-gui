# Collection of functions used for handling dependencies

if(NOT COA_FETCH_DEPS)
  return()
endif()

message(STATUS "Building dependencies from source")

include(FetchContent)

FetchContent_Declare(
  oac-tree
  GIT_REPOSITORY https://github.com/oac-tree/oac-tree.git
  GIT_TAG develop
  OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
  oac-tree-server
  GIT_REPOSITORY https://github.com/oac-tree/oac-tree-server.git
  GIT_TAG develop
  OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
  sup-epics
  GIT_REPOSITORY https://github.com/oac-tree/sup-epics.git
  GIT_TAG develop
  OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
  sup-gui-core
  GIT_REPOSITORY https://github.com/oac-tree/sup-gui-core.git
  GIT_TAG develop
  OVERRIDE_FIND_PACKAGE
)
