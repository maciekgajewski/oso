cmake_minimum_required(VERSION 3.10)

find_package(SDL2 REQUIRED)

add_executable(oso
	main.cc
)

target_include_directories(oso SYSTEM PRIVATE ${SDL2_INCLUDE_DIR})

target_link_libraries(oso ${SDL2_LIBRARY})

