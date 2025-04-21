#include "FenwickTree.hpp"

FenwickTree::FenwickTree(const vector<int>& data) { //constructor
	size = data.size();
	build(data);
}

void FenwickTree::build(const vector<int>& data) { //builds the tree given input data
	tree.resize(data.size() + 1, 0); //changes indexing
	for (int i = 0; i < data.size(); i++) {
		tree[i + 1] = data[i];
	}

	for (int i = 1; i < tree.size(); i++) { //updates values according to Fenwick Tree format
		int j = i + (i & -i);
		if (j < tree.size()) {
			tree[j] += tree[i];
		}
	}
}

int FenwickTree::prefixSum(int index) { //computes the prefix sum
	int sum = 0;
	index++;
	while (index > 0) {
		sum += tree[index];
		index -= (index & -index);
	}
	return sum;
}

int FenwickTree::rangeSum(int l, int r) { //computes sum in a range using prefix sum
	if (size == 0 || l > r || l < 0 || r >= size) {
		return 0;
	}
	if (l == 0) {
		return prefixSum(r);
	}
	else {
		return prefixSum(r) - prefixSum(l - 1);
	}
}