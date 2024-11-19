#include "placement.h"


Placement::Placement() {}
Placement::Placement(const std::vector<PlacementRow>& rows) {
    this->placement_rows = rows;
    this->site_width = rows[0].site_width;
    this->site_height = rows[0].site_height;
    this->start_x = rows[0].start_x;
    this->start_y = rows[0].start_y;
    this->num_rows = rows.size();
    this->num_cols = rows[0].total_num_of_sites;
    // printf("Num of placement row: %d\n", this->placement_rows.size());
    // printf("Placement init finish");
    // for (auto placement_row : this->placement_rows) {
    //     std::cout << *(placement_row.free_intervals.begin());
    // }

}
Placement::~Placement() {}

size_t Placement::getCellSiteHeight(const Cell& cell) {
    std::pair<size_t, size_t> CellRows = getCellRows(cell);
    // std::cout << "Banking cell: ";
    // std::cout << CellCols.first << ' ' <<CellCols.second << '\n';
    return CellRows.second - CellRows.first;
}
size_t Placement::getCellSiteWidth(const Cell& cell) {
    std::pair<size_t, size_t> CellCols = getCellCols(cell);
    return CellCols.second - CellCols.first;
}
std::pair<size_t, size_t> Placement::getCellRows(const Cell& cell) {
    return getYRows(cell.y, cell.height);
}
std::pair<size_t, size_t> Placement::getCellCols(const Cell& cell) {
    return getXCols(cell.x, cell.width);
}
std::pair<size_t, size_t> Placement::getYRows(double y, double height) {
    size_t row_end = (y + height - this->start_y) / this->site_height;
    size_t row_start = (y - this->start_y) / this->site_height;
    return std::make_pair(row_start, row_end);
}
std::pair<size_t, size_t> Placement::getXCols(double x, double width) {
    size_t col_end = std::ceil((x + width - this->start_x) / this->site_width);
    size_t col_start = (x - this->start_x) / this->site_width;
    return std::make_pair(col_start, col_end);
}
std::pair<double, double> Placement::coordFromSiteInd(size_t row, size_t col) {
    return std::make_pair(col * site_width + start_x, row * site_height + start_y);
}
std::pair<size_t, size_t> Placement::siteIndFromCoord(double x, double y) {
    return std::make_pair((y - this->start_y) / this->site_height, (x - this->start_x) / this->site_width);
}
bool Placement::isOccupied(std::pair<size_t, size_t> rows, std::pair<size_t, size_t> cols) {
    // std::cout << rows.first << ' ' << rows.second << '\n';
    for (size_t row = rows.first; row < rows.second; ++row) {
        // printf("row: %zu\n", row);
        // printf("Num row: %d\n", this->placement_rows.size());
        if (this->placement_rows[row].isOccupied(cols)) {
            return true; 
        }
    }
    return false;  
}
void Placement::insert(std::pair<size_t, size_t> rows, std::pair<size_t, size_t> cols) {
    for (size_t row = rows.first ; row < rows.second ; row++) {
        this->placement_rows[row].insert(Interval(cols.first, cols.second));
    }
}
void Placement::remove(std::pair<size_t, size_t> rows, std::pair<size_t, size_t> cols) {
    for (size_t row = rows.first ; row < rows.second ; row++) {
        this->placement_rows[row].remove(Interval(cols.first, cols.second));
    }
}
void Placement::printPlacementRows() const {
    for (const auto& row : placement_rows) {
        std::cout << row << "\n";
    }
}
