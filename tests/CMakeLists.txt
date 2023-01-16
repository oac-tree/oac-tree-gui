enable_testing()

find_package(GTest REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Test REQUIRED)
find_package(Threads)
include(GoogleTest)

add_subdirectory(libtest-utils)
add_subdirectory(testsup-gui-core)
add_subdirectory(testsequencer-gui-core)
add_subdirectory(testsequencer-gui-core-events)
add_subdirectory(testsup-pvmonitor-core)

if (NOT SEQUENCERGUI_CODAC)
  add_subdirectory(testanyvalue-editor-core)
endif()

# Add custom target `make testall` which will run test executables one after another.
# This can be used instead of usual `make test` or `ctest` while running tests via an ssh
# connection. In certain cases `make testall` will be faster than `ctest -j4` because of fewer
# instantiations of QApplication.

add_custom_target(testall
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testsup-gui-core
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testanyvalue-editor-core
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testsequencer-gui-core
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testsequencer-gui-core-events
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testsup-pvmonitor-core
)
