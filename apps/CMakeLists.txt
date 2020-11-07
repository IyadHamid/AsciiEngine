cmake_minimum_required(VERSION 3.8)
project(grapher VERSION 0.0.1 LANGUAGES CXX)

if (NOT TARGET AsciiEngine::AsciiEngine)
	find_package(AsciiEngine REQUIRED)
endif()

#Simple Grapher
add_executable(grapher src/grapher.cpp)
target_compile_features(grapher PRIVATE cxx_std_17)
target_link_libraries(grapher AsciiEngine::AsciiEngine)

#Obj Viewer (Currently not working)
#add_executable(objviewer src/objviewer.cpp)
#target_compile_features(objviewer PRIVATE cxx_std_17)
#target_link_libraries(objviewer AsciiEngine::AsciiEngine)
