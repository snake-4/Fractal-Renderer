cmake_minimum_required(VERSION 3.12)

project("FractalGraphics")

file(GLOB FractalGraphics_SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
add_executable(FractalGraphics ${FractalGraphics_SRC})

set_property(TARGET FractalGraphics PROPERTY CXX_STANDARD 17) #11

#find_package(glfw3 CONFIG REQUIRED)
#target_link_libraries(FractalGraphics PRIVATE glfw)

#find_package(OpenGL REQUIRED)
#target_link_libraries(FractalGraphics PRIVATE OpenGL::GL)

#find_package(lodepng CONFIG REQUIRED)
#target_link_libraries(FractalGraphics PRIVATE lodepng)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(FractalGraphics PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
