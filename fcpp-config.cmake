set(INCLUDES "${CMAKE_CURRENT_LIST_DIR}/../../include")
set(LIBS "${CMAKE_CURRENT_LIST_DIR}/../../libfcpp.so")

get_filename_component(fcpp_INCLUDE_DIRS ${INCLUDES} ABSOLUTE)
get_filename_component(fcpp_LIBRARIES ${LIBS} ABSOLUTE)

find_package(CURL)
if(${CURL_FOUND})
        set(fcpp_INCLUDE_DIRS ${fcpp_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})
        set(fcpp_LIBRARIES ${fcpp_LIBRARIES} ${CURL_LIBRARIES})
else(${CURL_FOUND})
        message(FATAL_ERROR "curl library not installed in system ... sudo apt-get install libcurl-dev ?")
endif(${CURL_FOUND})
