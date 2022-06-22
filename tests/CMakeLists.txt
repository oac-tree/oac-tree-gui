enable_testing()

find_package(GTest REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core Test REQUIRED)
find_package(Threads)
include(GoogleTest)

add_subdirectory(libtestmachinery)
add_subdirectory(testsequencerguicore)
add_subdirectory(testsequencerguievents)
if (NOT SEQUENCERGUI_CODAC)
  add_subdirectory(testanyvalueeditor)
endif()

# Add custom target `make testall` which will run test executables one after another.
# This can be used instead of usual `make test` or `ctest` while running tests via an ssh
# connection. In certain cases `make testall` will be faster than `ctest -j4` because of fewer
# instantiations of QApplication.

add_custom_target(testall
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testanyvalueeditorcore
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testsequencerguievents
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testanyvalueeditorcore
)
