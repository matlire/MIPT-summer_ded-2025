#include "smart_parse.h"

void init_token (token_t *const token)
{
    token->type  = 0;
    token->op    = 0;
    token->value = 0.0;
}

void init_eq (eq_t *const eq)
{
    log_printf(DEBUG, "Initing EQ");
    eq->a = 0.0;
    eq->b = 0.0;
    eq->c = 0.0;

    eq->root_num = 0;
    eq->d        = 0.0;
    eq->r1       = 0.0;
    eq->r2       = 0.0;
    log_printf(DEBUG, "Inited EQ");
}

static tree_node_t *new_node(node_types type, double value, char op)
{
    log_printf(DEBUG, "Initing node");
    tree_node_t *node = malloc(sizeof(tree_node_t));
    if (!node) { log_printf(ERROR, "Not inited node with type=%d value=%lf op=%c", type, value, op); return NULL; }
    node->type  = type;
    node->value = value;
    node->op    = op;
    node->left  = NULL;
    node->right = NULL;
    log_printf(DEBUG, "Inited Node");

    return node;
}

static int prior(const char op)
{
    log_printf(DEBUG, "Calculating priority for %c", op);
    switch (op)
    {
        case '^': return PRIOR_HIGHEST;
        case '*': case '/': return PRIOR_HIGH;
        case '+': case '-': return PRIOR_MEDIUM;
        default: return PRIOR_LOW;
    }
}

static bool right_action(const char op)
{
    log_printf(DEBUG, "Right action for %c", op);
    return (op == '^');
}

static void set_token(token_t *const token, const token_types type, const double value, const char op)
{
    log_printf(DEBUG, "Setting token with type=%d value=%.5lf op=%c", type, value, op);
    token->type  = type;
    token->value = value;
    token->op    = op;
}

static const char *handle_digit(const char *p, int *const n, token_t *const tokens)
{
    log_printf(DEBUG, "Handling digit with p=%s n=%d", p, *n);
    char *end = NULL;
    double val = strtod(p, &end);
    set_token(&tokens[*n], TOKEN_NUM, val, 0);
    (*n)++;

    if (isalpha((unsigned char)*end) || *end == '(')
    {
        log_printf(DEBUG, "Alpha or ( found");
        set_token(&tokens[*n], TOKEN_OP, 0.0, '*');
        (*n)++;
    }

    return end;
}

static void handle_ops(const char *p, int *const n, token_t *const tokens)
{
    char cur = *p;

    bool prev_is_op_or_lpar = ((*n) == 0) ||
    (tokens[(*n)-1].type == TOKEN_OP) ||
    (tokens[(*n)-1].type == TOKEN_LPART);

    log_printf(DEBUG, "Handling ops with char=%c and prev_is_op_or_lpar=%d", cur, prev_is_op_or_lpar);

    if ((cur == '+' || cur == '-') && prev_is_op_or_lpar)
    {
        if ((*n) < MAX_TOKENS - 1) {
            set_token(&tokens[*n], TOKEN_NUM, 0.0, 0);
            (*n)++;
        }
    }

    set_token(&tokens[*n], TOKEN_OP, 0.0, cur);
}

static int tokenize(const char *input, token_t *const tokens)
{
    int n = 0;
    const char *p = input;

    log_printf(DEBUG, "Tokenizing with input=%s", input);

    while (*p && n < MAX_TOKENS)
    {
        log_printf(DEBUG, "Iterating with p=%c and n=%d", *p, n);
        if (isdigit((unsigned char)*p) || (*p == '.' && isdigit((unsigned char)*(p+1))))
        {
            p = handle_digit(p, &n, tokens);
        } else if (isalpha((unsigned char)*p))
        {
            set_token(&tokens[n], TOKEN_VAR, 0.0, *p);
            n++; p++;

            if (isalpha((unsigned char)*p) || isdigit((unsigned char)*p) || *p == '(')
            {
                set_token(&tokens[n], TOKEN_OP, 0.0, '*');
                n++;
            }
        } else if (strchr("+-*/^", *p))
        {
            handle_ops(p, &n, tokens);
            n++; p++; 
        } else if (*p == '(')
        {
            set_token(&tokens[n], TOKEN_LPART, 0.0, '(');
            n++; p++;
        } else if (*p == ')')
        {
            set_token(&tokens[n], TOKEN_RPART, 0.0, ')');
            n++; p++;
            
            if (isalpha((unsigned char)*p) || isdigit((unsigned char)*p) || *p == '(')
            {
                set_token(&tokens[n], TOKEN_OP, 0.0, '*');
                n++;
            }
        } else p++;
    }

    return n;
}

