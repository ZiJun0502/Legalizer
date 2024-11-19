#ifndef PLACEMENT_H
#define PLACEMENT_H

#include "globals.h"
#include "placement_row.h"
#include "cell.h"
class Placement {
public:
    std::vector<PlacementRow> placement_rows;
    double site_width, site_height;
    double start_x, start_y;
    size_t num_rows, num_cols;

    Placement();
    Placement(const std::vector<PlacementRow>& placement_rows);
    ~Placement();

    size_t getCellSiteHeight(const Cell& cell);
    size_t getCellSiteWidth(const Cell& cell);
    std::pair<size_t, size_t> getCellRows(const Cell& cell);
    std::pair<size_t, size_t> getCellCols(const Cell& cell);
    std::pair<size_t, size_t> getYRows(double y, double height);
    std::pair<size_t, size_t> getXCols(double x, double width);
    std::pair<double, double> coordFromSiteInd(size_t row, size_t col);
    std::pair<size_t, size_t> siteIndFromCoord(double x, double y);
    bool isOccupied(std::pair<size_t, size_t> rows, std::pair<size_t, size_t> cols);
    std::pair<size_t, size_t> findFreeSpace(Cell& cell);
    void insert(std::pair<size_t, size_t> rows, std::pair<size_t, size_t> cols);
    void remove(std::pair<size_t, size_t> rows, std::pair<size_t, size_t> cols);
    // void insertCell(Cell& cell, bool no_collision);
    // void removeCell(Cell& cell);
    void printPlacementRows() const;
};
#endif