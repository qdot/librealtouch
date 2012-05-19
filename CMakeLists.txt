######################################################################################
# CMake directives
######################################################################################

#Require 2.6 or higher.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

INCLUDE( ${CMAKE_SOURCE_DIR}/BuildSysCMakeLib.cmake )

#setting link directory policy

IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######################################################################################
# Project declaration and options
######################################################################################

#Project declaration

PROJECT(LIBREALTOUCH)
INITIALIZE_BUILD()

#Common Options

OPTION_LIBRARY_BUILD_STATIC(ON)
OPTION_LIBRARY_BUILD_SHARED(OFF)

#Project specific options
OPTION(BUILD_EXAMPLES "Build librealtouch examples" ON)

######################################################################################
# Project specific package finding
######################################################################################

FIND_PACKAGE(HIDAPI)
# FIND_PACKAGE(argtable)

######################################################################################
# Project specific globals
######################################################################################

#library definitions

SET(LIBREALTOUCH_MAJOR_VERSION 1)
SET(LIBREALTOUCH_MINOR_VERSION 0)
SET(LIBREALTOUCH_BUILD_VERSION 0)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${HIDAPI_INCLUDE_DIR})

SET(LIBREALTOUCH_VERSION ${LIBREALTOUCH_MAJOR_VERSION}.${LIBREALTOUCH_MINOR_VERSION}.${LIBREALTOUCH_BUILD_VERSION})

#library name definitions

CREATE_LIBRARY_LINK_NAME(realtouch)

INCLUDE_DIRECTORIES(${LIBREALTOUCH_INCLUDE_DIR})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

LIST(APPEND LIBREALTOUCH_DEP_LIBS ${HIDAPI_LIBRARIES})

# #If we build libusb staticly on apple, we need the proper frameworks
IF(BUILD_STATIC AND APPLE)
  LIST(APPEND LIBREALTOUCH_DEP_LIBS "-framework IOKit" "-framework CoreFoundation")
ENDIF(BUILD_STATIC AND APPLE)

IF(UNIX AND NOT APPLE)
	SET(CMAKE_INSTALL_RPATH "/usr/lib:/home/qdot/usr/lib:${CMAKE_BINARY_DIR}/lib")
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
	LIST(APPEND LIBREALTOUCH_DEP_LIBS "udev")
ENDIF()

# ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
# ADD_SUBDIRECTORY(examples)
