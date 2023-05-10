cmake_minimum_required( VERSION 3.14 )

find_package(Libevent REQUIRED)
include_directories(${LIBEVENT_INCLUDE_DIR})

include( ${SENTINEL_ROOT_DIR}/cmake/third_party.cmake )

include_directories( ${SENTINEL_ROOT_DIR} )

include_directories(${SENTINEL_CORE_ROOT_DIR}/common)
include_directories(${SENTINEL_CORE_ROOT_DIR}/config)
include_directories(${SENTINEL_CORE_ROOT_DIR}/property)
include_directories(${SENTINEL_CORE_ROOT_DIR}/public)

# common
file(GLOB COMMON_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/common/*.cc")
file(GLOB COMMON_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/common/*_test.cc")
list(REMOVE_ITEM COMMON_SOURCE_FILES ${COMMON_TEST_SOURCE_FILES})

# config
file(GLOB CONFIG_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/config/*.cc")
file(GLOB CONFIG_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/config/*_test.cc")
list(REMOVE_ITEM CONFIG_SOURCE_FILES ${CONFIG_TEST_SOURCE_FILES})

# log
file(GLOB_RECURSE LOG_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/log/*.cc")
file(GLOB_RECURSE LOG_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/log/*_test.cc")
list(REMOVE_ITEM LOG_SOURCE_FILES ${LOG_TEST_SOURCE_FILES})

# utils
file(GLOB UTILS_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/utils/*.cc")
# file(GLOB UTILS_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/utils/*test.cc")
# list(REMOVE_ITEM UTILS_SOURCE_FILES ${UTILS_TEST_SOURCE_FILES})

# slot
file(GLOB_RECURSE SLOT_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/slot/*.cc")
file(GLOB_RECURSE SLOT_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/slot/*_test.cc")
list(REMOVE_ITEM SLOT_SOURCE_FILES ${SLOT_TEST_SOURCE_FILES})

# statistic
file(GLOB_RECURSE STATISTIC_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/statistic/*.cc")
file(GLOB_RECURSE STATISTIC_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/statistic/*_test.cc")
list(REMOVE_ITEM STATISTIC_SOURCE_FILES ${STATISTIC_TEST_SOURCE_FILES})

# transport
file(GLOB_RECURSE TRANSPORT_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/transport/*.cc")
file(GLOB_RECURSE TRANSPORT_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/transport/*_test.cc")
list(REMOVE_ITEM TRANSPORT_SOURCE_FILES ${TRANSPORT_TEST_SOURCE_FILES})

# flow
file(GLOB FLOW_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/flow/*.cc")
file(GLOB FLOW_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/flow/*_test.cc")
list(REMOVE_ITEM FLOW_SOURCE_FILES ${FLOW_TEST_SOURCE_FILES})

# circuitbreaker
file(GLOB CIRCUIT_BREAKER_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/circuitbreaker/*.cc")
file(GLOB CIRCUIT_BREAKER_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/circuitbreaker/*_test.cc")
list(REMOVE_ITEM CIRCUIT_BREAKER_SOURCE_FILES ${CIRCUIT_BREAKER_TEST_SOURCE_FILES})

# param
file(GLOB_RECURSE PARAM_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/param/*.cc")
file(GLOB_RECURSE PARAM_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/param/*_test.cc")
list(REMOVE_ITEM PARAM_SOURCE_FILES ${PARAM_TEST_SOURCE_FILES})

# system
file(GLOB_RECURSE SYSTEM_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/system/*.cc")
file(GLOB_RECURSE SYSTEM_TEST_SOURCE_FILES "${SENTINEL_CORE_ROOT_DIR}/system/*_test.cc")
list(REMOVE_ITEM SYSTEM_SOURCE_FILES ${SYSTEM_TEST_SOURCE_FILES})

list(APPEND
     SENTINEL_SOURCE_FILES 
     ${COMMON_SOURCE_FILES}
     ${CONFIG_SOURCE_FILES}
     ${LOG_SOURCE_FILES}
     ${UTILS_SOURCE_FILES}
     ${SLOT_SOURCE_FILES}
     ${STATISTIC_SOURCE_FILES}
     ${TRANSPORT_SOURCE_FILES}
     ${FLOW_SOURCE_FILES}
     ${CIRCUIT_BREAKER_SOURCE_FILES}
     ${PARAM_SOURCE_FILES}
     ${SYSTEM_SOURCE_FILES})

add_library(sentinel STATIC ${SENTINEL_SOURCE_FILES})

target_link_libraries(sentinel PUBLIC 
                      libevent::core
                      libevent::extra
                      libevent::pthreads
                      TBB::tbb
                      spdlog
                      absl::flat_hash_set
                      absl::str_format
                      absl::synchronization
                      absl::strings
                      absl::any)