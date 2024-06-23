#include "shortest_expression.hpp"
#include <vector>
#include <array>
#include "math.h"
#include <iostream>
#include <unordered_set>


constexpr std::array<const char*, 6> OPERATOR_SYMBOLS = { "+", "-", "*", "//", "%", "**" };
constexpr std::array<int, 6> OPERATOR_PRECEDENCE = { 0, 0, 1, 1, 2, 3 };


struct Expression {
    int value;
    int operation;
    int expression_length;
    const Expression* left;
    const Expression* right;
};


inline std::vector<Expression*> combine_expressions(const Expression* left, const Expression* right, int n_allowed_operations, const int* allowed_operations) {
    std::vector<Expression*> combined_expressions(n_allowed_operations);
    for (int i = 0; i < n_allowed_operations; ++i) {
        combined_expressions[i] = new Expression{ 0, -1, 0, nullptr, nullptr };
        int operation = allowed_operations[i];
        switch (operation) {
        case 0: combined_expressions[i]->value = left->value + right->value; break;
        case 1: combined_expressions[i]->value = left->value - right->value; break;
        case 2: combined_expressions[i]->value = left->value * right->value; break;
        case 3: combined_expressions[i]->value = left->value / right->value; break;
        case 4: combined_expressions[i]->value = left->value % right->value; break;
        case 5: combined_expressions[i]->value = pow(left->value, right->value); break;
        default:
            break;
        }
        combined_expressions[i]->operation = operation;
        combined_expressions[i]->expression_length = left->expression_length + right->expression_length;
        combined_expressions[i]->left = left;
        combined_expressions[i]->right = right;
    }
    return combined_expressions;
}


Expression compute_shortest_expression(int n, int n_allowed_numbers, const int* allowed_numbers, int n_allowed_operations, const int* allowed_operations) {
    // add the allowed numbers as expressions of length 1
    std::vector<std::vector<Expression*>> expressions_of_length(1, std::vector<Expression*>(n_allowed_numbers));
    for (int i = 0; i < n_allowed_numbers; ++i) {
        expressions_of_length[0][i] = new Expression{ allowed_numbers[i], -1, 1, nullptr, nullptr };
    }
    // build more complex expressions from smaller expressions
    std::unordered_set<int> found_numbers;
    int expression_length = 2;
    while (true) {
        expressions_of_length.push_back(std::vector<Expression*>());
        for (int length_1 = 1; length_1 < expression_length; ++length_1) {
            int length_2 = expression_length - length_1;
            // for all pairs of expressions of length length_1 and length_2
            for (auto expression_1 : expressions_of_length[length_1 - 1]) {
                for (auto expression_2 : expressions_of_length[length_2 - 1]) {
                    // combine the expressions in all possible ways
                    std::vector<Expression*> combined_expressions = combine_expressions(expression_1, expression_2, n_allowed_operations, allowed_operations);
                    for (auto combined_expression : combined_expressions) {
                        // we found the shortest expression
                        if (combined_expression->value == n) {
                            return *combined_expression;
                        }
                        // if we haven't seen this number before, add it to the list of expressions of this length
                        if (combined_expression->value > 0 && found_numbers.find(combined_expression->value) == found_numbers.end()) {
                            found_numbers.insert(combined_expression->value);
                            expressions_of_length[expression_length - 1].push_back(combined_expression);
                        } else {
                            delete combined_expression;
                        }
                    }
                }
            }
        }
        expression_length += 1;
    }
}

std::string expression_to_string(const Expression* expression) {
    if (expression->operation == -1) {
        return std::to_string(expression->value);
    }
    bool use_brackets_left = (expression->left->operation != -1 && OPERATOR_PRECEDENCE[expression->left->operation] < OPERATOR_PRECEDENCE[expression->operation]) || expression->operation == 3;
    bool use_brackets_right = (expression->right->operation != -1 && OPERATOR_PRECEDENCE[expression->right->operation] < OPERATOR_PRECEDENCE[expression->operation]) || expression->operation == 3;
    if (use_brackets_left) {
        if (use_brackets_right) {
            return "(" + expression_to_string(expression->left) + ")" + " " + OPERATOR_SYMBOLS[expression->operation] + " " + "(" + expression_to_string(expression->right) + ")";
        }
        return "(" + expression_to_string(expression->left) + ")" + " " + OPERATOR_SYMBOLS[expression->operation] + " " + expression_to_string(expression->right);
    } else if (use_brackets_right) {
        return expression_to_string(expression->left) + " " + OPERATOR_SYMBOLS[expression->operation] + " " + "(" + expression_to_string(expression->right) + ")";
    }
    return expression_to_string(expression->left) + " " + OPERATOR_SYMBOLS[expression->operation] + " " + expression_to_string(expression->right);
}

std::string shortest_expression_c(int n, int n_allowed_numbers, const int* allowed_numbers, int n_allowed_operations, const int* allowed_operations) {
    Expression shortest_expression = compute_shortest_expression(n, n_allowed_numbers, allowed_numbers, n_allowed_operations, allowed_operations);
    return expression_to_string(&shortest_expression);
}


Expression testing() {
    Expression* expression_1 = new Expression{ 1, -1, 1, nullptr, nullptr };
    Expression* expression_2 = new Expression{ 2, -1, 1, nullptr, nullptr };
    Expression expression_3 = { 3, 0, 2, expression_1, expression_2 };
    return expression_3;
}


int main(int argc, char const* argv[]) {
    int* allowed_numbers = new int[15] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16 };
    int* allowed_operations = new int[6] { 0, 1, 2, 3, 4, 5 };
    std::cout << shortest_expression_c(9279, 15, allowed_numbers, 6, allowed_operations) << std::endl;
    // Expression expression = testing();
    // std::cout << expression_to_string(&expression) << std::endl;
    return 0;
}
