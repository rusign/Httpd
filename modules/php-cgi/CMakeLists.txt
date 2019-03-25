cmake_minimum_required(VERSION 3.6)
project(php-cgi)

set(CMAKE_CXX_STANDARD 14)
set(APOUCHE_DIR ../../submodules/apouche)

include_directories(${APOUCHE_DIR})

set(SOURCE_FILES
        phpcgiModule.cpp
        utils/Splitter.cpp
        utils/Trimmer.cpp
        utils/Uri.cpp
        )

add_library(php-cgi SHARED ${SOURCE_FILES} ${APOUCHE_SOURCES})
