cmake_minimum_required(VERSION 3.16)

project(c-sample LANGUAGES C)

add_executable(c-sample main.c
    bitmap.h)

include(GNUInstallDirs)
install(TARGETS c-sample
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
