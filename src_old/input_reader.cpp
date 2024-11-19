#include "input_reader.h"

void read_input(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " [input.lg name] [input.opt name] [output.lg name]\n";
        return;
    }

    char *input_lg = argv[1];  
    char *input_opt = argv[2];
    output_file = argv[3];
    read_legalized_placement(input_lg);
    read_optimizer_step(input_opt);
}
void read_legalized_placement(char *filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }

    std::string line;
    std::cout << "hello\n";
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword == "Alpha") {
            iss >> alpha;
        }
        else if (keyword == "Beta") {
            iss >> beta;
        }
        else if (keyword == "DieSize") {
            iss >> die_lower_left_x >> die_lower_left_y >> die_upper_right_x >> die_upper_right_y;
        }
        else if (keyword == "PlacementRows") {
            double start_x, start_y, site_width, site_height;
            int total_num_of_sites;
            iss >> start_x >> start_y >> site_width >> site_height >> total_num_of_sites;
            placement_rows.emplace_back(start_x, start_y, site_width, site_height, total_num_of_sites);
        }
        else {
            // Cell information
            std::string cell_name = keyword;  // First word is cell name
            double x, y, width, height;
            std::string fix_status;
            if (iss >> x >> y >> width >> height >> fix_status) {
                bool is_fixed = (fix_status == "FIX");
                cells.emplace_back(cell_name, x, y, width, height, is_fixed, true);
                cell_ref[cell_name] = &(cells.back());
            }
        }
    }
    file.close();
}

void read_optimizer_step(char *filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("Banking_Cell:") == 0) {
            size_t arrow_pos = line.find("-->");
            if (arrow_pos == std::string::npos) continue;

            // Extract source cells
            std::string source_part = line.substr(13, arrow_pos - 13); // Get the part before -->
            std::istringstream source_stream(source_part);
            std::vector<std::string> source_cells;
            std::string cell;
            while (source_stream >> cell) {
                source_cells.push_back(cell);
            }

            // Extract merged cell information (name, x, y, width, height)
            std::string merged_part = line.substr(arrow_pos + 4); // Get the part after -->
            std::istringstream merged_stream(merged_part);
            std::string merged_name;
            double x, y, width, height;
            if (merged_stream >> merged_name >> x >> y >> width >> height) {
                banking_cells.emplace_back(source_cells, merged_name, x, y, width, height, false);
            }
        }
    }
    file.close();
}
