cmake_minimum_required(VERSION 3.0)
project(httpd)

# Project configuration
add_subdirectory("modules/php-cgi")

if (WIN32)
else ()
    add_subdirectory("submodules/yaml")
endif ()

# Directory configuration
set(SOURCES_DIR "httpd/src")
set(APOUCHE_DIR "submodules/apouche")
set(UNIT_DIR "test/src")

if (WIN32)
    set(YAML_DIR "submodules/yaml")
    set(YAML_SOURCES
            ${YAML_DIR}/src/binary.cpp
            ${YAML_DIR}/src/convert.cpp
            ${YAML_DIR}/src/directives.cpp
            ${YAML_DIR}/src/emit.cpp
            ${YAML_DIR}/src/emitfromevents.cpp
            ${YAML_DIR}/src/emitter.cpp
            ${YAML_DIR}/src/emitterstate.cpp
            ${YAML_DIR}/src/emitterutils.cpp
            ${YAML_DIR}/src/exceptions.cpp
            ${YAML_DIR}/src/exp.cpp
            ${YAML_DIR}/src/memory.cpp
            ${YAML_DIR}/src/node.cpp
            ${YAML_DIR}/src/node_data.cpp
            ${YAML_DIR}/src/nodebuilder.cpp
            ${YAML_DIR}/src/nodeevents.cpp
            ${YAML_DIR}/src/null.cpp
            ${YAML_DIR}/src/ostream_wrapper.cpp
            ${YAML_DIR}/src/parse.cpp
            ${YAML_DIR}/src/parser.cpp
            ${YAML_DIR}/src/regex_yaml.cpp
            ${YAML_DIR}/src/scanner.cpp
            ${YAML_DIR}/src/scanscalar.cpp
            ${YAML_DIR}/src/scantag.cpp
            ${YAML_DIR}/src/scantoken.cpp
            ${YAML_DIR}/src/simplekey.cpp
            ${YAML_DIR}/src/singledocparser.cpp
            ${YAML_DIR}/src/stream.cpp
            ${YAML_DIR}/src/tag.cpp
            )
endif ()

# Headers configuration

include_directories(
        "submodules/apouche"
        "submodules/yaml/src"
        "submodules/yaml/include"
        "submodules/yaml/include/node"
        "submodules/yaml/include/contrib"
        "submodules/yaml/test/gmock-1.7.0/gtest/include"
        "httpd/include"
)

# Source configuration
set(APOUCHE_SOURCES
        ${APOUCHE_DIR}/HttpBody/HttpBody.cpp
        ${APOUCHE_DIR}/HttpHeader/HttpHeader.cpp
        ${APOUCHE_DIR}/HttpMessage/HttpMessage.cpp
        ${APOUCHE_DIR}/HttpRequest/HttpRequest.cpp
        ${APOUCHE_DIR}/HttpResponse/HttpResponse.cpp
        ${APOUCHE_DIR}/Logger/Logger.cpp
        ${APOUCHE_DIR}/Logger/FileLogger.cpp
        )

set(HTTPD_SOURCES
        ${SOURCES_DIR}/api/FileLogger.cpp
        ${SOURCES_DIR}/api/HttpConf.cpp
        ${SOURCES_DIR}/api/HttpRequest.cpp
        ${SOURCES_DIR}/api/HttpResponse.cpp
        ${SOURCES_DIR}/api/Logger.cpp
        ${SOURCES_DIR}/exception/custom_exception.cpp
        ${SOURCES_DIR}/exception/socket_exception.cpp
        ${SOURCES_DIR}/exception/thread_exception.cpp
        ${SOURCES_DIR}/httpEngine/Engine.cpp
        ${SOURCES_DIR}/httpEngine/Grammar.cpp
        ${SOURCES_DIR}/httpEngine/Methods/Get.cpp
        ${SOURCES_DIR}/httpEngine/Methods/Head.cpp
        ${SOURCES_DIR}/httpEngine/Request.cpp
        ${SOURCES_DIR}/httpEngine/Response.cpp
        ${SOURCES_DIR}/io/ioReader.cpp
        ${SOURCES_DIR}/io/ioWriter.cpp
        ${SOURCES_DIR}/parser/YamlParser.cpp
        ${SOURCES_DIR}/threadPool/ThreadDelegator.cpp
        ${SOURCES_DIR}/threadPool/ThreadGuard.cpp
        ${SOURCES_DIR}/utils/Converter.cpp
        ${SOURCES_DIR}/utils/FolderView.cpp
        ${SOURCES_DIR}/utils/RequestExtractor.cpp
        ${SOURCES_DIR}/utils/Splitter.cpp
        ${SOURCES_DIR}/utils/String.cpp
        ${SOURCES_DIR}/utils/Trimmer.cpp
        ${SOURCES_DIR}/utils/Uri.cpp
        ${SOURCES_DIR}/worker/Worker.cpp
        )

set(HTTPD_WIN_SOURCES
        ${SOURCES_DIR}/network/SocketWindows.cpp
        ${SOURCES_DIR}/parser/ModuleLoaderWindows.cpp
        ${SOURCES_DIR}/server/HttpServerWindows.cpp
        )

set(HTTPD_UNIX_SOURCES
        ${SOURCES_DIR}/network/SocketUnix.cpp
        ${SOURCES_DIR}/parser/ModuleLoaderUnix.cpp
        ${SOURCES_DIR}/server/HttpServerUnix.cpp
        )

if (WIN32)
    set(HTTPD_SOURCES ${HTTPD_SOURCES} ${HTTPD_WIN_SOURCES} ${YAML_SOURCES})
else ()
    set(HTTPD_SOURCES ${HTTPD_SOURCES} ${HTTPD_UNIX_SOURCES})
endif ()

set(HTTPD_UNIT
        ${UNIT_DIR}/main.cpp
        ${UNIT_DIR}/cache/LRUCache.cpp
        ${UNIT_DIR}/exception/StackTrace.cpp
        ${UNIT_DIR}/engine/Engine.cpp
        ${UNIT_DIR}/io/ioReader.cpp
        ${UNIT_DIR}/threadpool/TaskQueue.cpp
        ${UNIT_DIR}/threadpool/ThreadDelegator.cpp
        ${UNIT_DIR}/threadpool/ThreadGuard.cpp
        ${UNIT_DIR}/utils/RequestsSplitter.cpp
        )

set(HTTPD_SERVER ${SOURCES_DIR}/httpd.cpp)
set(SOURCES ${HTTPD_SOURCES} ${APOUCHE_SOURCES})

# Executable
set(CMAKE_CXX_STANDARD 14)

if (WIN32)
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()

add_executable(zia ${SOURCES} ${HTTPD_SERVER})

if (WIN32)
else ()
    target_link_libraries(zia yaml-cpp)
endif ()

add_executable(unitary_test ${SOURCES} ${HTTPD_UNIT})
target_link_libraries(unitary_test gtest yaml-cpp)

# Cross-Platform library
if (WIN32)
    target_link_libraries(zia ws2_32.lib)
else ()
    target_link_libraries(zia pthread dl)
    target_link_libraries(unitary_test pthread dl)
endif ()
