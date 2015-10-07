# Specify minimum CMake version
cmake_minimum_required(VERSION 2.8.3)

# Project name
project( FallenLondon-ItemConverter )

# Source files
SET( SOURCES
	FileIO.cpp
	MainWindow.cpp
	Utils.cpp
)

# Moc Header files
SET( MOC_HEADERS
	MainWindow.h
)

# Header files
SET( HEADERS
	Conversion.h
	FileIO.h
	ItemCategory.h
	Utils.h
)

# UI files
SET( UIS
	MainWindow.ui
)

# Find installed Qt4 libraries and headers
find_package( Qt4 REQUIRED )

# Adds useful macros and variables
# this is needed to correctly link the qt library through target_link_libraries
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

# Run Qts user interface compiler uic on .ui files
QT4_WRAP_UI( UI_HEADERS ${UIS} )

# Run Qts meta object compiler moc on header files
QT4_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS} )

# Put moc files in a project folder
#SOURCE_GROUP("UI Files" REGULAR_EXPRESSION "\\w*\\.ui")
#SOURCE_GROUP("Moc Files" REGULAR_EXPRESSION "moc_.*")

# Create the library
ADD_EXECUTABLE(FallenLondon-ItemConverter
	main.cpp
	${SOURCES}
	${HEADERS}
	${MOC_HEADERS}
	${MOC_SOURCES}
	${UIS}
)

TARGET_LINK_LIBRARIES( FallenLondon-ItemConverter
	${QT_LIBRARIES}
)

# Set build configuration types
# "RelWithDebInfo" and "MinSizeRelease" can be added here
set ( CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE TYPE INTERNAL FORCE )

# Set build directories
# Binaries are created in /bin and libraries in /lib
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib )

include_directories(${CMAKE_BINARY_DIR})

# Relative paths are used in the created projects
# May not work correcty due to the CMake docs
set( CMAKE_USE_RELATIVE_PATHS TRUE )