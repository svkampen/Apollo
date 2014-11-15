set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${apollo_SOURCE_DIR}/bin)
set(APOLLO_SRC_FILES cJSON.c config.c net.c main.c list.c plugins.c protocol.c)

INCLUDE_DIRECTORIES(${apollo_SOURCE_DIR}/include ${apollo_SOURCE_DIR}/hashmap/include)
LINK_DIRECTORIES(${apollo_SOURCE_DIR}/bin)

ADD_DEFINITIONS(-ggdb -g3 -Wall -Werror -std=gnu11 -O2 -Wl,--dynamic-list=${apollo_SOUCE_DIR}/export)

ADD_EXECUTABLE(apollo ${APOLLO_SRC_FILES})
TARGET_LINK_LIBRARIES(apollo dl m hashmap)
