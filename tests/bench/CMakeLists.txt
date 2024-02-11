option(${UPPER_PROJECT_NAME}_TEST_BENCHMARKS "execute benchmarks in tests" OFF)

set(${UPPER_PROJECT_NAME}_BENCH_DIR
    "${CMAKE_BINARY_DIR}/bench"
    CACHE PATH "directory to which benchmark results are written" FORCE)
file(MAKE_DIRECTORY ${${UPPER_PROJECT_NAME}_BENCH_DIR})

include(${CMAKE_CURRENT_SOURCE_DIR}/source_list.cmake)
add_executable(bench ${SOURCE_FILES})
target_link_libraries(bench PRIVATE celero ${PROJECT_NAME} msgpack-cxx)

add_executable(bench_unity EXCLUDE_FROM_ALL unity_source.cpp)
target_link_libraries(bench_unity PRIVATE celero ${PROJECT_NAME} msgpack-cxx)
target_include_directories(bench_unity
                           PRIVATE ${${UPPER_PROJECT_NAME}_TEST_INCLUDE_DIR})
if(${UPPER_PROJECT_NAME}_TEST_BENCHMARKS)
    add_test(
        NAME ${TARGET}
        COMMAND ${TARGET} -t result.csv
        WORKING_DIRECTORY ${${UPPER_PROJECT_NAME}_BENCH_DIR})
endif()
