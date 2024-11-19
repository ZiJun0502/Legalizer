#ifndef LEGALIZER_H
#define LEGALIZER_H

#include "globals.h"
#include "placement.h"

class Legalizer {
public:
    struct outputInfo;
    std::vector<Cell> cells;
    std::vector<BankingCell> banking_cells;
    std::vector<outputInfo> outputInfos;
    std::unordered_map<std::string, Cell*> cell_ref;
    Placement placement;
    size_t num_cell_in_placement;

    Legalizer();
    Legalizer(const std::vector<PlacementRow>& placement_rows_input,
              const std::vector<Cell>& cells_input,
              const std::vector<BankingCell>& banking_cells);
    ~Legalizer();

    bool legalize();

    void insertCellToPlacement(Cell& cell, std::pair<size_t, size_t> rows, std::pair<size_t, size_t> cols);
    // std::pair<int, int> findPlacement(const Cell& cell) const; 
    void removeCellFromPlacement(Cell& cell);

std::pair<size_t, size_t> __findValidPosition(const std::pair<size_t, size_t>& desired_position,
                                                         const Cell& banking_cell, 
                                                         long long row_start, 
                                                         long long row_end, 
                                                         std::unordered_set<long long>& visited,
                                                         std::unordered_map<long long, std::vector<Interval>>& available_intervals);
    std::pair<size_t, size_t> findValidPosition(const std::pair<size_t, size_t>& desired_position, const Cell& merged_cell);
    bool isValidPosition(size_t row, size_t col, const Cell& merged_cell);
    size_t manhattanDistance(size_t row1, size_t col1, size_t row2, size_t col2);
    void writeOutputFile();
    void writeDrawFile(std::string draw_filename);

    // this class is used to store output
    struct outputInfo {
        double x, y;
        size_t num_moved_cell;
        std::vector<std::string> moved_cell_name;
        std::vector<std::pair<double, double>> moved_cell_coord;
        outputInfo() {}
        outputInfo(std::pair<double, double> coord) {
            x = coord.first, y = coord.second;
            num_moved_cell = 0;
        }
    };
};


#endif
