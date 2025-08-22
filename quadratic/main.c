#include "quadratic/quadratic.h"

int main() {    
    eq_t eq;
    init_eq(&eq);
    
    bool parse_result = io_parse_input(&eq);
    if (!parse_result) return 0;

    uint8_t root_number = calc_roots(&eq);

    io_print_output(&eq);

    return 0;
}

