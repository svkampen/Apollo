CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(apollo_plugins C)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../bin/plugins)

IF(NOT DEFINED BUILD_BF)
	SET(BUILD_BF FALSE)
ENDIF(NOT DEFINED BUILD_BF)

INCLUDE_DIRECTORIES(../include ../hashmap/include)

ADD_DEFINITIONS(-ggdb -g3 -O0 -Wall -Wno-unused-variable -std=gnu11)

ADD_LIBRARY(uncommon SHARED uncommon/uncommon.c)
SET_TARGET_PROPERTIES(uncommon PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(uncommon PROPERTIES COMPILE_FLAGS "-Wno-error=nonnull -Wno-nonnull -O0")

ADD_LIBRARY(common SHARED common/common.c)
SET_TARGET_PROPERTIES(common PROPERTIES PREFIX "")

ADD_LIBRARY(number SHARED number/number.c)
SET_TARGET_PROPERTIES(number PROPERTIES PREFIX "")

IF (BUILD_BF)
	ADD_LIBRARY(bf SHARED bf/bf.c)
	SET_TARGET_PROPERTIES(bf PROPERTIES PREFIX "")
	ADD_CUSTOM_COMMAND(TARGET bf PRE_BUILD COMMAND rm -rf BCI 2>/dev/null)
	ADD_CUSTOM_COMMAND(TARGET bf POST_BUILD COMMAND git clone https://github.com/svkampen/BCI COMMAND make -sC BCI shared COMMAND cp BCI/libbci.so ${apollo_SOURCE_DIR}/bin/plugins)
ENDIF (BUILD_BF)

ADD_LIBRARY(proto_irc SHARED proto_irc/proto_irc.c proto_irc/proto_irc_parse.c proto_irc/proto_irc_handlers.c)
SET_TARGET_PROPERTIES(proto_irc PROPERTIES PREFIX "")
