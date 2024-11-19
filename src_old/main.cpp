// #include "globals.h"

// int main(int argc, char *argv[]) {
//     read_input(argc, argv);
//     // print_globals();
//     // Legalizer legalizer;
//     // legalizer.legalize(placement_rows, cells, banking_cells);
// }
#include "globals.h"
#include <iostream>
#include <vector>
#include "RTree.h"

// Assume Site and Cell have basic structure like below
struct Site {
    double x, y, width, height;

    Site(double x, double y, double width, double height)
        : x(x), y(y), width(width), height(height) {}
};

struct Cell {
    double x, y, width, height;

    Cell(double x, double y, double width, double height)
        : x(x), y(y), width(width), height(height) {}
};

// R-tree Node Structure for Site
struct SiteNode {
    double x, y, width, height;

    // Constructor to make it compatible with R-tree
    SiteNode(double x, double y, double width, double height)
        : x(x), y(y), width(width), height(height) {}

    // Bounding box for the R-tree: it is the space occupied by the site
    std::pair<double, double> getBoundingBox() const {
        return {x + width, y + height};  // Top-right corner
    }
};

int main(int argc, char *argv[]) {
    // Example input (could be customized based on input files)
    double site_width = 1.0;
    double site_height = 10.0;
    
    std::vector<Cell> cells = {
        Cell(5.0, 5.0, 2.0, 2.0),  // Cell at (5, 5) with width 2 and height 2
        Cell(15.0, 5.0, 3.0, 3.0)  // Cell at (15, 5) with width 3 and height 3
    };

    // Initialize the R-tree for storing available spaces
    RTree<SiteNode> rtree;

    // Insert available space into R-tree (assume these are available areas)
    rtree.insert(SiteNode(0.0, 0.0, 5.0, site_height));  // Space from (0, 0) to (5, 10)
    rtree.insert(SiteNode(10.0, 0.0, 5.0, site_height)); // Space from (10, 0) to (15, 10)

    // Print available space before placement
    std::cout << "Available spaces before placing cells:" << std::endl;
    for (const auto& site : rtree) {
        std::cout << "Available space: (" << site.x << ", " << site.y 
                  << ") to (" << site.x + site.width << ", " << site.y + site.height << ")" << std::endl;
    }

    // Place cells and remove corresponding space from R-tree
    for (auto& cell : cells) {
        std::cout << "Placing cell at (" << cell.x << ", " << cell.y 
                  << ") with width " << cell.width << " and height " << cell.height << std::endl;

        // Remove corresponding space from R-tree (this assumes cells are placed in free spaces)
        rtree.remove(SiteNode(cell.x, cell.y, cell.width, cell.height));

        // After placing the cell, update available space (simplified for this example)
        // Ideally, you will need to merge or update the R-tree to account for new empty space
    }

    // Print available space after placement
    std::cout << "Available spaces after placing cells:" << std::endl;
    for (const auto& site : rtree) {
        std::cout << "Available space: (" << site.x << ", " << site.y 
                  << ") to (" << site.x + site.width << ", " << site.y + site.height << ")" << std::endl;
    }

    return 0;
}
