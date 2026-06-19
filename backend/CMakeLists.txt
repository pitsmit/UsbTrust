cmake_minimum_required(VERSION 3.28.3)
project(UsbTrust)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_APP "Build application" ON)
option(BUILD_HTTP_SERVER "Build HTTP server" ON)
option(ENABLE_TESTS "Enable test build" ON)
option(ENABLE_TEST_API "Enable test-only HTTP endpoints" OFF)

add_compile_options(
    -Wall
    -Wextra
    -Wunused
)

if(BUILD_APP)

    file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
        src/*.cpp
    )

    if(BUILD_HTTP_SERVER)
        list(APPEND APP_SOURCES
            api-stubs/api/ApiBase.cpp
            api-stubs/api/DefaultApi.cpp
        )
    endif()

    add_executable(app ${APP_SOURCES})

    target_include_directories(app PUBLIC
        ${PROJECT_SOURCE_DIR}/src
    )

    if(BUILD_HTTP_SERVER)
        target_include_directories(app PUBLIC
            ${PROJECT_SOURCE_DIR}/api-stubs/api
        )

        target_compile_definitions(app PUBLIC
            BUILD_HTTP_SERVER
        )
    endif()

    if(ENABLE_TEST_API)
        target_compile_definitions(app PUBLIC
            ENABLE_TEST_API
        )
    endif()

    add_subdirectory(external/IXWebSocket)

    target_link_libraries(app
        PRIVATE
        sqlite3
        pthread
        udev
        blkid
        systemd
        mount
        ixwebsocket
    )

    if(BUILD_HTTP_SERVER)
        target_link_libraries(app
            PRIVATE
            pistache
        )
    endif()

endif()

if(ENABLE_TESTS)

    file(COPY ${CMAKE_SOURCE_DIR}/sql
        DESTINATION ${CMAKE_BINARY_DIR}/tests)

    file(COPY ${CMAKE_SOURCE_DIR}/config.txt
        DESTINATION ${CMAKE_BINARY_DIR}/tests)

    file(RENAME
        ${CMAKE_BINARY_DIR}/tests/config.txt
        ${CMAKE_BINARY_DIR}/tests/config.base.txt)

    file(COPY ${CMAKE_SOURCE_DIR}/config.txt
        DESTINATION ${CMAKE_BINARY_DIR}/tests)

    enable_testing()
    add_subdirectory(tests)

endif()

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")

    add_compile_options(
        --coverage
        -O0
        -g
        -fno-inline
        -fno-default-inline
    )

    add_link_options(--coverage)

    add_custom_target(coverage
        COMMAND lcov --capture
            --directory ${CMAKE_BINARY_DIR}
            --base-directory ${CMAKE_SOURCE_DIR}
            --output-file coverage.info

        COMMAND lcov --remove coverage.info
            '/usr/*'
            '*/build/*'
            '*/_deps/*'
            '*/external/*'
            '*/tests/*'
            --output-file coverage.tmp.info

        COMMAND lcov --extract coverage.tmp.info
            '*/src/*'
            '*/api-stubs/*'
            --output-file coverage.filtered.info

        COMMAND genhtml coverage.filtered.info
            --output-directory coverage-report

        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

endif()

add_custom_target(build-containers
    COMMAND docker compose build
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

find_program(CPPCHECK cppcheck)

if(CPPCHECK)
    add_custom_target(cppcheck
        COMMAND cppcheck
            --enable=all
            --inconclusive
            --std=c++23
            --project=${CMAKE_BINARY_DIR}/compile_commands.json
            --suppress=missingIncludeSystem
            --suppress=unmatchedSuppression
            --quiet
            --inline-suppr
            --checkers-report=errs.txt
            -i ${CMAKE_BINARY_DIR}/_deps
            -i ${CMAKE_SOURCE_DIR}/external
            -i ${CMAKE_BINARY_DIR}/external
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
else()
    message(WARNING "cppcheck not found")
endif()