# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

#set(CMAKE_CXX_STANDARD 11)
set(IPERF_SRC ${CMAKE_SOURCE_DIR}/iperf3)


# check some architecture specific build options
if ("${ANDROID_ABI}" MATCHES "arm64-v8a|x86_64")
    set(HAVE_SO_MAX_PACING_RATE 1)
endif ()

# generate src/iperf_config.h
configure_file(${CMAKE_SOURCE_DIR}/../assets/iperf_config.h.in ${CMAKE_BINARY_DIR}/src/iperf_config.h)

###################################

include_directories(${IPERF_SRC}/src)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # build libiperf.so
        iperf
        STATIC

        ${IPERF_SRC}/src/cjson.c
        ${IPERF_SRC}/src/iperf_api.c
        ${IPERF_SRC}/src/iperf_error.c
        ${IPERF_SRC}/src/iperf_client_api.c
        ${IPERF_SRC}/src/iperf_locale.c
        ${IPERF_SRC}/src/iperf_server_api.c
        ${IPERF_SRC}/src/iperf_tcp.c
        ${IPERF_SRC}/src/iperf_udp.c
        ${IPERF_SRC}/src/iperf_sctp.c
        ${IPERF_SRC}/src/iperf_util.c
        ${IPERF_SRC}/src/net.c
        ${IPERF_SRC}/src/tcp_info.c
        ${IPERF_SRC}/src/tcp_window_size.c
        ${IPERF_SRC}/src/timer.c
        ${IPERF_SRC}/src/units.c
        )

add_library(
        iperf-jni SHARED

        iperf-jni-wrap.cpp iperf-jni.cpp
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries(iperf-jni
        iperf

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
