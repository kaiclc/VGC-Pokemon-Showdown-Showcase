#pragma once
#include <vector>

class SegmentTree {
public:
    explicit SegmentTree(const std::vector<int>& data);
    int rangeSum(int l, int r);
private:
    std::vector<int> tree;
    int n;  // number of elements

    void build(const std::vector<int>& data, int node, int start, int end);
    int query(int node, int start, int end, int l, int r);
};




