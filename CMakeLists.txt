######## A simple cmakelists.txt file for OpenCV() #############
cmake_minimum_required(VERSION 3.2)
PROJECT(NPDFace)
#FIND_PACKAGE(OpenCV QUIET REQUIRED )
find_package( OpenCV REQUIRED )
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -pg")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -std=c++11 -pg")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable -pthread")
ADD_EXECUTABLE(NPDFace Configuration.h
DQTree.cpp
DQTree.h
main.cpp
Message.h
Model.cpp
Model.h
NPDFeature.cpp
NPDFeature.h
)
TARGET_LINK_LIBRARIES (NPDFace ${OpenCV_LIBS} ${OpenCL_LIBS})
# ########## end ####################################
