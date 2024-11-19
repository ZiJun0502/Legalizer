#include "globals.h"
#include "input_reader.h"
#include "legalizer.h"

int main(int argc, char *argv[]) {
    read_input(argc, argv);
    // print_globals();
    Legalizer legalizer(placement_rows, cells);
    legalizer.legalize(banking_cells);
}