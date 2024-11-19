#include "globals.h"
double alpha;
double beta;
double die_lower_left_x, die_lower_left_y, die_upper_right_x, die_upper_right_y;
extern double placement_row_start_x, placement_row_start_y, placement_row_start_x;
extern double site_width, site_height;
std::vector<Cell> cells;
std::vector<PlacementRow> placement_rows;
std::vector<BankingCell> banking_cells;
char *output_filename;

void print_globals() {
    // Print scalar globals
    std::cout << "Alpha: " << alpha << "\n";
    std::cout << "Beta: " << beta << "\n";
    std::cout << "Die Lower Left (X, Y): (" << die_lower_left_x << ", " << die_lower_left_y << ")\n";
    std::cout << "Die Upper Right (X, Y): (" << die_upper_right_x << ", " << die_upper_right_y << ")\n";

    // Print cells
    std::cout << "\nCells:\n";
    for (const auto& cell : cells) {
        std::cout << "Name: " << cell.name 
                  << ", Position: (" << cell.x << ", " << cell.y << ")"
                  << ", Size: (" << cell.width << ", " << cell.height << ")"
                  << ", Fixed: " << (cell.isFixed ? "Yes" : "No") << "\n";
    }

    // Print placement rows
    std::cout << "\nPlacement Rows:\n";
    for (const auto& row : placement_rows) {
        std::cout << "Start Position: (" << row.startX << ", " << row.startY << ")"
                  << ", Site Width: " << row.siteWidth 
                  << ", Site Height: " << row.siteHeight 
                  << ", Total Number of Sites: " << row.totalNumOfSites << "\n";
    }

    // Print banking cells
    std::cout << "\nBanking Cells:\n";
    for (const auto& banking_cell : banking_cells) {
        std::cout << "Source Cells: ";
        for (const auto& source_cell : banking_cell.source_cell_names) {
            std::cout << source_cell << " ";
        }
        std::cout << "\nMerged Cell Name: " << banking_cell.name
                  << ", Position: (" << banking_cell.x << ", " << banking_cell.y << ")"
                  << ", Size: (" << banking_cell.width << ", " << banking_cell.height << ")\n";
    }
    printf("Num cells: %d, num place_rows: %d, num banking_cells: %d\n", cells.size(), placement_rows.size(), banking_cells.size());
}