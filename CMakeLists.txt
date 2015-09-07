cmake_minimum_required(VERSION 2.8)

project(PyRenderer)

set(CMAKE_CXX_STANDARD 11)

set(PYTHON_LIBRARY C:/Python34/libs/python34_d.lib)
set(PYTHON_INCLUDE_DIR C:/Python34/include)

find_package(SDL2)
find_package(OpenGL)
find_package(GLEW)
#find_package(PythonLibs 2.7)

if (WIN32)
	add_definitions(-DGLEW_STATIC)
	set(GLM_DIR $ENV{SDK}/glm)
else (WIN32)
	set(GLM_DIR /usr/include/glm)
endif (WIN32)

set(GL_LIBS ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
set(SDL2_LIBS ${SDL2_LIBRARY})

set(GL_INC ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
set(SDL2_INC ${SDL2_INCLUDE_DIR})

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)
file(GLOB SHADERS Shaders/*)
file(GLOB SCRIPTS Scripts/*)

include_directories(${GLM_DIR} ${GL_INC} ${SDL2_INC}  ${PYTHON_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../PyLiaison/include include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../PyLiaison ${CMAKE_CURRENT_BINARY_DIR}/PyLiaison)

add_executable(PyRenderer ${SOURCES} ${HEADERS} ${SHADERS} ${SCRIPTS})
target_link_libraries(PyRenderer ${SDL2_LIBS} ${GL_LIBS} ${PYTHON_LIBRARY} pyliason)
