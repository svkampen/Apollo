CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(apollo_plugins C)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../bin/plugins)

INCLUDE_DIRECTORIES(../include ../hashmap/include)

ADD_DEFINITIONS(-ggdb -g3 -Wall -Werror)
# ADD_LIBRARY(common SHARED common/common.c)

ADD_LIBRARY(proto_irc SHARED proto_irc/proto_irc.c proto_irc/proto_irc_parse.c)
SET_TARGET_PROPERTIES(proto_irc PROPERTIES PREFIX "")
