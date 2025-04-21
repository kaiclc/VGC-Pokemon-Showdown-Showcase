#include "PokemonFenwickTree.hpp"

PokemonFenwickTree::PokemonFenwickTree(const vector<int>& battles, const unordered_map<string, vector<int>>& realCounts) //constructor
: totalBattlesTree(battles) {
	for (const auto& group : realCounts) {
		pokemonTrees.emplace(group.first, FenwickTree(group.second));
	}
}

tuple<int, int> PokemonFenwickTree::getUsage(const string& pokemonName, int startIndex, int endIndex) { //gets pokemon usage
	auto it = pokemonTrees.find(pokemonName);
	if (it == pokemonTrees.end()) {
		return { 0, 0 };
	}
	return {
		it->second.rangeSum(startIndex, endIndex),
		totalBattlesTree.rangeSum(startIndex, endIndex)
	};
}