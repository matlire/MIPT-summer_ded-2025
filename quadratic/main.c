#include "quadratic/quadratic.h"
#include "test/test_core.h"

int main() {
    run_all_tests();

    // Create equation struct
    eq_t eq;
    init_eq(&eq);
    
    while (true)
    {
        // Parse user input
        bool parse_result = io_parse_input(&eq);
        if (!parse_result) return 0;

        // Calculate root
        uint8_t root_number = quadratic_calc_roots(&eq);
    
        // Print formatted result
        io_print_output(&eq);
    }

    return 0;
}

