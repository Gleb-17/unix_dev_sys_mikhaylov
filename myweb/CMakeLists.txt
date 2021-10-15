cmake_minimum_required(VERSION 3.0)
project(myweb)
set(SOURCE_EXE myweb.c)
add_executable(${PROJECT_NAME} ${SOURCE_EXE})
add_definitions(-Wall)
#real path will be /usr/local/bin if you choose "make install" option
install(TARGETS ${PROJECT_NAME} DESTINATION bin)


#create with debug: cmake -DCMAKE_BUILD_TYPE=Debug <path>
