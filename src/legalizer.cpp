#include "legalizer.h"

Legalizer::Legalizer() {

}
Legalizer::Legalizer(const std::vector<PlacementRow>& placement_rows,
                     const std::vector<Cell>& cells_input): Legalizer() {
    // Copying the data into the class members
    this->placement = Placement(placement_rows);
    this->cells = cells_input;

    // Build cell_ref map for quick lookup by name
    for (auto& cell : cells) {
        cell_ref[cell.name] = &cell;
        this->placement.insertCell(cell, true);
    }


}
Legalizer::~Legalizer() {

}
// Removes a cell from the placement (freeing the space it occupied)
void Legalizer::removeCellFromPlacement(const Cell& cell) {
    // Calculate the site range the cell occupied
    int start_site = static_cast<int>(cell.x / site_width);
    int end_site = static_cast<int>((cell.x + cell.width) / site_width);

    Interval occupied(start_site, end_site);
}

// Finds the nearest available space for a cell to be placed
std::pair<int, int> Legalizer::findPlacement(const Cell& cell) const {
    // Find the closest row that can accommodate the cell's width (in terms of sites)
    for (const auto& row : placement_rows) {
    }

    throw std::runtime_error("No suitable placement found for cell: " + cell.name);
}

// Places the cell at the given site range and converts to coordinates
void Legalizer::insertCellToPlacement(const Cell& cell, int site_start, int site_end, double y0, double y1) {
    // Convert the site range into physical coordinates
    double x0 = site_start * site_width;
    double x1 = site_end * site_width;

    // Reserve the space by marking it as occupied
    Interval occupied(site_start, site_end);

    // Update the cell's position
    const_cast<Cell&>(cell).x = x0;
    const_cast<Cell&>(cell).y = y0;
    const_cast<Cell&>(cell).isPlaced = true;

    // Here, you would update the row's free intervals to reflect that the space is now occupied
}

// Main method to legalize placement of cells
void Legalizer::legalize(std::vector<BankingCell>& banking_cells) {
    for (auto& cell : cells) {
            // try {
            //     auto [site, y] = findPlacement(cell);
            //     int num_sites_needed = static_cast<int>(cell.width / site_width);
            //     insertCellToPlacement(cell, site, site + num_sites_needed, y, y + cell.height);
            // } catch (const std::runtime_error& e) {
            //     std::cerr << "Error legalizing cell " << cell.name << ": " << e.what() << std::endl;
            // }
    }
}