# Build a tiny dummy shared library to validate loader behavior with an existing .so
add_library(domain_loader_dummy SHARED domain_loader_dummy.cpp)
set_target_properties(domain_loader_dummy PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
)

# Provide the absolute path of the dummy library to the test via a macro
target_compile_definitions(${test} PRIVATE DUMMY_LIB_PATH="$<TARGET_FILE:domain_loader_dummy>")
