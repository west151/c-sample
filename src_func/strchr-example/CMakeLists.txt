cmake_minimum_required(VERSION 3.16)

project(strchr-example LANGUAGES C)

add_executable(strchr-example main.c )

include(GNUInstallDirs)
install(TARGETS strchr-example
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
