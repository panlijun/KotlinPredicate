# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.4.1)
project("insportapp")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
if(DEFINED ANDROID_NDK_MAJOR AND ${ANDROID_NDK_MAJOR} GREATER 20)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-openmp")
endif()

# mnn 库
include_directories(${CMAKE_SOURCE_DIR}/mnn/include/)

add_library( MNN SHARED IMPORTED GLOBAL )
set_target_properties(
        MNN
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/mnn/${ANDROID_ABI}/libMNN.so)

add_library( MNN_CL SHARED IMPORTED GLOBAL )
set_target_properties( MNN_CL
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/mnn/${ANDROID_ABI}/libMNN_CL.so)

#add_library( MNN_Vulkan SHARED IMPORTED GLOBAL )
#set_target_properties( MNN_Vulkan
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/mnn/${ANDROID_ABI}/libMNN_Vulkan.so)

add_library( MNN_Express SHARED IMPORTED GLOBAL )
set_target_properties( MNN_Express
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/mnn/${ANDROID_ABI}/libMNN_Express.so)

# huawei npu 支持 so
#include_directories(${CMAKE_SOURCE_DIR}/hiai/include/)
#
#add_library( libcpucl SHARED IMPORTED GLOBAL )
#set_target_properties(
#        libcpucl
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/hiai/${ANDROID_ABI}/libcpucl.so)
#
#add_library( libhcl SHARED IMPORTED GLOBAL )
#set_target_properties(
#        libhcl
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/hiai/${ANDROID_ABI}/libhcl.so)
#
#add_library( HIAI_IR_BUILD SHARED IMPORTED GLOBAL )
#set_target_properties( HIAI_IR_BUILD
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/hiai/${ANDROID_ABI}/libhiai_ir_build.so)
#
#add_library( HIAI_IR SHARED IMPORTED GLOBAL )
#set_target_properties( HIAI_IR
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/hiai/${ANDROID_ABI}/libhiai_ir.so)
#
#add_library( HIAI SHARED IMPORTED GLOBAL )
#set_target_properties( HIAI
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/hiai/${ANDROID_ABI}/libhiai.so)


# 本地源代码
aux_source_directory(. SRC_LIST)
add_library(insportapp SHARED ${SRC_LIST})

# 其他库
find_library(log-lib log)
find_library( jnigraphics-lib jnigraphics)
find_library(android-lib android)

add_definitions(-DMNN_USE_LOGCAT)

target_link_libraries(
        insportapp
        ${log-lib}
        ${android-lib}
        ${jnigraphics-lib}
        MNN
        MNN_CL
#        MNN_Vulkan
        MNN_Express
#        libcpucl
#        libhcl
#        HIAI_IR_BUILD
#        HIAI_IR
#        HIAI
)
