#ifndef SMART_PARSE_H
#define SMART_PARSE_H

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

#include "colors.h"

#define MAX_TOKENS 256

/*
    Structure for qudratic equation
    Paraemters:
        a, b, c  - koefs
        root_num - number of roots
        d        - discriminant
        r1, r2   - roots
*/
typedef struct 
{
    double  a, b, c;
    uint8_t root_num;
    double  d;
    double  r1, r2;
    char    to_find;
} eq_t;

/*
    Types of tokens: number, variable, operation, left bracket, right bracket
*/
typedef enum
{
    TOKEN_NUM,
    TOKEN_VAR,
    TOKEN_OP,
    TOKEN_LPART,
    TOKEN_RPART,
} token_types;

/*
    Token struct
    Parameters:
        type  - token_types
        op    - operation
        value - value
*/
typedef struct
{
    token_types type;
    char        op;
    double      value;
} token_t;

/*
    Types of nodes: number, operation, value
*/
typedef enum
{
    NODE_NUM,
    NODE_OP,
    NODE_VAR
} node_types;

/*
    Node type
    Parameters:
        type  - node_types
        value - value
        op    - operation
        left  - left node
        right - righ node
*/
typedef struct tree_node_t
{
    node_types type;
    
    double value;
    char   op;

    struct tree_node_t *left;
    struct tree_node_t *right;
} tree_node_t;

/*
    Operation priorities
*/
typedef enum
{
    PRIOR_HIGHEST = 3,
    PRIOR_HIGH    = 2,
    PRIOR_MEDIUM  = 1,
    PRIOR_LOW     = 0,
} ops_priors;

typedef enum
{
    OK                         = 0,
    ERROR_INCORRECT_FORMATTING = 1,
    ERROR_SCARYY               = 2,
    ERROR_INCORRECT_INPUT      = 3,
} errors;

void init_eq (eq_t *const eq);

/*
    Parse equation input
    Parameters:
        input - input raw string
        eq    - precreated equation to store result
    Output:
        Result code
*/
uint8_t parse_eq_input (const char *const input, eq_t *eq);

/*
    Init token
*/
void init_token (token_t *const token);

/*
    Tokenize input
    Parameters:
        input  - input raw string
        tokens - precreated token array
*/
static int tokenize (const char *input, token_t *tokens);

/*
    Generate tree
    Parameters:
        tokens - array of tokens
        n      - tokens count
*/
static tree_node_t *generate_tree  (token_t *tokens, int n);

/*
    Flatten multiply operation
    Parameters:
        node      - root node
        coeff_out - array of returning coefficients
        power_out - output power of variable
*/
static void flatten_mul(tree_node_t *node, double *coeff_out, int *power_out);

/*
    Collect tree into equation
    Parameters:
        node - root node
        eq   - equation to collect into
        sign - sign of collectable (left part +, right part -)
*/
static void collect (tree_node_t *node, eq_t *eq, int sign);

/*
    Create new node
    Parameters:
        type  - node type
        value - value of node
        op    - operations
*/
static tree_node_t *new_node (node_types type, double value, char op);

/*
    Prior returns priority if operation op
    Parameter:
        op - operation
*/
static int prior (char op);

/*
    Right/left action (+x - left, x^ - right)
    Parameter:
        op - operations
*/
static bool right_action(char op);

/*
    Init token
    Parameters:
        token - ptr to token
        type  - token type
        value - token value
        op    - operation
*/
static void set_token(token_t *token, token_types type, double value, char op);

/*
    Check if full subtree is numeric
    Parameters:
        node - root node
        out  - result of operation after evaling
*/
static bool eval_num_node(tree_node_t *node, double *out);

/*
    Handle digit in tokenizing
    Parameters:
        p      - input char
        n      - tokens count
        tokens - ptr to token array
*/
static const char *handle_digit(const char *p, int *n, token_t *tokens);

/*
    Handle operation in tokenizing
    Parameters:
        p      - input char
        n      - tokens count
        tokens - ptr to token array
*/
static void handle_ops(const char *p, int *n, token_t *tokens);

/*
    Init and create operation node
    Parameter:
        node_ptr    - ptr to node in array
        nodes_stack - nodes array (stack)
        top         - top stack operation 
*/
static tree_node_t *create_default_op_node(int *node_ptr, tree_node_t **nodes_stack, char top);

#endif
