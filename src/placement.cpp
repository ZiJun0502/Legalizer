#include "placement.h"


Placement::Placement() {}
Placement::Placement(const std::vector<PlacementRow>& rows) {
    this->placement_rows = rows;
    this->site_width = rows[0].site_width;
    this->site_height = rows[0].site_height;
    this->start_x = rows[0].start_x;
    this->start_y = rows[0].start_y;
    printf("Num of placement row: %d\n", this->placement_rows.size());
    printf("Placement init finish");
    for (auto placement_row : this->placement_rows) {
        std::cout << *(placement_row.free_intervals.begin());
    }

}
Placement::~Placement() {}

size_t Placement::getCellRow(Cell& cell) {
    return ((cell.y - this->start_y) / this->site_height);
}
size_t Placement::getCellCol(Cell& cell) {
    return ((cell.x - this->start_x) / this->site_width);
}
void Placement::insertCell(Cell& cell, bool no_collision) {
    size_t cell_row = getCellRow(cell);
    size_t cell_col = getCellCol(cell);
    if (no_collision) {
        // insert directly
        // printf("Inserting at cell row: %zu, cell col: %zu, cell: ", cell_row, cell_col);
        // std::cout << cell << '\n';
        this->placement_rows[cell_row].insert(cell);
    } else {
        // search for space
        
    }
}

void Placement::printPlacementRows() const {
    for (const auto& row : placement_rows) {
        std::cout << row;
    }
}
