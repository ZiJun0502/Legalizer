#ifndef GLOBALS_H
#define GLOBALS_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility> 
#include <unordered_map>
#include <fstream>
#include <cfloat>
#include <cstdio>
#include <cassert>
#include <sstream>
#include <boost/numeric/interval.hpp>


// Global variables definition
extern double alpha; // Changed from size_t to double
extern double beta; // Changed from size_t to double
extern double die_lower_left_x, die_lower_left_y, die_upper_right_x, die_upper_right_y;
extern double placement_row_start_x, placement_row_start_y, placement_row_start_x;
extern double site_width, site_height;

#include "placement_row.h"
#include "placement.h"
#include "cell.h"

extern std::vector<Cell> cells;
extern std::vector<PlacementRow> placement_rows;
extern std::vector<BankingCell> banking_cells;
extern std::unordered_map<std::string, Cell*> cell_ref;
extern char *output_filename;
void print_globals();

#endif
