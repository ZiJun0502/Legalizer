#include "globals.h"
double alpha;
double beta;
double die_lower_left_x, die_lower_left_y, die_upper_right_x, die_upper_right_y;
double placement_row_start_x, placement_row_start_y;
double site_width, site_height;
std::vector<Cell> cells;
std::unordered_map<std::string, Cell*> cell_ref;
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
        std::cout << cell << "\n";  // This will call the overloaded << operator
    }

    // Print placement rows
    std::cout << "\nPlacement Rows:\n";
    for (const auto& row : placement_rows) {
        std::cout << row << "\n";  // This will call the overloaded << operator
    }

    // Print banking cells
    int lim = 0;
    std::cout << "\nBanking Cells:\n";
    for (const auto& banking_cell : banking_cells) {
        std::cout << banking_cell << "\n";  // This will call the overloaded << operator
        if(lim++>100) break;
    }

    std::printf("Num cells: %d, num place_rows: %d, num banking_cells: %d\n", 
                cells.size(), placement_rows.size(), banking_cells.size());
}
