#include "tetris_legalizer.h"

TetrisLegalizer::TetrisLegalizer() {

}
TetrisLegalizer::TetrisLegalizer(const std::vector<PlacementRow>& placement_rows,
                     const std::vector<Cell>& cells_input): TetrisLegalizer() {
    // Copying the data into the class members
    this->placement = Placement(placement_rows);
    this->cells = cells_input;

    // Place fixed cells into placement
    for (auto& cell : cells) {
        cell_ref[cell.name] = &cell;
        if (cell.isFixed) {
            insertCellToPlacement(cell, {}, {});
        }
    }
    // printf("TetrisLegalizer: Finish Placement Initialization\n");
    // this->placement.printPlacementRows();
}
TetrisLegalizer::~TetrisLegalizer() {

}
// Removes a cell from the placement (freeing the space it occupied)
void TetrisLegalizer::removeCellFromPlacement(Cell& cell) {
    assert(cell.isPlaced && "Removing a cell that is not placed");
    // std::cout << cell << '\n';
    auto cell_rows = this->placement.getCellRows(cell);
    auto cell_cols = this->placement.getCellCols(cell);
    this->placement.remove(cell_rows, cell_cols);
    cell.isPlaced = false;
}

// Finds the nearest available space for a cell to be placed
// std::pair<int, int> TetrisLegalizer::findPlacement(const Cell& cell) const {
// }

// Places the cell at the corresponding sites based on its x and y coordinates.
void TetrisLegalizer::insertCellToPlacement(Cell& cell, std::pair<size_t, size_t> rows, std::pair<size_t, size_t> cols) {
    assert(!cell.isPlaced && "Inserting a cell that is already placed");
    if (rows.first == rows.second) {
        // position not specified, inesrt into placement based on cell coordinates
        rows = this->placement.getCellRows(cell);
        cols = this->placement.getCellCols(cell);
    } else {
        auto coord = this->placement.coordFromSiteInd(rows.first, cols.first);
        cell.x = coord.first;
        cell.y = coord.second;
    }
    this->placement.insert(rows, cols);
    cell.isPlaced = true;
}

// Main method to legalize placement of cells
bool TetrisLegalizer::legalize(std::vector<BankingCell> banking_cells) {
    this->writeDrawFile(std::string("init_draw.txt"));

    std::unordered_set<size_t> ignored_cell_ind;

    for (auto& banking_cell : banking_cells) {
        cells.push_back(banking_cell);
        cell_ref[banking_cell.name] = &cells.back();

        // Mark source cells for removal
        for (const auto& source_cell_name : banking_cell.source_cell_names) {
            Cell* source_cell = cell_ref[source_cell_name];
            int index = source_cell - &cells[0];
            ignored_cell_ind.insert(index);
        }
    }

    std::vector<Cell> new_cells;
    for (size_t i = 0; i < cells.size(); i++) {
        if (ignored_cell_ind.find(i) == ignored_cell_ind.end()) {
            new_cells.push_back(cells[i]);
        }
    }
    std::sort(new_cells.begin(), new_cells.end(), [](const Cell& a, const Cell& b) {
        return a.x < b.x;  // Sort cells by x-coordinate
    });
    this->writeOutputFile();
    this->writeDrawFile("draw.txt");
}
// helper functions
inline bool intersects(const Interval& int1, const Interval& int2) {
    return std::max(int1.lower(), int2.lower()) < std::min(int1.upper(), int2.upper());
}
inline bool widthIsLargerOrEq(const Interval& int1, size_t width) {
    return int1.upper() - int1.lower() >= width;
}
inline bool isOnSite(PlacementRow& placement_row, const Interval& int1) {
    return int1.lower() >= 0 && int1.lower() < placement_row.total_num_of_sites;
}

std::pair<size_t, size_t> TetrisLegalizer::findValidPosition(const std::pair<size_t, size_t>& desired_position, const Cell& cell) {
    size_t num_row_occupied = this->placement.getCellSiteHeight(cell);
    size_t num_col_occupied = this->placement.getCellSiteWidth(cell);
    size_t best_row = std::numeric_limits<size_t>::max();
    size_t best_col = std::numeric_limits<size_t>::max();
    size_t min_distance = std::numeric_limits<size_t>::max();

    auto site_ind = this->placement.siteIndFromCoord(cell.x, cell.y);
    size_t desired_row = site_ind.first;
    size_t desired_col = site_ind.second;

    for (size_t row = 0 ; row < this->placement.num_rows ; row++) {
        PlacementRow& placement_row = this->placement.placement_rows[row];
        auto first_pos = *placement_row.free_intervals.begin();
        if (isValidPosition(row, first_pos.lower(), cell)) {
            min_distance = std::min(min_distance, manhattanDistance(row, first_pos.lower(), desired_row, desired_col));
        }
    }
    return {best_row, best_col};
}
bool TetrisLegalizer::isValidPosition(size_t row, size_t col, const Cell& banking_cell) {
    // Check if the area from (row, col) can fit the merged_cell in placement
    size_t num_row_occupied = this->placement.getCellSiteHeight(banking_cell);
    size_t num_col_occupied = this->placement.getCellSiteWidth(banking_cell);
    // std::cout << banking_cell;
    // std::cout << num_row_occupied << ' ' << num_col_occupied << '\n';
    // Check if the cells in that region are free
    return !this->placement.isOccupied(std::make_pair(row, row+num_row_occupied), std::make_pair(col, col+num_col_occupied));
}

size_t TetrisLegalizer::manhattanDistance(size_t row1, size_t col1, size_t row2, size_t col2) {
    return std::abs((long long) row1 - (long long) row2) + std::abs((long long) col1 - (long long) col2);
}

void TetrisLegalizer::writeOutputFile() {
    std::ofstream output_file(output_filename);
    output_file << std::fixed << std::setprecision(6);
    for (auto outputInfo : outputInfos) {
        output_file << outputInfo.x << ' ' << outputInfo.y << '\n';
        output_file << outputInfo.num_moved_cell << '\n';
        for (size_t i = 0 ; i < outputInfo.moved_cell_name.size() ; i++) {
            output_file << outputInfo.moved_cell_name[i] << outputInfo.moved_cell_coord[i].first << ' ' << outputInfo.moved_cell_coord[i].second;
        }
    }
    output_file.close();
}

void TetrisLegalizer::writeDrawFile(std::string draw_filename) {
    std::ofstream draw_file(draw_filename);
    draw_file << std::fixed << std::setprecision(6);
    draw_file << "DieSize " << die_lower_left_x << ' ' << die_lower_left_y << ' ' << die_upper_right_x << ' ' << die_upper_right_y << '\n';
    for (Cell& cell : cells) {
        if (cell.isPlaced) {
            draw_file << cell.name << ' ' << cell.x << ' ' << cell.y << ' ' << cell.width << ' ' << cell.height << ' ' << cell.isFixed << '\n';
        }
    }
}