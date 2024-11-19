#include "legalizer.h"

Legalizer::Legalizer() {

}
Legalizer::Legalizer(const std::vector<PlacementRow>& placement_rows,
                     const std::vector<Cell>& cells_input): Legalizer() {
    // Copying the data into the class members
    this->placement = Placement(placement_rows);
    this->cells = cells_input;

    // Build cell_ref map for quick lookup by name
    for (auto& cell : cells) {
        cell_ref[cell.name] = &cell;
        insertCellToPlacement(cell, {}, {});
    }
    // printf("Legalizer: Finish Placement Initialization\n");
    // this->placement.printPlacementRows();
}
Legalizer::~Legalizer() {

}
// Removes a cell from the placement (freeing the space it occupied)
void Legalizer::removeCellFromPlacement(Cell& cell) {
    assert(cell.isPlaced && "Removing a cell that is not placed");
    // std::cout << cell << '\n';
    auto cell_rows = this->placement.getCellRows(cell);
    auto cell_cols = this->placement.getCellCols(cell);
    this->placement.remove(cell_rows, cell_cols);
    cell.isPlaced = false;
}

// Finds the nearest available space for a cell to be placed
// std::pair<int, int> Legalizer::findPlacement(const Cell& cell) const {
// }

// Places the cell at the corresponding sites based on its x and y coordinates.
void Legalizer::insertCellToPlacement(Cell& cell, std::pair<size_t, size_t> rows, std::pair<size_t, size_t> cols) {
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
void Legalizer::legalize(std::vector<BankingCell>& banking_cells) {
    for (auto& banking_cell : banking_cells) {
        // remove source cell
        for (std::string& source_cell_name : banking_cell.source_cell_names) {
            Cell& source_cell = *cell_ref[source_cell_name];
            removeCellFromPlacement(source_cell);
            // this->placement.remove(this->placement.getCellRows(source_cell), this->placement.getCellCols(source_cell));
        }
        // the desired rows/cols coordinates
        std::pair<size_t, size_t> target_rows = this->placement.getCellRows(banking_cell);
        std::pair<size_t, size_t> target_cols = this->placement.getCellCols(banking_cell);

        size_t num_row_occupied = this->placement.getCellSiteHeight(banking_cell);
        size_t num_col_occupied = this->placement.getCellSiteWidth(banking_cell);
        // site coordinate
        std::pair<size_t, size_t> target_pos = std::make_pair(target_rows.first, target_cols.first);
        // valid_positoin: [row, col] site in placement
        std::pair<size_t, size_t> valid_position = findValidPosition(target_pos, banking_cell);
        std::pair<double, double> valid_position_coord = std::make_pair(
            valid_position.second * this->placement.site_width + this->placement.start_x,
            valid_position.first * this->placement.site_height + this->placement.start_y
        );
        std::pair<size_t, size_t> inserting_rows = std::make_pair(valid_position.first, valid_position.first + num_row_occupied);
        std::pair<size_t, size_t> inserting_cols = std::make_pair(valid_position.second, valid_position.second + num_col_occupied);
        std::cout << banking_cell << '\n';
        printf("Valid position: (%zu, %zu)\n", valid_position.first, valid_position.second);
        printf("Target row: (%zu, %zu), Target col: (%zu, %zu)\n", target_rows.first, target_rows.second, target_cols.first, target_cols.second);
        printf("Insert row: (%zu, %zu), Insert col: (%zu, %zu), ", inserting_rows.first, inserting_rows.second, inserting_cols.first, inserting_cols.second);
        printf("Num of interval in row: %zu\n", this->placement.placement_rows[target_rows.first].free_intervals.size());
        // this->placement.insert(inserting_rows, inserting_cols);
        this->insertCellToPlacement(banking_cell, inserting_rows, inserting_cols);
        this->outputInfos.emplace_back(valid_position_coord);
    }
    this->writeOutputFile();
    this->writeDrawFile();
}
// helper functions
inline bool intersects(const Interval& int1, const Interval& int2) {
    return std::max(int1.lower(), int2.lower()) < std::min(int1.upper(), int2.upper());
}
inline bool widthIsLargerOrEq(const Interval& int1, size_t width) {
    return int1.upper() - int1.lower() >= width;
}

std::pair<size_t, size_t> Legalizer::findValidPosition(const std::pair<size_t, size_t>& desired_position, const Cell& banking_cell) {
    size_t num_row_occupied = this->placement.getCellSiteHeight(banking_cell);
    size_t num_col_occupied = this->placement.getCellSiteWidth(banking_cell);
    size_t best_row = desired_position.first;
    size_t best_col = desired_position.second;
    size_t min_distance = std::numeric_limits<size_t>::max();

    long long num_rows_to_search = this->placement.num_rows;
    long long row_start = std::max((long long) best_row - num_rows_to_search / 2, 0LL);
    long long row_end   = std::min((long long) best_row + num_rows_to_search / 2, (long long) this->placement.num_rows);
    num_rows_to_search = row_end - row_start;
    std::vector<std::vector<Interval>> available_intervals(num_rows_to_search);
    for (long long row = 0 ; row < num_rows_to_search ; row++) {
        available_intervals[row] = this->placement.placement_rows[row + row_start].filterIntervalsByWidth(num_col_occupied);
        // for (auto i: available_intervals[row]) {
        //     std::cout << i << ' ';
        // }
        // std::cout << '\n';
    }
    for (size_t start_row = 0; start_row + num_row_occupied <= available_intervals.size(); start_row++) {
        // Initialize the intersection to the intervals of the first row in the window
        std::vector<Interval> intersection = available_intervals[start_row];
        
        // Compute intersection across rows in the sliding window
        for (size_t offset = 1; offset < num_row_occupied; offset++) {
            const auto& next_row_intervals = available_intervals[start_row + offset];
            std::vector<Interval> new_intersection;
            Interval temp;
            for (const auto& int1 : intersection) {
                for (const auto& int2 : next_row_intervals) {
                    if (intersects(int1, int2) && widthIsLargerOrEq(temp = intersect(int1, int2), num_col_occupied)) {
                        // Intervals intersect or touch
                        // std::cout << "Intersect width: " << temp.upper() - temp.lower() << '\n';
                        // std::cout << "Required width: " << num_col_occupied << '\n';
                        new_intersection.push_back(temp);
                    } else {
                    }
                }
            }

            intersection = std::move(new_intersection);
            if (intersection.empty()) break; // No common space, exit early
        }

        // Check the intersection width and update the best position
        for (const auto& intv : intersection) {
            size_t center_col = intv.lower(); // Simplification: Use lower bound as "position"
            size_t center_row = start_row + row_start;
            size_t distance = manhattanDistance(center_row, center_col, best_row, best_col);
            if (distance < min_distance) {
                min_distance = distance;
                best_row = center_row;
                best_col = center_col;
            }
        }
    }
    return {best_row, best_col};

    // stupid version
    // for (size_t row = std::max((size_t) 0, row_min); row < this->placement.num_rows ; ++row) {
    //     for (size_t col = 0 ; col < this->placement.num_cols ; ++col) {
    //         // printf("row: %zu, col: %zu\n", row, col);
    //         // Check if the position can accommodate the merged cell
    //         if (isValidPosition(row, col, banking_cell)) {
    //             size_t dist = manhattanDistance(row, col, desired_position.first, desired_position.second);
    //             if (dist < min_distance) {
    //                 best_row = row;
    //                 best_col = col;
    //                 min_distance = dist;
    //             }
    //             return {best_row, best_col};
    //         }
    //     }
    // }

    // return {best_row, best_col};
}
bool Legalizer::isValidPosition(size_t row, size_t col, const Cell& banking_cell) {
    // Check if the area from (row, col) can fit the merged_cell in placement
    size_t num_row_occupied = this->placement.getCellSiteHeight(banking_cell);
    size_t num_col_occupied = this->placement.getCellSiteWidth(banking_cell);
    // std::cout << banking_cell;
    // std::cout << num_row_occupied << ' ' << num_col_occupied << '\n';
    // Check if the cells in that region are free
    return !this->placement.isOccupied(std::make_pair(row, row+num_row_occupied), std::make_pair(col, col+num_col_occupied));
}

size_t Legalizer::manhattanDistance(size_t row1, size_t col1, size_t row2, size_t col2) {
    return std::abs(row1 - row2) + std::abs(col1 - col2);
}

void Legalizer::writeOutputFile() {
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

void Legalizer::writeDrawFile() {
    std::ofstream draw_file("draw.txt");
    draw_file << std::fixed << std::setprecision(6);
    draw_file << "DieSize " << die_lower_left_x << ' ' << die_lower_left_y << ' ' << die_upper_right_x << ' ' << die_upper_right_y << '\n';
    for (Cell& cell : cells) {
        if (cell.isPlaced) {
            draw_file << cell.name << ' ' << cell.x << ' ' << cell.y << ' ' << cell.width << ' ' << cell.height << ' ' << cell.isFixed << '\n';
        }
    }
}