static tree_node_t *create_default_op_node(int *const node_ptr, tree_node_t **const nodes_stack, const char top)
{
    log_printf(DEBUG, "Creating default operation node");
    if ((*node_ptr) < 2) return NULL;

    tree_node_t *r = nodes_stack[--(*node_ptr)];
    tree_node_t *l = nodes_stack[--(*node_ptr)];
    tree_node_t *node = new_node(NODE_OP, 0, top);

    node->left = l;
    node->right = r;
    nodes_stack[(*node_ptr)++] = node;
    
    return node;
}

static void parse_tokens(token_t *const tokens, int *const n, tree_node_t **const nodes_stack, char *const op_stack, int *const node_ptr, int *const op_ptr)
{
    for (int i = 0; i < (*n); ++i)
    {
        token_t token = tokens[i];
        log_printf(DEBUG, "Parsing token with type=%d value=%.5lf op=%c", token.type, token.value, token.op);
        if (token.type      == TOKEN_NUM) nodes_stack[(*node_ptr)++] = new_node(NODE_NUM, token.value, 0);
        else if (token.type == TOKEN_VAR) nodes_stack[(*node_ptr)++] = new_node(NODE_VAR, 0, token.op);
        else if (token.type == TOKEN_OP)
        {
            while ((*op_ptr) > 0 && op_stack[(*op_ptr)-1] != '(')
            {
                char top = op_stack[(*op_ptr) - 1];
                int ptop = prior(top);
                int pcur = prior(token.op);
                if ((ptop > pcur) || (ptop == pcur && !right_action(token.op)))
                {
                    (*op_ptr)--;
                    create_default_op_node(node_ptr, nodes_stack, top);
                }
                else break;
            }
            op_stack[(*op_ptr)++] = token.op;
        }
        else if (token.type == TOKEN_LPART) op_stack[(*op_ptr)++] = '(';
        else if (token.type == TOKEN_RPART)
        {
            while ((*op_ptr) > 0 && op_stack[(*op_ptr) - 1] != '(')
            {
                char top = op_stack[--(*op_ptr)];
                create_default_op_node(node_ptr, nodes_stack, top);
            }
            if ((*op_ptr) > 0 && op_stack[(*op_ptr) - 1] == '(') (*op_ptr)--;
        }
    }
}

static tree_node_t *generate_tree(const token_t *const tokens, const int n)
{
    log_printf(DEBUG, "Generating tree...");
    if (n == 0) return NULL;

    tree_node_t *nodes_stack[MAX_TOKENS];
    char op_stack[MAX_TOKENS];

    int node_ptr = 0;
    int op_ptr = 0;

    parse_tokens(tokens, &n, nodes_stack, op_stack, &node_ptr, &op_ptr);
    
    while (op_ptr > 0)
    {
        char top = op_stack[--op_ptr];
        create_default_op_node(&node_ptr, nodes_stack, top);
    }

    if (node_ptr != 1) { log_printf(ERROR, "Error generating tree! n=%d", n); return NULL; }

    log_printf(DEBUG, "Tree generated!");
    return nodes_stack[0];
}

static bool eval_num_node(const tree_node_t *node, double *const out)
{
    log_printf(DEBUG, "Evaling num node with type=%d value=%.5lf op=%c", node->type, node->value, node->op);

    if (!node) return false;
    if (node->type == NODE_NUM) { *out = node->value; return true; }
    if (node->type == NODE_VAR) return false;

    double l = 0.0, r = 0.0;
    if (!eval_num_node(node->left, &l)) return false;
    if (!eval_num_node(node->right, &r)) return false;

    switch (node->op) {
        case '+': *out = l + r; return true;
        case '-': *out = l - r; return true;
        case '*': *out = l * r; return true;
        case '/': if (r == 0.0) return false; *out = l / r; return true;
        case '^': *out = pow(l, r); return true;
        default: return false;
    }
}

