#include "globals.h"
#include "input_reader.h"
#include "legalizer.h"

int main(int argc, char *argv[]) {
    read_input(argc, argv);
    // print_globals();
    Legalizer legalizer(placement_rows, cells, banking_cells);
    bool success = legalizer.legalize();
    if (!success) {
        printf("Legalization Fail for output: %s\n", output_filename);
        return 1;
    }
    return 0;
}