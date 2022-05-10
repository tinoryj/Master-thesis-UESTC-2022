cmake_minimum_required(VERSION 3.1)
if (NOT BUILD_AARCH)
   set (BUILD_AARCH "AARCH64")
   message (
      STATUS "No BUILD_AARCH selected, defaulting to ${BUILD_AARCH}"
   )
endif ()

if(BUILD_AARCH STREQUAL "AARCH32")
   message(STATUS "Build for ARMv7-A aarch32")
   # Librarys for qemu v7 aarch32 
   set(BOOST_INCLUDE_DIRS /home/tinoryj/ARM/librarys/boost-32/include)
   set(BOOST_LIBRARY_DIR /home/tinoryj/ARM/librarys/boost-32/lib)
   set(OPENSSL_INCLUDE_DIRS /home/tinoryj/ARM/librarys/openssl-32/include)
   set(OPENSSL_LIBRARY_DIR /home/tinoryj/ARM/librarys/openssl-32/lib)
   # Compilers for qemu v7 aarch32 
   # SET (CMAKE_C_COMPILER               "/home/tinoryj/ARM/librarys/toolchains/aarch32/bin/arm-linux-gnueabihf-gcc")
   # SET (CMAKE_CXX_COMPILER             "/home/tinoryj/ARM/librarys/toolchains/aarch32/bin/arm-linux-gnueabihf-g++")
   # SET (CMAKE_AR                       "/home/tinoryj/ARM/librarys/toolchains/aarch32/bin/arm-linux-gnueabihf-ar")
   # SET (CMAKE_LINKER                   "/home/tinoryj/ARM/librarys/toolchains/aarch32/bin/arm-linux-gnueabihf-ld")
   # SET (CMAKE_NM                       "/home/tinoryj/ARM/librarys/toolchains/aarch32/bin/arm-linux-gnueabihf-nm")
   # SET (CMAKE_OBJDUMP                  "/home/tinoryj/ARM/librarys/toolchains/aarch32/bin/arm-linux-gnueabihf-objdump")
   # SET (CMAKE_RANLIB                   "/home/tinoryj/ARM/librarys/toolchains/aarch32/bin/arm-linux-gnueabihf-ranlib")
else()
   message(STATUS "Build for ARMv8-A aarch64")
   # Librarys for qemu v8 aarch64 
   set(BOOST_INCLUDE_DIRS /home/tinoryj/ARM/librarys/boost-64/include)
   set(BOOST_LIBRARY_DIR /home/tinoryj/ARM/librarys/boost-64/lib)
   set(OPENSSL_INCLUDE_DIRS /home/tinoryj/ARM/librarys/openssl-64/include)
   set(OPENSSL_LIBRARY_DIR /home/tinoryj/ARM/librarys/openssl-64/lib)
   # Compilers for qemu v8 aarch64 
   # SET (CMAKE_C_COMPILER               "/home/tinoryj/ARM/librarys/toolchains/aarch64/bin/aarch64-linux-gnu-gcc")
   # SET (CMAKE_CXX_COMPILER             "/home/tinoryj/ARM/librarys/toolchains/aarch64/bin/aarch64-linux-gnu-g++")
   # SET (CMAKE_AR                       "/home/tinoryj/ARM/librarys/toolchains/aarch64/bin/aarch64-linux-gnu-ar")
   # SET (CMAKE_LINKER                   "/home/tinoryj/ARM/librarys/toolchains/aarch64/bin/aarch64-linux-gnu-ld")
   # SET (CMAKE_NM                       "/home/tinoryj/ARM/librarys/toolchains/aarch64/bin/aarch64-linux-gnu-nm")
   # SET (CMAKE_OBJDUMP                  "/home/tinoryj/ARM/librarys/toolchains/aarch64/bin/aarch64-linux-gnu-objdump")
   # SET (CMAKE_RANLIB                   "/home/tinoryj/ARM/librarys/toolchains/aarch64/bin/aarch64-linux-gnu-ranlib")
endif(BUILD_AARCH STREQUAL "AARCH32")

set (CMAKE_SYSTEM_PROCESSOR arm )
set (CMAKE_SYSTEM_NAME Linux )
if (NOT CMAKE_BUILD_TYPE)
   set (CMAKE_BUILD_TYPE "Release")
   message (
      STATUS "No CMAKE_BUILD_TYPE selected, defaulting to ${CMAKE_BUILD_TYPE}"
   )
endif ()

set(OPENSSL_USE_STATIC_LIBS ON)
set(BOOST_USE_STATIC_LIBS ON)

# set(CMAKE_C_FLAGS "-static ${CMAKE_C_FLAGS}")
# set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-Wall -std=c1x -g -ggdb -g2") 
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -ggdb -g2 -fno-omit-frame-pointer -fsanitize=leak")
set(CMAKE_C_FLAGS_RELEASE "-O3") 
set(CMAKE_CXX_FLAGS_RELEASE "-O3")



project(TEEDEDUP)

add_subdirectory(src)