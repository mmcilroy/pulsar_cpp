add_executable( one_to_one_performance_test one_to_one_performance_test.cpp )
target_link_libraries( one_to_one_performance_test pthread )

add_executable( one_to_three_performance_test one_to_three_performance_test.cpp )
target_link_libraries( one_to_three_performance_test pthread )

add_executable( one_to_three_sequenced_performance_test one_to_three_sequenced_performance_test.cpp )
target_link_libraries( one_to_three_sequenced_performance_test pthread )

add_executable( three_to_one_performance_test three_to_one_performance_test.cpp )
target_link_libraries( three_to_one_performance_test pthread )

add_executable( functional_test functional_test.cpp )
target_link_libraries( functional_test pthread )
