#include "SegmentTree.hpp"

SegmentTree::SegmentTree(const std::vector<int>& data) {
    n = data.size();
    if (n == 0) {
        tree = std::vector<int>();
        return;
    }

    // initialization using safe upper bound of 4n
    // we need 2 * next power of two
    tree.resize(4 * n, 0);

    // start recursive tree building from root node (index 1)
    // note that using 1-based indexing simplifies child/parent calculations:
    // - left child of node i: 2i
    // - right child of node i: 2i+1
    build(data, 1, 0, n - 1);
}

// recursive tree construction helper
// node: current node index in tree array
// start/end: represents data interval (inclusive-inclusive) this node is responsible for
void SegmentTree::build(const std::vector<int>& data, int node, int start, int end) {
    // base case: leaf node (single element)
    if (start == end) {
        tree[node] = data[start];
        return;
    }

    // recursive case: internal node
    // split interval into two halves
    int mid = (start + end) / 2;

    // build left subtree
    build(data, 2 * node, start, mid);

    // build right subtree
    build(data, 2 * node + 1, mid + 1, end);

    // sum results from children
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// recursively finds sum in range from l to r
int SegmentTree::query(int node, int start, int end, int l, int r)  {
    // case 1: current segment does not overlap with query range
    if (r < start || end < l) {
        return 0;
    }

    // case 2: current segment is fully in query range
    if (l <= start && end <= r) {
        return tree[node];
    }

    // case 3: partial overlap - divide and conquer
    int mid = (start + end) / 2;
    // query both children and add results
    int leftSum = query(2 * node, start, mid, l, r);
    int rightSum = query(2 * node + 1, mid + 1, end, l, r);
    return leftSum + rightSum;
}


int SegmentTree::rangeSum(int l, int r)  {
    // h
    if (n == 0 || l > r || l < 0 || r >= n) {
        return 0;
    }

    // start recursive query from root node (index 1)
    // which covers all elements from 0 to n-1
    return query(1, 0, n - 1, l, r);
}