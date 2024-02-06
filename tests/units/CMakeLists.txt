include(${CMAKE_CURRENT_SOURCE_DIR}/source_list.cmake)
add_executable(test_units ${SOURCE_FILES})
target_add_catch2(test_units)

add_executable(test_units_unity EXCLUDE_FROM_ALL unity_source.cpp)
target_link_libraries(test_units_unity PRIVATE Catch2::Catch2WithMain
                                               ${PROJECT_NAME})
target_include_directories(test_units_unity
                           PRIVATE ${${UPPER_PROJECT_NAME}_TEST_INCLUDE_DIR})
