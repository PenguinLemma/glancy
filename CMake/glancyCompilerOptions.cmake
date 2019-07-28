if(NOT DEFINED _GLANCY_GNU_CLANG_COMPILER_OPTIONS)
    set(_GLANCY_GNU_CLANG_COMPILER_OPTIONS -Wall -Wextra -Wpedantic -Werror)
endif()

if(NOT DEFINED _GLANCY_MSVC_COMPILER_OPTIONS)
    set(_GLANCY_MSVC_COMPILER_OPTIONS /W4 /WX /permissive-)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(GLANCY_COMPILER_OPTIONS ${_GLANCY_GNU_CLANG_COMPILER_OPTIONS})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(GLANCY_COMPILER_OPTIONS ${_GLANCY_MSVC_COMPILER_OPTIONS})
endif()
