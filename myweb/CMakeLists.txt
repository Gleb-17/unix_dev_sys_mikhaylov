cmake_minimum_required(VERSION 3.0)
project(myweb)

#create variables
set(SOURCE_DIR src)
set(SOURCE_WEB_DIR webroot)
set(LOG_DIR /var/log/myweb)
set(WEB_DIR /srv/myweb)
set(XINETD /etc/xinetd.d)
set(WEB_FILES webroot/index.html webroot/page1.html)
set(WRAP_FILES ${SOURCE_DIR}/myweb-wrap)


#target_sources(SOURCES PRIVATE ${SOURCE_DIR}/myweb.c ${SOURCE_DIR}/journal.c)
file(GLOB SOURCES "${SOURCE_DIR}/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
add_definitions(-Wall)

install(DIRECTORY DESTINATION ${LOG_DIR})
install(DIRECTORY DESTINATION ${WEB_DIR})
install(FILES ${WEB_FILES} DESTINATION ${WEB_DIR})
install(PROGRAMS ${WRAP_FILES} DESTINATION bin)
install(FILES myweb-xinetd DESTINATION ${XINETD})

#real path will be /usr/local/bin if you choose "make install" option
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

#create with debug: cmake -DCMAKE_BUILD_TYPE=Debug <path>
