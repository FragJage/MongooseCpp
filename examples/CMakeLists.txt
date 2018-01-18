#Activer le support c++11 
if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
else ()
  set(CMAKE_CXX_STANDARD 11)
endif ()
 
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
  src_files
  ../src/*
)

add_definitions( -DMG_ENABLE_CALLBACK_USERDATA )
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

#Déclaration des l'exécutables
add_executable(
  example1
  ${Mongoose_files}
  ${src_files}
  example1.cpp
)

add_executable(
  example2
  ${Mongoose_files}
  ${src_files}
  example2.cpp
)

add_executable(
  example3
  ${Mongoose_files}
  ${src_files}
  example3.cpp
)

#Spécification WIN32
if(WIN32)
  target_link_libraries(example1 ws2_32)
  target_link_libraries(example2 ws2_32)
  target_link_libraries(example3 ws2_32)
endif()