cmake_minimum_required(VERSION 3.16)

project(strstr-example LANGUAGES C)

add_executable(strstr-example main.c )

include(GNUInstallDirs)
install(TARGETS strstr-example
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
