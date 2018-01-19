cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)
project(SimpleStupidFunnel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SSF_BUILD_TESTS "Build unit tests created with GoogleTest framework" OFF)
option(SSF_UPDATE_REPOS "Update thirdpary git repos" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

######################
### EXTERNAL REPOS ###
######################
include(DownloadProject)

if (SSF_UPDATE_REPOS)
    set(UPDATE_STATUS 0)
else ()
    set(UPDATE_STATUS 1)
endif ()

### SimulationDriver ###
download_project(PROJ sim
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/LoganBarnes/SimulationDriver.git
        GIT_TAG master
        UPDATE_DISCONNECTED ${UPDATE_STATUS}
        QUIET
        )

set(SIM_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# compile sim with current project
add_subdirectory(${sim_SOURCE_DIR} ${sim_BINARY_DIR})

#############################
### Project Configuration ###
#############################
if (NOT MSVC)
    set(INTENSE_FLAGS -Wall -Wextra -Winit-self -Winit-self
            -Wold-style-cast -Woverloaded-virtual -Werror
            -Wmissing-declarations -pedantic -Wuninitialized)
else ()
    set(INTENSE_FLAGS -WX)
endif ()


###############
### Project ###
###############

set(SSF_SOURCE_FILES
        src/ssf/ssf.cpp
        src/ssf/ssf.hpp
        src/ssf/test_cases.hpp)

add_library(ssf ${SSF_SOURCE_FILES})
target_include_directories(ssf PUBLIC src)
target_link_libraries(ssf SimDriver)
target_compile_options(ssf PRIVATE ${INTENSE_FLAGS})

set(EXEC_SOURCE_FILES
        src/exec/SimpleStupidFunnel.cpp
        src/exec/SimpleStupidFunnel.hpp
        src/exec/main.cpp
        )
configure_file(src/exec/Config.hpp.in ${CMAKE_BINARY_DIR}/ssf/Config.hpp)

add_executable(funnel ${EXEC_SOURCE_FILES})
target_link_libraries(funnel ssf ${CMAKE_BINARY_DIR})
target_compile_options(funnel PRIVATE ${INTENSE_FLAGS})


###############
### Testing ###
###############

if (${SSF_BUILD_TESTS})
    # Download and unpack googletest at configure time
    download_project(PROJ googletest
            PREFIX thirdparty
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG master
            UPDATE_DISCONNECTED ${UPDATE_STATUS}
            QUIET
            )

    # Prevent overriding the parent project's compiler/linker settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # compile googletest with current project
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

    target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
            "${googletest_SOURCE_DIR}/googletest/include"
            "${googletest_SOURCE_DIR}/googlemock/include")

    # add test coverage capabilities
    if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES "Debug")
        include(CodeCoverage)
        setup_target_for_coverage(${PROJECT_NAME}_coverage ssf-tests coverage)
        set(PROFILE_FLAGS -fprofile-arcs -ftest-coverage)
        set(PROFILE_LIBS gcov)
        target_compile_options(ssf PRIVATE ${PROFILE_FLAGS})
        target_link_libraries(ssf ${PROFILE_LIBS})
    endif ()

    include(CTest)
    enable_testing()

    set(TEST_SOURCE_FILES
            src/testing/SSFTests.cpp
            )

    add_executable(ssf-tests ${TEST_SOURCE_FILES})
    target_link_libraries(ssf-tests ssf gmock_main ${PROFILE_LIBS})
    target_compile_options(ssf-tests PRIVATE ${INTENSE_FLAGS} ${PROFILE_FLAGS})
    add_test(NAME ssf-test COMMAND ssf-tests)
endif ()
