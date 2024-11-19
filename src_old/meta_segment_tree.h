#ifndef META_SEGMENT_TREE_H
#define META_SEGMENT_TREE_H

#include "globals.h"


/* =====================================================
   SegmentTree class to manage free space in a single row
   =====================================================
   This class represents a segment tree that tracks the available free space at each site
   in a single placement row. It supports building the tree, updating the space at a specific site,
   and querying for the maximum contiguous free space within a specified range.
*/
class SegmentTree {
private:
    std::vector<size_t> free_space;  // Free space at each site (using double for precision)
    std::vector<size_t> max_contiguous;  // Max contiguous free space
    size_t row_size;

    void buildTree(size_t node, size_t start, size_t end);  // Helper function to build the tree
    void updateTree(size_t node, size_t start, size_t end, size_t pos, double value);  // Update the tree when space at a site changes
    double queryMaxContiguous(size_t node, size_t start, size_t end, size_t l, size_t r) const;  // Query for max contiguous free space

public:
    SegmentTree(size_t size);  // Constructor to initialize the segment tree for a given row size
    void update(size_t pos, double value); // Update a single site (value represents the space)
    double queryMaxContiguous(size_t l, size_t r) const; // Get max contiguous free space in range [l, r]
    double getTotalFreeSpace() const; // Get the total free space in the segment tree
};

/* ==========================================================
   MetaSegmentTree class to manage the segment trees for multiple rows
   ==========================================================
   This class represents a meta-segment tree that manages multiple SegmentTree instances,
   each representing a row of placement. It allows querying and updating of multiple rows
   of cells to optimize placement during legalization.
*/
class MetaSegmentTree {
private:
    std::vector<SegmentTree> row_trees; // Segment trees for each row
    std::vector<int> max_free_rows;    // Row indices with maximum free space
    std::vector<double> total_free;    // Total free space per meta-tree range

    void buildTree(size_t node, size_t start, size_t end);  // Build the meta-tree
    void updateTree(size_t node, size_t start, size_t end, size_t row);  // Update meta-tree when a row's space changes

public:
    MetaSegmentTree(const std::vector<PlacementRow>& rows);  // Initialize the meta-tree using placement rows
    MetaSegmentTree(const std::vector<PlacementRow>& placement_rows, std::vector<Cell>& cells); 
    
    // Function to access a specific row's SegmentTree
    SegmentTree& getRowTree(size_t index) {
        return row_trees[index];
    }

    const SegmentTree& getRowTree(size_t index) const {
        return row_trees[index];
    }

    void updateRow(size_t row);  // Update row after insertion or deletion of a cell
    
    // Function to find rows that can accommodate a cell
    std::pair<int, int> findContiguousRows(double cell_width, int num_rows) const;
};

#endif