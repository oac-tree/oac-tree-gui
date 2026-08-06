# Setup Qt6 or Qt5 depending on COA_USE_QT6 flag

if(COA_USE_QT6)
  set(QT_VERSION_MAJOR 6)
  set(QT_FIND_COMPONENTS Widgets Core Gui PrintSupport Charts Test)
else()
  set(QT_VERSION_MAJOR 5)
  set(QT_FIND_COMPONENTS Widgets Core Gui PrintSupport Charts Test)
endif()

if(CODAC_FOUND)
  # Set NO_CMAKE_PATH and PATHS to CMAKE_PREFIX_PATH, so that find_package will use the system Qt first
  # if it finds one, but still look in CMAKE_PREFIX_PATH as a last resort. This gives system Qt priority over CODAC Qt
  set(QT_FIND_OPTIONS NO_CMAKE_PATH PATHS ${CMAKE_PREFIX_PATH})
endif()

find_package(Qt${QT_VERSION_MAJOR} QUIET REQUIRED ${QT_FIND_OPTIONS} COMPONENTS ${QT_FIND_COMPONENTS})

# Gather details about the Qt package found
set(QT_VERSION "${Qt${QT_VERSION_MAJOR}_VERSION_MAJOR}.${Qt${QT_VERSION_MAJOR}_VERSION_MINOR}.${Qt${QT_VERSION_MAJOR}_VERSION_PATCH}")
set(QT_DETAILS "[${Qt${QT_VERSION_MAJOR}_DIR}][v${QT_VERSION}]")
foreach(QT_COMPONENT ${QT_FIND_COMPONENTS})
  get_target_property(QT_${QT_COMPONENT}_LOCATION Qt${QT_VERSION_MAJOR}::${QT_COMPONENT} LOCATION)
  string(PREPEND QT_DETAILS "[${QT_COMPONENT}:${QT_${QT_COMPONENT}_LOCATION}]")
endforeach()

# Print the message about finding Qt if it has not been printed yet, or if the details have changed
# This is essentially a reimplementaion of find_package_message, but with more detailed information gated by verbosity
if(NOT "${QT_DETAILS}" STREQUAL "${FIND_PACKAGE_MESSAGE_DETAILS_QT}")
  message(STATUS "Found Qt: ${Qt${QT_VERSION_MAJOR}_DIR} (version \"${QT_VERSION}\")")
  foreach(QT_COMPONENT ${QT_FIND_COMPONENTS})
    message(VERBOSE "\tQt::${QT_COMPONENT}: ${QT_${QT_COMPONENT}_LOCATION}")
  endforeach()
  set(FIND_PACKAGE_MESSAGE_DETAILS_QT ${QT_DETAILS} CACHE INTERNAL "Details about finding Qt")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
