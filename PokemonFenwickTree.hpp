#pragma once
#include "FenwickTree.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class PokemonFenwickTree {
	FenwickTree totalBattlesTree;
	unordered_map<string, FenwickTree> pokemonTrees;
public:
	PokemonFenwickTree(const vector<int>& battles, const unordered_map<string, vector<int>>& realCounts);
	tuple<int, int> getUsage(const string& pokemonName, int startIndex, int endIndex);
};