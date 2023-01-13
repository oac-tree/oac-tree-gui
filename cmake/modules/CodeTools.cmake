# Collection of functions to set-up code beautification and analysis
include(ClangFormat)

# List of targets for project code beautification.
set(BEAUTIFICATION_TARGETS sequencer-gui-core sup-gui-core sup-pvmonitor-core)

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
    if (SEQUENCERGUI_SETUP_CLANGFORMAT)
        project_clangformat_setup()
    endif()
endfunction()
