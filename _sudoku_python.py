import io
import random
import time
import copy
import numpy


class Cell:
    def __init__(self, value, x, y, board, blocksize):
        self.value = value
        self.x = x
        self.y = y
        self.board = board
        self.blocksize = blocksize
        self.max_value = blocksize[0] * blocksize[1]

    def is_valid(self, value):
        return self.is_block_valid(value) and self.is_row_valid(value) and self.is_column_valid(value)

    def is_row_valid(self, value):
        return value not in [x.value for x in self.board.get_row_ne(self.y)]

    def is_column_valid(self, value):
        return value not in [x.value for x in self.board.get_column_ne(self.x)]

    def is_block_valid(self, value):
        block_x = self.x // self.blocksize[0]
        block_y = self.y // self.blocksize[1]
        return value not in [x.value for x in self.board.get_block_ne(block_x, block_y)]

    def get_valid_values(self):
        return list(filter(lambda x: self.is_valid(x), list(range(1, self.max_value + 1))))

    def clear(self):
        self.value = 0

    def is_empty(self):
        return self.value == 0

    def __repr__(self):
        return str(self)

    def __str__(self):
        return "{}(x{} y{})".format(self.value, self.x, self.y)


class Board:
    def __init__(self, size, blocksize):
        self.size = size
        self.blocksize = blocksize
        self.max_value = blocksize[0] * blocksize[1]
        self.values = [[Cell(0, x, y, self, blocksize)
                        for x in range(size)] for y in range(size)]

    def __getitem__(self, coords):
        return self.values[coords[1]][coords[0]]

    def get_column(self, column):
        return [row[column] for row in self.values]

    def get_row(self, row):
        return self.values[row]

    def get_block(self, x, y):
        bsx = self.blocksize[0]
        bsy = self.blocksize[1]
        return [self[x * bsx + ci, y * bsy + ri] for ci in range(bsx) for ri in range(bsy)]

    def get_block_ne(self, x, y):  # not empty
        return list(filter(lambda x: not x.is_empty(), self.get_block(x, y)))

    def get_row_ne(self, row):  # not empty
        return list(filter(lambda x: not x.is_empty(), self.get_row(row)))

    def get_column_ne(self, column):  # not empty
        return list(filter(lambda x: not x.is_empty(), self.get_column(column)))

    def get_all(self):
        return [item for sublist in self.values for item in sublist]

    def get_empty(self):
        return list(filter(lambda x: x.is_empty(), self.get_all()))

    def __repr__(self):
        return str(self)


class Sudoku:
    def __init__(self, items=None):
        self.board = Board(9, (3, 3))
        if items:
            for y in range(9):
                for x in range(9):
                    self.board[x, y].value = items[y][x]

    UNSOLVABLE = -1
    UNSOLVED = 0
    SOLVED = 1
    MULTIPLE = 2

    def solve(self):
        old_board = Sudoku([[x.value for x in row]
                            for row in self.board.values]).board

        solutions = []
        result = self.solve_recursive(solutions)
        self.board = old_board

        if result == self.UNSOLVABLE:
            return self.UNSOLVABLE, None
        elif result == self.UNSOLVED:
            return self.SOLVED, solutions[0]
        elif len(solutions) > 1:
            return self.MULTIPLE, None

    def solve_recursive(self, solutions):
        empty = self.board.get_empty()
        if len(empty) == 0:
            solutions.append(Sudoku([[x.value for x in row]
                                     for row in self.board.values]).board)
            if len(solutions) == 2:
                return self.SOLVED
            return self.UNSOLVED
        first_candy = min([(cell, cell.get_valid_values())
                           for cell in empty], key=lambda x: len(x[1]))

        cell, candidates = first_candy
        if len(candidates) == 0:
            return self.UNSOLVABLE

        random.shuffle(candidates)

        for candidate in candidates:
            cell.value = candidate
            res = self.solve_recursive(solutions)
            if res == self.SOLVED:
                return self.SOLVED
        cell.value = 0
        return self.UNSOLVED

    def fill(self):
        empty = self.board.get_empty()
        if len(empty) == 0:
            return True
        random.shuffle(empty)

        cell = min(empty, key=lambda x: len(x.get_valid_values()))
        candidates = cell.get_valid_values()
        if len(candidates) == 0:
            return False

        random.shuffle(candidates)
        for candidate in candidates:
            cell.value = candidate
            if self.fill():
                return True
        cell.value = 0
        return False

    def __str__(self):
        return str(self.board)


def generate_sudoku(clues):
    remove = 81 - clues
    s = Sudoku()
    s.fill()

    removed = 0
    while removed < remove:
        x = random.randint(0, 8)
        y = random.randint(0, 8)
        v = s.board[x, y].value
        if v == 0:
            continue
        s.board[x, y].clear()

        code, _ = s.solve()
        if code != Sudoku.SOLVED:
            s.board[x, y].value = v
        else:
            removed += 1
    return [x.value for row in s.board.values for x in row]

def _chunks(l, n):
    n = max(1, n)
    return (l[i:i+n] for i in range(0, len(l), n))

def solve_sudoku(sudoku_values):
    sud = Sudoku(list(_chunks(sudoku_values, 9)))
    solution = sud.solve()[1]
    if solution:
        return [x.value for row in solution.values for x in row]
    else:
        return None