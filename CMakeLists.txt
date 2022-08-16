cmake_minimum_required(VERSION 3.15)

if ($ENV{GITHUB_RUN_NUMBER})
    project (Matrix VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project (Matrix VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)

# option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
# message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")
# if(ENABLE_UNIT_TESTS)
#   # the following code to fetch googletest
#   # is inspired by and adapted after:
#   #   - https://cmake.org/cmake/help/v3.11/module/FetchContent.html
#   include(FetchContent)

#   FetchContent_Declare(
#     googletest
#     GIT_REPOSITORY https://github.com/google/googletest.git
#     GIT_TAG        release-1.11.0
#   )

#   FetchContent_GetProperties(googletest)

#   if(NOT googletest_POPULATED)
#     FetchContent_Populate(googletest)
#     # Prevent GoogleTest from overriding our compiler/linker options
#     # when building with Visual Studio
#     set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#     # Prevent GoogleTest from using PThreads
#     set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)

#     # adds the targers: gtest, gtest_main, gmock, gmock_main
#     add_subdirectory(
#       ${googletest_SOURCE_DIR}
#       ${googletest_BINARY_DIR}
#       )

#     # Silence std::tr1 warning on MSVC
#     if(MSVC)
#       foreach(_tgt gtest gtest_main gmock gmock_main)
#         target_compile_definitions(${_tgt}
#           PRIVATE
#             "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"
#           )
#       endforeach()
#     endif()
#   endif()
# endif()

file(GLOB matrix_file_cpp
 "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
)

add_executable(matrix_cli 
${matrix_file_cpp}
)

# add_executable(g_test_matrix 
# test/g_test_matrix.h 
# test/test.h
# test/test.cpp)

set_target_properties(matrix_cli 
# g_test_matrix
PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(matrix_cli
    PRIVATE 
    include
    "${CMAKE_BINARY_DIR}"
)
# target_link_libraries(g_test_matrix
#    gtest gtest_main
# )
if (MSVC)
    target_compile_options(matrix_cli 
    PRIVATE
    /EHsc /W4 /WX
        )    
    # target_compile_options(g_test_matrix 
    # PRIVATE
    # /EHsc /W4 /WX
    #     )   
 else ()
    target_compile_options(matrix_cli 
    PRIVATE
    -Wextra -pedantic -Werror
        )
    # target_compile_options(g_test_matrix 
    # PRIVATE
    # -Wextra -pedantic -Werror
    #     )
endif()

# enable_testing()
# add_test(test_version g_test_matrix)

# install(TARGETS matrix_cli RUNTIME DESTINATION bin)

# set(CPACK_GENERATOR DEB)
# set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
# set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
# set(CPACK_PACKAGE_CONTACT madim-neodart@rambler.ru)
# include(CPack)


