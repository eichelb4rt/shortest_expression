import argparse
from dataclasses import dataclass

ALLOWED_NUMBERS = [1, 2, 3, 4, 5, 6]
ALLOWED_OPERATIONS = ["+", "-", "*"]

OPERATOR_PRECEDENCE = {
    "+": 1,
    "-": 1,
    "*": 2,
    "_": 5000,  # dummy operator to indicate no operator
}


@dataclass
class Expression:
    result: int
    expression_string: str
    length: int
    last_operator: str


def all_combinations(expression_1: Expression, expression_2: Expression) -> list[Expression]:
    combinations = []
    for operator in ALLOWED_OPERATIONS:
        # only brackets if there are multiple numbers or if the last operator is not the same as the current one
        use_brackets_1 = expression_1.length > 1 and OPERATOR_PRECEDENCE[expression_1.last_operator] < OPERATOR_PRECEDENCE[operator]
        use_brackets_2 = expression_2.length > 1 and OPERATOR_PRECEDENCE[expression_2.last_operator] < OPERATOR_PRECEDENCE[operator]
        expression_1_str = f"({expression_1.expression_string})" if use_brackets_1 else expression_1.expression_string
        expression_2_str = f"({expression_2.expression_string})" if use_brackets_2 else expression_2.expression_string
        new_expression = f"{expression_1_str} {operator} {expression_2_str}"
        new_result = eval(f"{expression_1.result}{operator}{expression_2.result}")
        combinations.append(Expression(new_result, new_expression, expression_1.length + expression_2.length, operator))
    return combinations


def get_shortest_expressions(lower_bound: int, upper_bound: int) -> dict[int, Expression]:
    # length -> expressions of that length
    expressions_of_length: dict[int, list[Expression]] = {}
    expressions_of_length[1] = [Expression(allowed_number, str(allowed_number), 1, "_") for allowed_number in ALLOWED_NUMBERS]

    # number -> Expression
    found_expressions: dict[int, Expression] = {}
    n_found = len(ALLOWED_NUMBERS)
    result_length = 2
    while n_found < upper_bound - lower_bound + 1:
        expressions_of_length[result_length] = []
        for length_1 in range(1, result_length):
            length_2 = result_length - length_1
            for expression_1 in expressions_of_length[length_1]:
                for expression_2 in expressions_of_length[length_2]:
                    for combination in all_combinations(expression_1, expression_2):
                        if combination.result >= lower_bound and combination.result <= upper_bound and combination.result not in found_expressions:
                            expressions_of_length[result_length].append(combination)
                            found_expressions[combination.result] = combination
                            n_found += 1
        result_length += 1
    return found_expressions


def main():
    parser = argparse.ArgumentParser(description="Find the shortest expression for a given number")
    parser.add_argument("number", type=int, help="The number to find the shortest expression for")
    args = parser.parse_args()
    lower_bound = 1
    upper_bound = args.number * 2
    all_shortest_expressions = get_shortest_expressions(lower_bound, upper_bound)
    print(f"{args.number} = {all_shortest_expressions[args.number].expression_string}")


if __name__ == "__main__":
    main()
