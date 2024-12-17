set(CMAKE_CXX_STANDARD 11)

add_executable(main main.cpp nearest_neighbor.cpp)
add_executable(test test.cpp nearest_neighbor.cpp)
add_executable(bf bf.cpp)

enable_testing()
add_test(NAME normal_test COMMAND test)
