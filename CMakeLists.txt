cmake_minimum_required(VERSION 3.29)
project(VGC_Pokemon_Showdown_Showcase)

set(CMAKE_CXX_STANDARD 14)

add_executable(VGC_Pokemon_Showdown_Showcase
    main.cpp
        SegmentTree.cpp
        PokemonSegTree.cpp
        FenwickTree.cpp
        PokemonFenwickTree.cpp
        simdjson.cpp)
