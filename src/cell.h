#ifndef CELL_H
#define CELL_H

#include "globals.h"

class Cell {
public:
    std::string name;
    double x, y; // Positions
    double width, height; // Dimensions
    size_t site_width, site_height;
    std::pair<size_t, size_t> rows, cols;
    bool isFixed;
    bool isPlaced;

    // Regular constructor
    Cell() {}
    Cell(const std::string& n, double x_, double y_, double w, double h, bool fix, bool isPlaced = false)
        : name(n), x(x_), y(y_), width(w), height(h), isFixed(fix), isPlaced(false) {}

    // Copy constructor
    Cell(const Cell& other)
        : name(other.name), x(other.x), y(other.y), width(other.width), height(other.height),
          isFixed(other.isFixed), isPlaced(other.isPlaced) {}
    // Overload the << operator
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell) {
        os << "Name: " << cell.name
           << ", Position: (" << cell.x << ", " << cell.y << ")"
           << ", Size: (" << cell.width << ", " << cell.height << ")"
           << ", Fixed: " << (cell.isFixed ? "Yes" : "No");
        return os;
    }
};

class BankingCell : public Cell {
public:
    std::vector<std::string> source_cell_names;

    BankingCell(const std::vector<std::string>& src, const std::string& merged,
                double x_, double y_, double w, double h, bool fix)
        : Cell(merged, x_, y_, w, h, fix), source_cell_names(src) {}
    BankingCell(const BankingCell& other)
        : Cell(other), source_cell_names(other.source_cell_names) {}
    // Overload the << operator for BankingCell
    friend std::ostream& operator<<(std::ostream& os, const BankingCell& banking_cell) {
        os << "Source Cells: ";
        for (const auto& source_cell : banking_cell.source_cell_names) {
            os << source_cell << " ";
        }
        os << "\nMerged Cell Name: " << banking_cell.name
           << ", Position: (" << banking_cell.x << ", " << banking_cell.y << ")"
           << ", Size: (" << banking_cell.width << ", " << banking_cell.height << ")";
        return os;
    }
};

#endif