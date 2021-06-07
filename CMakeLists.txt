cmake_minimum_required(VERSION 3.16)
project(continuous_integration)

#set( CMAKE_INCLUDE_PATH "/usr/include")
#set( CMAKE_LIBRARY_PATH "/usr/lib/x86_64-linux-gnu/libncursesw.so")
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
set(CMAKE_CXX_STANDARD 20)

add_executable(continuous_integration main.cpp objects/Repository.cpp objects/Repository.h objects/Agent.cpp objects/Agent.h objects/Dispatcher.cpp objects/Dispatcher.h objects/Task.cpp objects/Task.h objects/Environment.cpp objects/Environment.h)

# Look for the package you want to link
find_package( Curses REQUIRED )
find_package( Threads )

# Include the directories of the package (to find curses.h for instance)
target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS} )

# Link the library
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES} )
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} )