cmake_minimum_required(VERSION 3.3)
project(fcpp)

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 0)
set(${PROJECT_NAME}_VERSION_PATCH 0)

set(FC_AUTHOR "Frontcoder")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(LINKS
        )

set(LIBS
        )

set(FCPP_INCLUDES
        ${PROJECT_SOURCE_DIR}/ext/jsoncpp/include/
        ${PROJECT_SOURCE_DIR}/include/
        )
set(INCLUDES ${FCPP_INCLUDES})

set(SOURCES
        ${PROJECT_SOURCE_DIR}/ext/jsoncpp/src/jsoncpp.cpp
        ${PROJECT_SOURCE_DIR}/src/fcpp/util/Util.cpp
        ${PROJECT_SOURCE_DIR}/src/fcpp/http/HttpResponse.cpp
        ${PROJECT_SOURCE_DIR}/src/fcpp/http/HttpClient.cpp
        ${PROJECT_SOURCE_DIR}/src/fcpp/sockets/SocketClient.cpp
        )

find_package(CURL)
if(${CURL_FOUND})
        set(INCLUDES ${INCLUDES} ${CURL_INCLUDE_DIRS})
        set(LIBS ${LIBS} ${CURL_LIBRARIES})
else(${CURL_FOUND})
        message(FATAL_ERROR "curl library not installed in system ... sudo apt-get install libcurl-dev ?")
endif(${CURL_FOUND})

include_directories(${INCLUDES})
link_directories(${LINKS})

add_library(${PROJECT_NAME} SHARED ${FCPP_INCLUDES} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

add_library(${PROJECT_NAME}_static STATIC ${FCPP_INCLUDES} ${SOURCES})
target_link_libraries(${PROJECT_NAME}_static ${LIBS})
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# Tests
add_executable(${PROJECT_NAME}_test_twitch_irc ${FCPP_INCLUDES} ${PROJECT_SOURCE_DIR}/test/fcpp/sockets/twitch_irc.cpp)
target_link_libraries(${PROJECT_NAME}_test_twitch_irc ${PROJECT_NAME})

install(DIRECTORY ${FCPP_INCLUDES} DESTINATION include)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(TARGETS ${PROJECT_NAME}_static DESTINATION lib)
install(FILES ${PROJECT_NAME}-config.cmake DESTINATION lib/cmake/${PROJECT_NAME})

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VENDOR ${FC_AUTHOR})
set(CPACK_PACKAGE_CONTACT ${FC_AUTHOR})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${FC_AUTHOR})
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FrontCoder C++ Base Library")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
include(CPack)
