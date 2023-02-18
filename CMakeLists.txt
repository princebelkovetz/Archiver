cmake_minimum_required(VERSION 3.21)
project(archiver)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        archiver
        archiver.cpp
        archive_maker.cpp archive_maker.h trie_node.cpp trie_node.h file_reader.cpp file_reader.h file_writer.cpp file_writer.h archive_unpacker.cpp archive_unpacker.h int9t_namespace.h parser.cpp parser.h exceptions.cpp exceptions.h trie_build.h trie_build.cpp)

