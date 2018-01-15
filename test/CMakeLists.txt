#Activer le support c++11 
if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
else ()
  set(CMAKE_CXX_STANDARD 11)
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual") # various warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage
 
include_directories(../dependency)
include_directories(../src)
 
#Génération de la liste des fichiers sources
file(
  GLOB_RECURSE
  Mongoose_files
  ../dependency/mongoose/*
)

file(
  GLOB_RECURSE
  UnitTest_files
  ../dependency/UnitTest/*
)

file(
  GLOB_RECURSE
  src_files
  ../src/*
)

add_definitions( -DMG_ENABLE_CALLBACK_USERDATA )

#Déclaration de l'exécutable
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
add_executable(
  unittest
  ${Mongoose_files}
  ${UnitTest_files}
  ${src_files}
  main.cpp
  HttpClient.cpp
  HttpHelper.cpp
  TestWebServer.cpp
  TestBaseController.cpp
  TestPageController.cpp
  TestApiController.cpp
  TestTypeController.cpp
)

#Spécification WIN32
if(WIN32)
  target_link_libraries(unittest ws2_32)
endif()