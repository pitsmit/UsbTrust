include(FetchContent)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

add_compile_options(
    -Wall
    -Wextra
    -Wunused
    -Wold-style-cast
    -Wnon-virtual-dtor
    -Wshadow
    -Wsuggest-override
    -Wlogical-op
    -Wuseless-cast
)

option(ENABLE_COVERAGE "Enable coverage" ON)

function(enable_coverage target)
    if(ENABLE_COVERAGE)
        target_compile_options(${target} PRIVATE --coverage -O0 -g)
        target_link_options(${target} PRIVATE --coverage)
    endif()
endfunction()

# =========================
# UNIT TESTS
# =========================
add_executable(unit_tests
    unit/test_mount_point_builder.cpp
    unit/test_config.cpp
    unit/test_mount_flags_builder.cpp
    unit/test_lib_mount_tab.cpp
    unit/test_mount_mode.cpp
    helpers/ConfigFileBuilder/ConfigFileBuilder.cpp
    ../src/infrastructure/config/Config.cpp
    ../src/linux/MountFlagsBuilder/MountFlagsBuilder.cpp
    ../src/linux/LibMountTab/LibMountTab.cpp
    ../src/entities/MountMode/MountMode.cpp
    ../src/entities/DeviceInfo/DeviceInfoBuilder.cpp
    ../src/exceptions/Exceptions.cpp
    ../src/services/MountPointBuilderService/MountPointBuilderService.cpp
)

target_include_directories(unit_tests PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

set(CONFIG_PATH "config.txt")
target_compile_definitions(unit_tests PRIVATE 
    CONFIG_PATH="${CONFIG_PATH}"
)

target_link_libraries(unit_tests
    GTest::gtest_main
    mount
)

enable_coverage(unit_tests)

gtest_discover_tests(unit_tests
    PROPERTIES LABELS "unit"
)


# =========================
# INTEGRATION TESTS
# =========================
add_executable(integration_tests
    integration/test_mount_registry.cpp
    integration/test_device_manager.cpp
    helpers/ConfigFileBuilder/ConfigFileBuilder.cpp
    ../src/entities/DeviceInfo/DeviceInfoBuilder.cpp
    ../src/entities/MountMode/MountMode.cpp
    ../src/managers/MountRegistryManager/MountRegistryManager.cpp
    ../src/managers/DeviceManager/DeviceManager.cpp
    ../src/repositories/MountRepository/MountRepository.cpp
    ../src/repositories/DeviceInfoRepository/DeviceInfoRepository.cpp
    ../src/repositories/DeviceRepository/DeviceRepository.cpp
    ../src/storage/DataBase/DataBase.cpp
    ../src/storage/Statement/Statement.cpp
    ../src/storage/DBInitializer/DBInitializer.cpp
    ../src/exceptions/Exceptions.cpp
    ../src/infrastructure/config/Config.cpp
)

target_include_directories(integration_tests PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

set(CONFIG_PATH "config.txt")
target_compile_definitions(integration_tests PRIVATE 
    CONFIG_PATH="${CONFIG_PATH}"
)

target_link_libraries(integration_tests
    GTest::gtest_main
    GTest::gmock
    mount
    sqlite3
)

enable_coverage(integration_tests)

gtest_discover_tests(integration_tests
    PROPERTIES LABELS "integration"
)