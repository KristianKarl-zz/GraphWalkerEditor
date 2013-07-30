cmake_minimum_required (VERSION 2.8.3)
project (gwe)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/kde4/apps/cmake/modules/")

# use: cmake -DCMAKE_BUILD_TYPE=Debug
#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

find_package(Qt4 4.8.4 REQUIRED)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  find_package(Boost 1.53 COMPONENTS program_options unit_test_framework REQUIRED)
else()
  find_package(Boost 1.53 COMPONENTS program_options REQUIRED)
endif()

# QT stuff
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
  if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")
  else()
    set(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -Wall")
  endif()
endif()

FILE(GLOB_RECURSE SOURCE_FILES src/main/c++/*.cpp)
FILE(GLOB_RECURSE MOC_HDR_FILES src/main/c++/gui/*.h)
FILE(GLOB_RECURSE TEST_SOURCE_FILES src/test/c++/*.cpp)

include_directories( ..  src src/main/c++/gui, src/main/c++/graphwalker , src/main/c++/ogdf )
set(GWE_RESOURCES src/main/resources/gwe.qrc)

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(GWE_RESOURCES_RCC ${GWE_RESOURCES})

# After this call, MOC_SOURCE_FILES = moc_Class1.cxx moc_Class2.cxx moc_Class3.cxx.
QT4_WRAP_CPP( MOC_SOURCE_FILES ${MOC_HDR_FILES} )

# Set Open Graph Drawing Framework libaries
set(OGDF_LIBRARY -lOGDF -lCOIN)

add_executable( gwe
                ${SOURCE_FILES}
                ${MOC_SOURCE_FILES}
                ${GWE_RESOURCES_RCC} )

target_link_libraries( gwe
                      ${OGDF_LIBRARY}
                      ${QT_LIBRARIES}
                      ${Boost_LIBRARIES} )


ENABLE_TESTING()
add_executable( gwe_unit_test
                ${TEST_SOURCE_FILES} )
target_link_libraries( gwe_unit_test
                      ${OGDF_LIBRARY}
                      ${QT_LIBRARIES}
                      ${Boost_LIBRARIES} )
ADD_TEST( NAME gwe_unit_tests
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
          COMMAND ${CMAKE_BINARY_DIR}/gwe_unit_test )
