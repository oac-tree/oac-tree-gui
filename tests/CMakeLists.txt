enable_testing()

find_package(GTest REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core Test REQUIRED)
find_package(Threads)
include(GoogleTest)

add_subdirectory(libtestmachinery)
add_subdirectory(testsequencerguicore)
add_subdirectory(testsequencerguievents)
add_subdirectory(testanyvalueeditor)

