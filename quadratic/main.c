#include "quadratic.h"
#include "test_core.h"
#include "parameter_parser.h"

int main(int argc, char *argv[]) {
    // Parse and execute flags
    if (!parameters_parse(argc, argv))
    {
        print_clear_formatting();
        print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "Some error during arguments parsing.\n\nCheck usage using --help!\n");
        return 0;
    }

    print_clear_formatting();

    // Test quadratic function
    run_all_tests();

    // Create equation struct
    eq_t eq; 
   
    // Main loop
    while (true)
    {
        init_eq(&eq);
        // Parse user input
        uint8_t parse_result = quadratic_parse_input(&eq);
        if (parse_result != OK) continue;

        // Calculate root
        uint8_t root_number = quadratic_calc_roots(&eq);
    
        // Print formatted result
        quadratic_print_output(&eq, stdout);
    }

    return 0;
}

