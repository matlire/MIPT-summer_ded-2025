#include "smart_parse.h"

void init_token (token_t *const token)
{
    token->type  = 0;
    token->op    = 0;
    token->value = 0.0;
}

static tree_node_t *new_node(node_types type, double value, char op)
{
    tree_node_t *node = malloc(sizeof(tree_node_t));
    if (!node) return NULL;
    node->type = type;
    node->value = value;
    node->op = op;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static int prior(char op)
{
    switch (op)
    {
        case '^': return PRIOR_HIGHEST;
        case '*': case '/': return PRIOR_HIGH;
        case '+': case '-': return PRIOR_MEDIUM;
        default: return PRIOR_LOW;
    }
}

static bool right_action(char op)
{
    return (op == '^');
}

static void set_token(token_t *token, token_types type, double value, char op)
{
    token->type  = type;
    token->value = value;
    token->op    = op;
}

static const char *handle_digit(const char *p, int *n, token_t *tokens)
{
    char *end = NULL;
    double val = strtod(p, &end);
    set_token(&tokens[*n], TOKEN_NUM, val, 0);
    (*n)++;

    if (isalpha((unsigned char)*end) || *end == '(')
    {
        set_token(&tokens[*n], TOKEN_OP, 0.0, '*');
        (*n)++;
    }

    return end;
}

static void handle_ops(const char *p, int *n, token_t *tokens)
{
    char cur = *p;

    bool prev_is_op_or_lpar = ((*n) == 0) ||
    (tokens[(*n)-1].type == TOKEN_OP) ||
    (tokens[(*n)-1].type == TOKEN_LPART);

    if ((cur == '+' || cur == '-') && prev_is_op_or_lpar)
    {
        if ((*n) < MAX_TOKENS - 1) {
            set_token(&tokens[*n], TOKEN_NUM, 0.0, 0);
            (*n)++;
        }
    }

    set_token(&tokens[*n], TOKEN_OP, 0.0, cur);
}

static int tokenize(const char *input, token_t *tokens)
{
    int n = 0;
    const char *p = input;

    while (*p && n < MAX_TOKENS)
    {
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

static tree_node_t *create_default_op_node(int *node_ptr, tree_node_t **nodes_stack, char top)
{
    if ((*node_ptr) < 2) return NULL;
    tree_node_t *r = nodes_stack[--(*node_ptr)];
    tree_node_t *l = nodes_stack[--(*node_ptr)];
    tree_node_t *node = new_node(NODE_OP, 0, top);
    node->left = l;
    node->right = r;
    nodes_stack[(*node_ptr)++] = node;
    return node;
}

static void parse_tokens(token_t *tokens, int *n, tree_node_t **nodes_stack, char *op_stack, int *node_ptr, int *op_ptr)
{
    for (int i = 0; i < (*n); ++i)
    {
        token_t token = tokens[i];

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

static tree_node_t *generate_tree(token_t *tokens, int n)
{
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

    if (node_ptr != 1) return NULL;
    return nodes_stack[0];
}

static bool eval_num_node(tree_node_t *node, double *out)
{
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

static void flatten_mul(tree_node_t *node, double *coeff_out, int *power_out)
{
    if (!node) return;
    double v;
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
}

static void collect(tree_node_t *node, eq_t *eq, int sign)
{
    if (!node || !eq) return;

    if (node->type == NODE_NUM)      eq->c += sign * node->value;
    else if (node->type == NODE_VAR) eq->b += sign * 1.0;
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
                int p = (int)node->right->value;
                if (p == 2)      eq->a += sign * 1.0;
                else if (p == 1) eq->b += sign * 1.0;
            }
        }
    }
}

static void free_tree(tree_node_t *node)
{
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

void parse_eq_input(const char *input_orig, eq_t *eq)
{
    if (!input_orig || !eq) return;

    char *buf = strdup(input_orig);
    if (!buf) return;

    char *lh = strtok(buf, "=");
    char *rh = strtok(NULL, "=");
    if (!lh || !rh)
    {
        free(buf);
        return;
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

    // printf("\n\n%.2lf;%.2lf;%.2lf\n\n", eq->a, eq->b, eq->c);
}
