cmake_minimum_required( VERSION 3.14 )
include( FetchContent )

#######################################################################
# Declare project dependencies
#######################################################################

FetchContent_Declare( abseil
    GIT_REPOSITORY  https://github.com/abseil/abseil-cpp.git
    GIT_TAG         20230125.0
    )

FetchContent_Declare( spdlog
    GIT_REPOSITORY  https://github.com/gabime/spdlog.git
    GIT_TAG         v1.11.0
    )

FetchContent_Declare( onetbb
    GIT_REPOSITORY  https://github.com/oneapi-src/oneTBB.git
    GIT_TAG         v2021.9.0
    )

FetchContent_MakeAvailable(abseil)

FetchContent_MakeAvailable(spdlog)

FetchContent_MakeAvailable(onetbb)