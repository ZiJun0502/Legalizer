#ifndef GLOBALS_H
#define GLOBALS_H
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility> 
#include <unordered_map>
#include <fstream>
#include <cfloat>
#include <sstream>
#include <boost/numeric/interval.hpp>

#include "input_reader.h"
#include "legalizer.h"

// Global variables definition
using namespace boost::numeric;
using Interval = interval<size_t>;

class Cell {
public:
    std::string name;
    double x, y; // Positions
    double width, height; // Dimensions
    bool isFixed;
    bool isPlaced;

    Cell(const std::string& n, double x_, double y_, double w, double h, bool fix, bool isPlaced = false) 
        : name(n), x(x_), y(y_), width(w), height(h), isFixed(fix), isPlaced(isPlaced) {}
};

class PlacementRow {
public:
    double startX, startY; // Positions
    double siteWidth, siteHeight; // Dimensions
    int totalNumOfSites;

    PlacementRow(double sx, double sy, double sw, double sh, int sites)
        : startX(sx), startY(sy), siteWidth(sw), siteHeight(sh), totalNumOfSites(sites) {}
};

// BankingCell now inherits from Cell
class BankingCell : public Cell {
public:
    std::vector<std::string> source_cell_names;

    BankingCell(const std::vector<std::string>& src, const std::string& merged,
                double x_, double y_, double w, double h, bool fix)
        : Cell(merged, x_, y_, w, h, fix), source_cell_names(src) {}
};


extern double alpha; // Changed from size_t to double
extern double beta; // Changed from size_t to double
extern double die_lower_left_x, die_lower_left_y, die_upper_right_x, die_upper_right_y; // Changed from size_t to double
extern double placement_row_start_x, placement_row_start_y, placement_row_start_x;
extern double site_width, site_height;
extern std::vector<Cell> cells;
extern std::vector<PlacementRow> placement_rows;
extern std::vector<BankingCell> banking_cells;
extern std::unordered_map<std::string, Cell*> cell_ref;
extern char *output_filename;
void print_globals();

#endif
