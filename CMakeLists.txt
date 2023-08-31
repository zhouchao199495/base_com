cmake_minimum_required (VERSION 3.8)
project ("base")

set(CMAKE_CXX_STANDARD 17)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)


SET(PATH_PREFIX ./)
INCLUDE_DIRECTORIES(
    ${PATH_PREFIX}
    ${PATH_PREFIX}src
    ${PATH_PREFIX}include
)

LINK_DIRECTORIES(
    ${PATH_PREFIX}lib
)
# base
set(APP base)
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
AUX_SOURCE_DIRECTORY(base DIR_BASE)
add_executable(${APP} ${DIR_BASE} ${DIR_SRCS})
target_link_libraries(${APP} PUBLIC pthread log4cpp)

# client
set(APP client)
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
AUX_SOURCE_DIRECTORY(client DIR_CLIENT)
add_executable(${APP} ${DIR_CLIENT} ${DIR_SRCS})
target_link_libraries(${APP} PUBLIC pthread log4cpp)

