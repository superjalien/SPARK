# ############################################# #
#                                               #
#         SPARK Particle Engine : Demos         #
#                 Solution file                 #
#                                               #
# ############################################# #

PROJECT(SPARK)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/")

find_package(OpenGL)
find_package(SDL)


add_subdirectory(projects/engine)

if(SDL_FOUND)
add_subdirectory(projects/demos)
endif(SDL_FOUND)
