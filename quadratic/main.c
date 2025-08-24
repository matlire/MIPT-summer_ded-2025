#include "quadratic.h"
#include "test_core.h"
#include "parameter_parser.h"

int main(int argc, char *argv[]) {
    // Parse and execute flags
    if (!parameters_parse(argc, argv))
    {
        print_clear_formatting();
        print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "Some error during arguments parsing.\n\nUsage: --input_file input.csv");
        return 0;
    }

    print_clear_formatting();

    // Test quadratic function
    run_all_tests();

    // Create equation struct
    eq_t eq;
    init_eq(&eq);
   
    // Main loop 
    while (true)
    {
        // Parse user input
        bool parse_result = quadratic_parse_input(&eq);
        if (!parse_result) return 0;

        // Calculate root
        uint8_t root_number = quadratic_calc_roots(&eq);
    
        // Print formatted result
        quadratic_print_output(&eq, stdout);
    }

    return 0;
}

