cmake_minimum_required(VERSION 3.16)

project(strcpy-example LANGUAGES C)

add_executable(strcpy-example main.c )

include(GNUInstallDirs)
install(TARGETS strcpy-example
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
