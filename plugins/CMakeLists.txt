CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(apollo_plugins C)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../bin/plugins)

INCLUDE_DIRECTORIES(../include ../hashmap/include)

ADD_DEFINITIONS(-ggdb -g3 -Wall -Wno-unused-variable -Werror -std=gnu11)

ADD_LIBRARY(uncommon SHARED uncommon/uncommon.c)
SET_TARGET_PROPERTIES(uncommon PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(uncommon PROPERTIES COMPILE_FLAGS "-Wno-error=nonnull -Wno-nonnull -O0")

ADD_LIBRARY(common SHARED common/common.c)
SET_TARGET_PROPERTIES(common PROPERTIES PREFIX "")

ADD_LIBRARY(proto_irc SHARED proto_irc/proto_irc.c proto_irc/proto_irc_parse.c proto_irc/proto_irc_handlers.c)
SET_TARGET_PROPERTIES(proto_irc PROPERTIES PREFIX "")
