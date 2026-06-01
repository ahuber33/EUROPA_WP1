#----------------------------------------------------------------------------
# Setup the project
#----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16...3.21)
project(EUROPA_WP1)


#----------------------------------------------------------------------------
# Options
#----------------------------------------------------------------------------
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
option(GEANT4_USE_QT ON)
option(BUILD_TESTS "Build unit tests" ON)

#----------------------------------------------------------------------------
# Find Geant4 package
#----------------------------------------------------------------------------
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED gdml ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()


#----------------------------------------------------------------------------
# ROOT
#----------------------------------------------------------------------------
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Include directories
#----------------------------------------------------------------------------
include(${Geant4_USE_FILE})
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${Geant4_INCLUDE_DIR}
    ${ROOT_INCLUDE_DIRS}
)

#----------------------------------------------------------------------------
# Project sources and headers
#----------------------------------------------------------------------------
set(PROJECT_SRC
    src/EUROPA_WP1Physics.cc
    src/Geometry.cc
    src/EUROPA_WP1EventAction.cc
    src/EUROPA_WP1VisManager.cc
    src/EUROPA_WP1GeometryMessenger.cc
    src/EUROPA_WP1GeometryConstruction.cc
    src/EUROPA_WP1PrimaryGeneratorAction.cc
    src/EUROPA_WP1RunAction.cc
    src/EUROPA_WP1SteppingAction.cc
    src/EUROPA_WP1ActionInitialization.cc
)

set(PROJECT_HEADER
    include/EUROPA_WP1Physics.hh
    include/Geometry.hh
    include/EUROPA_WP1EventAction.hh
    include/EUROPA_WP1VisManager.hh
    include/EUROPA_WP1GeometryMessenger.hh
    include/EUROPA_WP1GeometryConstruction.hh
    include/EUROPA_WP1PrimaryGeneratorAction.hh
    include/EUROPA_WP1RunAction.hh
    include/EUROPA_WP1SteppingAction.hh
    include/EUROPA_WP1ActionInitialization.hh
)

#----------------------------------------------------------------------------
# Create the main library
#----------------------------------------------------------------------------
add_library(EUROPA_WP1Lib
    ${PROJECT_SRC}
    ${PROJECT_HEADER}
)

target_include_directories(EUROPA_WP1Lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${Geant4_INCLUDE_DIR}
    ${ROOT_INCLUDE_DIRS}
)

target_link_libraries(EUROPA_WP1Lib
    ${Geant4_LIBRARIES}
    ${ROOT_LIBRARIES}
)

#----------------------------------------------------------------------------
# Create the main executable
#----------------------------------------------------------------------------
add_executable(EUROPA_WP1 EUROPA_WP1.cc)
target_link_libraries(EUROPA_WP1 PRIVATE EUROPA_WP1Lib)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
install(TARGETS EUROPA_WP1 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

