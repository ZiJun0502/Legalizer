#ifndef INPUT_READER_H
#define INPUT_READER_H

#include "globals.h"

// Function to read the input files
void read_legalized_placement(char *filename);
void read_optimizer_step(char *filename);
void read_input(int argc, char* argv[]);

#endif // INPUT_READER_H