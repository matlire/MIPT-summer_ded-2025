#include "quadratic/quadratic.h"

int main() {
    // Create equation struct
    eq_t eq;
    init_eq(&eq);
    
    // Parse user input
    bool parse_result = io_parse_input(&eq);
    if (!parse_result) return 0;

    // Calculate root
    uint8_t root_number = calc_roots(&eq);
    
    // Print formatted result
    io_print_output(&eq);

    return 0;
}

