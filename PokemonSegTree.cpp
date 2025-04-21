#include "PokemonSegTree.hpp"

PokemonSegTree::PokemonSegTree(const std::vector<int>& battles,
                         const std::unordered_map<std::string, std::vector<int>>& realCounts)
    : totalBattlesTree(battles) {
    // initializes totalBattlesTree to segmentTree with data = battles
    // and adds each pokemon to pokemonTrees
    for (const auto& [name, counts] : realCounts) {
        pokemonTrees.emplace(name, SegmentTree(counts));
    }
}

std::tuple<int, int> PokemonSegTree::getUsage(const std::string& pokemonName, int startIndex, int endIndex) {
    auto it = pokemonTrees.find(pokemonName);
    // case 1: pokemon does not exist
    if (it == pokemonTrees.end()) {
        return {0, 0};
    }

    // case 2: return tuple containing rangeSums of realTotal and battleTotal
    return {
        it->second.rangeSum(startIndex, endIndex),
        totalBattlesTree.rangeSum(startIndex, endIndex)
    };
}