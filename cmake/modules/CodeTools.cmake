# Collection of functions to set-up code beautification and analysis
include(ClangFormat)

# List of targets for project code beautification.
set(BEAUTIFICATION_TARGETS sequencer-gui-core sequencer-gui-components sequencer-gui-views
  testsequencer-gui-core testsequencer-gui-components testsequencer-gui-components-app testsequencer-gui-views)

# Defines new target for 'clangformat' to beautify whole project.
# Use 'make clangformat' or 'cmake --build . --target clangformat' to beautify the code.
# Beautification settings are located in .clang-format in project directory.

function(project_clangformat_setup)
    set(all_sources)
    foreach(target ${BEAUTIFICATION_TARGETS})
        get_target_property(target_sources ${target} SOURCES)
        list(APPEND all_sources ${target_sources})
    endforeach()
   clangformat_setup(${all_sources})
endfunction()


# Setups targets for code processing.

function(project_codetools_setup)
    if (COA_SETUP_CLANGFORMAT)
        project_clangformat_setup()
    endif()
endfunction()

# Fetches google test version 1.12.1
function(fetch_googletest)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850)
FetchContent_MakeAvailable(googletest)
  message(VERBOSE "GTest binaries are present at ${googletest_BINARY_DIR}")
endfunction()

