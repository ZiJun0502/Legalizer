#include "globals.h"
#include "meta_segment_tree.h"


/* =====================================================
   --- SegmentTree implementation ---
   ===================================================== */

SegmentTree::SegmentTree(size_t size) : row_size(size) {
    free_space.assign(4 * size, 1.0); // Initially, all sites are free
    max_contiguous.assign(4 * size, size); // Initially, all sites can fit a full row
    buildTree(0, 0, row_size - 1); // Build the tree structure
}

// Builds the segment tree recursively for a given range (start to end).
// Each node represents a range of free space, and max_contiguous stores the largest contiguous free space.
void SegmentTree::buildTree(size_t node, size_t start, size_t end) {
    if (start == end) {
        max_contiguous[node] = free_space[node] = 1; // Each site initially free
    } else {
        size_t mid = (start + end) / 2;
        buildTree(2 * node + 1, start, mid); // Left child
        buildTree(2 * node + 2, mid + 1, end); // Right child
        max_contiguous[node] = end - start + 1; // Max space in the current node
    }
}

// Updates the free space at a specific position (pos) in the row with a given value.
void SegmentTree::update(size_t pos, double value) {
    updateTree(0, 0, row_size - 1, pos, value); // Recursively update the tree
}

// Helper function to update the segment tree in a specific range.
void SegmentTree::updateTree(size_t node, size_t start, size_t end, size_t pos, double value) {
    if (start == end) {
        free_space[node] = value;
        max_contiguous[node] = value > 0.0 ? 1 : 0;  // 1 if space is available, 0 if blocked
    } else {
        size_t mid = (start + end) / 2;
        if (pos <= mid) {
            updateTree(2 * node + 1, start, mid, pos, value); // Update left child
        } else {
            updateTree(2 * node + 2, mid + 1, end, pos, value); // Update right child
        }
        // Recalculate the max contiguous space
        max_contiguous[node] = std::max(max_contiguous[2 * node + 1], max_contiguous[2 * node + 2]);
    }
}

// Queries the maximum contiguous free space in a given range [l, r].
double SegmentTree::queryMaxContiguous(size_t l, size_t r) const {
    return queryMaxContiguous(0, 0, row_size - 1, l, r); // Recursively query the tree
}

// Helper function to query the max contiguous space in a range, using recursion.
double SegmentTree::queryMaxContiguous(size_t node, size_t start, size_t end, size_t l, size_t r) const {
    if (r < start || end < l) {
        return 0.0; // No overlap
    }
    if (l <= start && end <= r) {
        return max_contiguous[node]; // Total overlap
    }
    // Partial overlap
    size_t mid = (start + end) / 2;
    double left_query = queryMaxContiguous(2 * node + 1, start, mid, l, r);
    double right_query = queryMaxContiguous(2 * node + 2, mid + 1, end, l, r);
    return std::max(left_query, right_query); // Combine results from left and right child
}

/* =====================================================
   --- MetaSegmentTree implementation ---
   ===================================================== */

// Constructor: Initializes a MetaSegmentTree from a existing valid placement, where cells do not collide
MetaSegmentTree::MetaSegmentTree(const std::vector<PlacementRow>& placement_rows, std::vector<Cell>& cells) {
    for (const auto& row : placement_rows) {
        row_trees.emplace_back(SegmentTree(row.totalNumOfSites)); 
    }

    for (auto& cell : cells) {
        size_t row_idx = static_cast<size_t>(cell.y / site_height);  

        for (size_t i = static_cast<size_t>(cell.x); i <= static_cast<size_t>(cell.x1); ++i) {
            row_trees[row_idx].update(i, 0.0); // Mark space as occupied (0.0 means blocked)
        }
    }
}

// Constructor: Initializes a MetaSegmentTree from a vector of PlacementRow objects.
MetaSegmentTree::MetaSegmentTree(const std::vector<PlacementRow>& rows) {
    for (const auto& row : rows) {
        row_trees.emplace_back(SegmentTree(row.totalNumOfSites)); // Create a segment tree for each row
    }
}

// Updates the segment tree for a specific row (functionality not fully implemented).
void MetaSegmentTree::updateRow(size_t row) {
    // Update the segment tree for a particular row (functionality placeholder)
}

// New method to query the max contiguous space in a row (functionality commented out).
// double MetaSegmentTree::queryRowMaxContiguous(size_t row, size_t l, size_t r) const {
//     return row_trees[row].queryMaxContiguous(l, r);
// }

// New method to update the segment tree for a specific row (functionality commented out).
// void MetaSegmentTree::updateRowSpace(size_t row, size_t pos, double value) {
//     row_trees[row].update(pos, value);
// }

