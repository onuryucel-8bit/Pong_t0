cmake_minimum_required(VERSION 3.30)

project(Pong VERSION 1.0)

# kutuphanelerin dosya yolu
set(LIBS_LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add dependencies
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

include(FindLua)
find_package(lua CONFIG REQUIRED)

# header only
add_library(baku INTERFACE)
target_include_directories(baku INTERFACE ${LIBS_LOCAL_DIR}/baku)

add_subdirectory(${LIBS_LOCAL_DIR}/imgui-docking imgui_build)

#=====================================================================#
#=====================================================================#

# supurge teknigi ile proje dosyalari MY_SOURCES icerisine atiliyor
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${MY_SOURCES})

#====================LIBS=============================================#
#=====================================================================#


# Playground include dirs
target_include_directories(${PROJECT_NAME}  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LUA_LIBRARIES}
)

# Automatically links to these libs
target_link_libraries(${PROJECT_NAME}  PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    ${LUA_LIBRARIES}
    imgui
    baku
)

target_compile_definitions(${PROJECT_NAME}  PUBLIC cmake_PROJECT_RES="${CMAKE_CURRENT_SOURCE_DIR}/res/")
target_compile_definitions(${PROJECT_NAME}  PUBLIC cmake_PROJECT_SCRIPT="${CMAKE_CURRENT_SOURCE_DIR}/script/")
