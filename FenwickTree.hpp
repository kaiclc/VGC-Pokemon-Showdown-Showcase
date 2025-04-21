#pragma once
#include <vector>
using namespace std;

class FenwickTree {
	vector<int> tree;
	int size;
	void build(const vector<int>& data);
	int prefixSum(int index);
public:
	explicit FenwickTree(const vector<int>& data);
	int rangeSum(int l, int r);
};