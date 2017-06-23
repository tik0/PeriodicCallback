cmake_minimum_required (VERSION 2.6)

SET (PROJECT_NAME "periodicCallbackExample")
MESSAGE (STATUS "Set PROJECT_NAME to ${PROJECT_NAME}")
PROJECT ("${PROJECT_NAME}")


# Define the source
ADD_EXECUTABLE ("${PROJECT_NAME}" example.cxx)

TARGET_LINK_LIBRARIES ("${PROJECT_NAME}" pthread)

# Additional compiler flags
SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
SET ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -g" )
SET ( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2" )


