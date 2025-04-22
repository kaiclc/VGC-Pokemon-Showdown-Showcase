#pragma once
#include "FenwickTree.hpp"
#include "simdjson.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;
using namespace simdjson;

class PokemonFenwickTree {
	FenwickTree totalBattlesTree;
	unordered_map<string_view, FenwickTree> pokemonTrees;
public:
	PokemonFenwickTree(const vector<int>& battles, const unordered_map<string_view, vector<int>>& realCounts);
	tuple<int, int> getUsage(const string_view& pokemonName, int startIndex, int endIndex);
};
