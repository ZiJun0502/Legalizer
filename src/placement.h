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

    Placement();
    Placement(const std::vector<PlacementRow>& placement_rows);
    ~Placement();

    size_t getCellRow(Cell& cell);
    size_t getCellCol(Cell& cell);
    void insertCell(Cell& cell, bool no_collision);
    void printPlacementRows() const;
};
#endif