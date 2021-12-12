cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
set (PROJECT_NAME print_ip VERSION ${PROJECT_VESRION}) 

project(${PROJECT_NAME})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    message("### DOXYGEN_FOUND")
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    message("### IN "${DOXYGEN_IN})
    message("### OUT "${DOXYGEN_OUT})

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

configure_file(src/version/version.h.in version.h)

set(SOURCE_FILES 
    src/main.cpp     
)

add_executable(print_ip ${SOURCE_FILES})
add_library(print_ip_lib src/version/versionLib.cpp)
add_executable(test_Version src/version/test_Version.cpp)

set_target_properties(print_ip print_ip_lib test_Version PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(print_ip_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)
set_target_properties(test_Version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(print_ip PRIVATE
    print_ip_lib
)
target_link_libraries(test_Version
    ${Boost_LIBRARIES}
    print_ip_lib
)

if (MSVC)
    target_compile_options(print_ip PRIVATE
        /W4
    )
    target_compile_options(print_ip_lib PRIVATE
        /W4
    )
    target_compile_options(test_Version PRIVATE
        /W4
    )
else ()
    target_compile_options(print_ip PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(print_ip_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_Version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_Version test_Version)