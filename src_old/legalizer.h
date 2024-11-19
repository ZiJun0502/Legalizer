#ifndef LEGALIZER_H
#define LEGALIZER_H

#include "globals.h"
#include "meta_segment_tree.h"



/* ==============================================================
   Legalizer class that contains the legalizer logic for placement
   ==============================================================
   This class handles the placement legalization process, which involves adjusting
   the position of cells to avoid overlap and ensure that all cells are properly
   placed within the available space. It uses the MetaSegmentTree to manage space
   availability across multiple rows and tracks each step of the legalization process.
*/
class Legalizer {
private:
    MetaSegmentTree* meta_tree;  // MetaSegmentTree to manage multiple rows and space availability
    void init_placement(std::vector<PlacementRow>& placement_rows, std::vector<Cell>& cells);
    void removeCellFromPlacement(const Cell& cell);  // Helper to remove a cell from the placement
    std::pair<int, int> findPlacement(const Cell& cell) const;  // Find the placement for the cell
    void insertCellToPlacement(const Cell& cell, double x0, double x1, double y0, double y1);  // Insert the cell into the placement

    // Nested struct for each legalization step
    struct LegalizationStep {
        int merged_ff_x;
        int merged_ff_y;
        int num_moved_cells;
        std::vector<Cell> moved_cells;
    };

public:
    Legalizer();  // Default constructor
    ~Legalizer();  // Destructor to clean up the MetaSegmentTree
    void legalize(std::vector<PlacementRow>& rows, std::vector<Cell>& cells, 
                  std::vector<BankingCell>& banking_cells);  // Legalize the cells in the placement
};

#endif
