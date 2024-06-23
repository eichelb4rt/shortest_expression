import numpy as np
import argparse

from shortest_expression_interface import shortest_expression


ALL_OPERATORS = ["+", "-", "*", "//", "%", "**"]


def main():
    with open("allowed_numbers.txt", "r") as file:
        ALLOWED_NUMBERS = [int(number) for number in file.read().split(",") if number]
    with open("allowed_operations.txt", "r") as file:
        ALLOWED_OPERATORS = [operator.strip() for operator in file.read().split(",") if operator]
    print(f"Allowed numbers: {ALLOWED_NUMBERS}")
    print(f"Allowed operators: {ALLOWED_OPERATORS}")
    allowed_operator_indices = [ALL_OPERATORS.index(operator) for operator in ALLOWED_OPERATORS]

    parser = argparse.ArgumentParser(description="Find the shortest expression for a given number")
    parser.add_argument("number", type=int, help="The number to find the shortest expression for")
    args = parser.parse_args()

    expression = shortest_expression(args.number, np.array(ALLOWED_NUMBERS, dtype=np.int32), np.array(allowed_operator_indices, dtype=np.int32))

    print(f"{args.number} = {expression}")


if __name__ == "__main__":
    main()
