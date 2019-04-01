import _sudoku_native
import _sudoku_python


generate_native = _sudoku_native.generate_sudoku
generate_python = _sudoku_python.generate_sudoku
solve_native = _sudoku_native.solve_sudoku
solve_python = _sudoku_python.solve_sudoku


def print_sudoku(sudoku):
    assert type(sudoku) == list and len(sudoku) == 81, "Invalid sudoku"

    sep = "+–––––––+–––––––+–––––––+"
    print(sep)
    r = 1
    for ri in range(9):
        print(end="| ")
        c = 1
        for ci in range(9):
            v = sudoku[ri * 9 + ci]
            print(v if v > 0 else " ", end=(" | " if c % 3 == 0 else " "))
            c += 1
        print(end="\n")
        if r % 3 == 0:
            print(sep)
        r += 1


def save_sudoku(sudoku, filename):
    assert type(sudoku) == list and len(sudoku) == 81, "Invalid sudoku"

    with open(filename, "wb+") as f:
        for v in sudoku:
            f.write(bytes([v]))
