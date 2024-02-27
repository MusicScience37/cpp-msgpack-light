include(${CMAKE_CURRENT_SOURCE_DIR}/source_list.cmake)
add_executable(test_integ ${SOURCE_FILES})
target_add_catch2(test_integ)
target_link_libraries(test_integ PRIVATE msgpack-cxx)
target_add_ausan(test_integ)

add_executable(test_integ_unity EXCLUDE_FROM_ALL unity_source.cpp)
target_link_libraries(test_integ_unity PRIVATE Catch2::Catch2WithMain
                                               ${PROJECT_NAME} msgpack-cxx)
target_include_directories(test_integ_unity
                           PRIVATE ${${UPPER_PROJECT_NAME}_TEST_INCLUDE_DIR})
