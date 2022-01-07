cmake_minimum_required(VERSION 3.0.0)
project(example VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(src)

if(MSVC)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
endif(MSVC)


link_libraries( elog)

aux_source_directory(. src)


add_executable(example ${src})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
