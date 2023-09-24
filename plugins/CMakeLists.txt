cmake_minimum_required(VERSION 3.4)

if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DSPLAB_PLUGINS_PATH})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DSPLAB_PLUGINS_PATH})
endif (WIN32)

if (UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${DSPLAB_PLUGINS_PATH})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${DSPLAB_PLUGINS_PATH})
endif (UNIX)

add_subdirectory(awgn)
add_subdirectory(buffer)
add_subdirectory(gain)
add_subdirectory(hilbert-transform)
add_subdirectory(imag-transform)
add_subdirectory(real-transform)
add_subdirectory(summator)

add_subdirectory(sinus-generator)
add_subdirectory(noise-generator)

add_subdirectory(udp-receiver)
add_subdirectory(udp-sender)

add_subdirectory(papr-meter)
add_subdirectory(sinad-meter)

add_subdirectory(spectrum-analyzer)
add_subdirectory(hex-viewer)

add_subdirectory(cast-from-raw)
add_subdirectory(cast-to-raw)
