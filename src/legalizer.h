#ifndef LEGALIZER_H
#define LEGALIZER_H

#include "globals.h"
#include "placement.h"

class Legalizer {
public:
    std::vector<Cell> cells;
    std::unordered_map<std::string, Cell*> cell_ref;
    Placement placement;
    Legalizer();
    Legalizer(const std::vector<PlacementRow>& placement_rows_input,
              const std::vector<Cell>& cells_input);
    ~Legalizer();

    void legalize(std::vector<BankingCell>& banking_cells);

    void insertCellToPlacement(const Cell& cell, int site_start, int site_end, double y0, double y1);
    std::pair<int, int> findPlacement(const Cell& cell) const; 
    void removeCellFromPlacement(const Cell& cell);
};


#endif
