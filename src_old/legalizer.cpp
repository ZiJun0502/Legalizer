#include "legalizer.h"


/* =====================================================
   --- Legalizer implementation ---
   ===================================================== */

// Removes a cell from the placement and frees the space in the meta-segment tree.
void Legalizer::init_placement(std::vector<PlacementRow>& placement_rows, 
                               std::vector<Cell>& cells) {
    meta_tree = new MetaSegmentTree(placement_rows, cells);
}

void Legalizer::removeCellFromPlacement(const Cell& cell) {
    // Free the space occupied by the cell in the meta-segment tree
    // This function will update the segment tree for the affected rows (placeholder)
}

// Finds a valid placement for the cell in a row, ensuring enough space is available.
std::pair<int, int> Legalizer::findPlacement(const Cell& cell) const {
    // Use meta-tree to find contiguous rows with sufficient space
    // Placeholder for finding placement logic
    // return {-1, -1}; // Indicating no valid placement found
}

// Inserts a cell into the placement structure and updates the segment tree for the occupied space.
void Legalizer::insertCellToPlacement(const Cell& cell, double x0, double x1, double y0, double y1) {
    // Insert the cell in the placement structure and update the segment tree
    // Mark the space as occupied for the inserted cell (placeholder)
}

// Legalizes the placement of cells by first finding placements, removing cells, and then inserting them.
void Legalizer::legalize(std::vector<PlacementRow>& placement_rows, 
                         std::vector<Cell>& cells, 
                         std::vector<BankingCell>& banking_cells) {
    if (!meta_tree) {
        init_placement(placement_rows, cells); // Initialize placement if not already done
    }

    std::ofstream output_file(output_filename); // Open the output file to write legalized placements
    // Create segtrees/metatree where they will store the spaces left considering all cells are inserted
    // Placeholder for legalizing the placement and writing the result to output file
    for (auto& banking_cell : banking_cells) {
        for (std::string source_cell_name : banking_cell.source_cell_names) {
            Cell* source_cell = cell_ref[source_cell_name];
            removeCellFromPlacement(*source_cell); // Remove the source cell from placement
        }

        // Placeholder for finding the placement and inserting the banking cell
        // auto [row_idx, max_space] = findPlacement(banking_cell);
        // if (row_idx != -1) {
        //     insertCellToPlacement(banking_cell, 0.0, max_space, row_idx, row_idx + banking_cell.height);
        //     output_file << merged_ff_x << " " << merged_ff_y << std::endl;
        // }
    }

    // Close the file after writing the legal placement information
    output_file.close();
}
