cmake_minimum_required(VERSION 3.16)

project(strtok-example LANGUAGES C)

add_executable(strtok-example main.c )

include(GNUInstallDirs)
install(TARGETS strtok-example
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