static void flatten_mul(const tree_node_t *node, double *const coeff_out, int *const power_out)
{
    if (!node) { log_printf(ERROR, "No node for flattening!"); return; }
    double v = 0;
    if (eval_num_node(node, &v))
    {
        *coeff_out *= v;
        return;
    }
    if (node->type == NODE_NUM)
    {
        *coeff_out *= node->value;
    }
    else if (node->type == NODE_VAR)
    {
        *power_out += 1;

    }
    else if (node->type == NODE_OP)
    {
        if (node->op == '*')
        {
            flatten_mul(node->left, coeff_out, power_out);
            flatten_mul(node->right, coeff_out, power_out);
        }
        else if (node->op == '^')
        {
            if (node->left && node->left->type == NODE_VAR && node->right && node->right->type == NODE_NUM)
            {
                int p = (int) node->right->value;
                *power_out += p;

            }
            else ;
        }
        else ;
    }
    log_printf(DEBUG, "Done flattening with coeff_out=%.5lf power_out=%d", *coeff_out, *power_out);
}

static void collect(const tree_node_t *node, eq_t *const eq, const int sign)
{
    if (!node || !eq) { log_printf(ERROR, "No node or no eq for collecting!"); return; }

    log_printf(DEBUG, "Collecting for node with type=%d value=%.5lf op=%c", node->type, node->value, node->op);

    if (node->type == NODE_VAR)
    {
        eq->to_find = node->op;
    }

    if      (node->type == NODE_NUM) eq->c += sign * node->value;
    else if (node->type == NODE_VAR) { eq->b += sign * 1.0; eq->to_find = node->op; }
    else if (node->type == NODE_OP)
    {
        if (node->op == '+')
        {
            collect(node->left, eq, sign);
            collect(node->right, eq, sign);
        }
        else if (node->op == '-')
        {
            collect(node->left, eq, sign);
            collect(node->right, eq, -sign);
        }
        else if (node->op == '*')
        {
            double coeff = 1.0;
            int power = 0;
            flatten_mul(node, &coeff, &power);
            if (power == 2)      eq->a += sign * coeff;
            else if (power == 1) eq->b += sign * coeff;
            else if (power == 0) eq->c += sign * coeff;
        }
        else if (node->op == '^')
        {
            if (node->left && node->left->type == NODE_VAR && node->right && node->right->type == NODE_NUM)
            {
                eq->to_find = node->left->op;
                int p = (int)node->right->value;
                if (p == 2)      eq->a += sign * 1.0;
                else if (p == 1) eq->b += sign * 1.0;
            }
        }
    }
    log_printf(DEBUG, "Done collecting with eq_a=%.5lf eq_b=%.5lf eq_c=%.5lf eq_to_find=%c", \
                        eq->a, eq->b, eq->c, eq->to_find);
}

static void free_tree(tree_node_t *node)
{
    if (!node) {log_printf(WARN, "No node to free tree!"); return; }
    log_printf(DEBUG, "Freeing tree...");
    free_tree(node->left);
    free_tree(node->right);
    free(node);
    log_printf(DEBUG, "Tree freed!");
}

uint8_t parse_eq_input(const char *input, eq_t *eq)
{
    if (!input || !eq) return ERROR_SCARYY;

    log_printf(INFO, "Parsing eq input...");

    char *buf = strdup(input);
    if (!buf) return ERROR_SCARYY;
    
    int c = 0;
    char *p = buf;
    while (*p)
    {
        if (*p == '=') c++;
        *p++;
    }
    if (c != 1) return ERROR_INCORRECT_FORMATTING;

    char *lh = strtok(buf, "=");
    char *rh = strtok(NULL, "=");
    if (!lh || !rh)
    {
        free(buf);
        return ERROR_INCORRECT_FORMATTING;
    }

    token_t tokens[MAX_TOKENS];

    for (int i = 0; i < MAX_TOKENS; i++)
    {
        init_token(&tokens[i]);
    }

    int n1 = tokenize(lh, tokens);
    tree_node_t *node_1 = generate_tree(tokens, n1);
    if (node_1) collect(node_1, eq, +1);

    free_tree(node_1);

    int n2 = tokenize(rh, tokens);
    tree_node_t *node_2 = generate_tree(tokens, n2);
    if (node_2) collect(node_2, eq, -1);

    free_tree(node_2);
    free(buf);

    log_printf(INFO, "Parsed eq input with a=%.5lf b=%.5lf c=%.5lf and to_find=%c", \
                        eq->a, eq->b, eq->c, eq->to_find);
    
    return OK;
}
