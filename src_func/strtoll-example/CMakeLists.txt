cmake_minimum_required(VERSION 3.16)

project(strtoll-example LANGUAGES C)

add_executable(strtoll-example main.c )

include(GNUInstallDirs)
install(TARGETS strtoll-example
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
