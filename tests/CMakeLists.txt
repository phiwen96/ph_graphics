# add_subdirectory (tests)

# set (test_name "test_${CMAKE_PROJECT_NAME}")
# $<TARGET_FILE:app2>

add_test (NAME test_0 COMMAND app)
add_test (NAME test_1 COMMAND app)

# add_test (NAME test_1 COMMAND app $<TARGET_FILE:app>)



set_tests_properties (test_0 PROPERTIES TIMEOUT 10)
set_tests_properties(test_1 PROPERTIES TIMEOUT 10)