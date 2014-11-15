cmake_minimum_required(VERSION 2.8)
project(apollo C)
subdirs(src plugins hashmap)
file(COPY hashmap/bin/libhashmap.so DESTINATION ${apollo_SOURCE_DIR}/bin)
