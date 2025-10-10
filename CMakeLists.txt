cmake_minimum_required(VERSION 3.30)

project(shooty_triangle)

add_executable(shooty_triangle
	src/main.c
	src/controller.c
	src/player.c
	src/c_map.c
	src/font_and_text.c
	src/ammo.c
	src/S_shapes.c
	src/enemy.c
	)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

target_link_libraries(shooty_triangle PRIVATE 
	SDL3::SDL3-static
	SDL3_ttf::SDL3_ttf-static
)

target_compile_options(shooty_triangle PRIVATE -O2 -Wall -Wextra)
