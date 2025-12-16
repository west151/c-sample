cmake_minimum_required(VERSION 3.16)

project(atoi-sample LANGUAGES C)

add_executable(atoi-sample main.c )

include(GNUInstallDirs)
install(TARGETS atoi-sample
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
