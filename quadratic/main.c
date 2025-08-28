#include "quadratic.h"
#include "test_core.h"
#include "parameter_parser.h"
#include "logging.h"

int main(int argc, char *argv[]) {
    // Start logging
    FILE *log_file = load_file("log.log", "a");
    init_logging(log_file, DEBUG);

    log_printf(INFO, "\n\n---STARTING SOLVER---");

    // Parse and execute flags
    switch (parameters_parse(argc, argv))
    {
        case OK_EXIT: return 0;
        case OK_CONT: break;
        case PERROR:  default: 
            print_clear_formatting();
            print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, \
                          "Some error during arguments parsing.\n\nCheck usage using --help!\n");
            log_printf(FATAL, "Some error during argument parsing.");
            return 1;
    }

    print_clear_formatting(); 

    // Test quadratic function
    log_printf(INFO, "Running tests...");
    print_colored(COLOR_FORE_BLACK, COLOR_BACK_YELLOW, "RUNNING TESTS...\n");
    int failed = run_all_tests();
    if (failed)
    {
        log_printf(FATAL, "Tests failed!");
        print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "TESTS FAILED!");
        return 1;
    }
    log_printf(INFO, "Tests OK");
    print_colored(COLOR_FORE_WHITE, COLOR_BACK_GREEN, "OK\n\n");

    // Create equation struct
    eq_t eq = { }; 
   
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

    close_log_file();

    return 0;
}

