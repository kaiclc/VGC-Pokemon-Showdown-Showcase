#pragma once
#include "SegmentTree.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <tuple>

class PokemonSegTree {
public:
    PokemonSegTree(const std::vector<int>& battles, const std::unordered_map<std::string, std::vector<int>>& realCounts);
    std::tuple<int, int> getUsage(const std::string& pokemonName, int startIndex, int endIndex);
        //  returned tuple contains realTotal, battleTotal
private:
    SegmentTree totalBattlesTree;
    std::unordered_map<std::string, SegmentTree> pokemonTrees;

};

