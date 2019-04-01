#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUDOKU_SIZE 9
#define BLOCK 3

typedef unsigned char ubyte;
typedef ubyte bool;

#define false 0
#define true 1

typedef struct Cell
{
    ubyte x;
    ubyte y;
    ubyte value;
} Cell;

Cell *getCell(ubyte x, ubyte y, Cell board[])
{
    return board + (y * SUDOKU_SIZE + x);
}

Cell *getRow(ubyte row, Cell *board)
{
    Cell *rowCells = malloc(sizeof(Cell) * SUDOKU_SIZE);
    for (ubyte i = 0; i < SUDOKU_SIZE; i++)
    {
        rowCells[i] = *getCell(i, row, board);
    }
    return rowCells;
}

Cell *getColumn(ubyte column, Cell *board)
{
    Cell *columnCells = malloc(sizeof(Cell) * SUDOKU_SIZE);
    for (ubyte i = 0; i < SUDOKU_SIZE; i++)
    {
        columnCells[i] = *getCell(column, i, board);
    }
    return columnCells;
}

Cell *getBlock(ubyte bx, ubyte by, Cell *board)
{
    Cell *blockCells = malloc(sizeof(Cell) * SUDOKU_SIZE);
    for (ubyte y = 0; y < BLOCK; y++)
    {
        for (ubyte x = 0; x < BLOCK; x++)
        {
            blockCells[y * BLOCK + x] = getCell(bx * BLOCK + x, by * BLOCK + y, board)[0];
        }
    }
    return blockCells;
}

Cell *getBlockNotEmpty(ubyte bx, ubyte by, Cell *board, ubyte *length)
{
    Cell *blockCells = getBlock(bx, by, board);
    Cell *notEmptyCells = malloc(sizeof(Cell) * SUDOKU_SIZE);

    ubyte counter = 0;
    for (ubyte i = 0; i < SUDOKU_SIZE; i++)
    {
        if (blockCells[i].value != 0)
        {
            notEmptyCells[counter] = blockCells[i];
            counter++;
        }
    }
    *length = counter;
    free(blockCells);

    return notEmptyCells;
}

Cell *getRowNotEmpty(ubyte row, Cell *board, ubyte *length)
{
    Cell *rowCells = getRow(row, board);
    Cell *notEmptyCells = malloc(sizeof(Cell) * SUDOKU_SIZE);

    ubyte counter = 0;
    for (ubyte i = 0; i < SUDOKU_SIZE; i++)
    {
        if (rowCells[i].value != 0)
        {
            notEmptyCells[counter] = rowCells[i];
            counter++;
        }
    }
    *length = counter;
    free(rowCells);

    return notEmptyCells;
}

Cell *getColumnNotEmpty(ubyte column, Cell *board, ubyte *length)
{
    Cell *columnCells = getColumn(column, board);
    Cell *notEmptyCells = malloc(sizeof(Cell) * SUDOKU_SIZE);

    ubyte counter = 0;
    for (ubyte i = 0; i < SUDOKU_SIZE; i++)
    {
        if (columnCells[i].value != 0)
        {
            notEmptyCells[counter] = columnCells[i];
            counter++;
        }
    }
    *length = counter;
    free(columnCells);

    return notEmptyCells;
}

bool isRowValid(ubyte row, ubyte value, Cell *board)
{
    ubyte length;
    Cell *ne = getRowNotEmpty(row, board, &length);
    for (ubyte i = 0; i < length; i++)
    {
        if (ne[i].value == value)
        {
            free(ne);
            return false;
        }
    }
    free(ne);
    return true;
}

bool isColumnValid(ubyte column, ubyte value, Cell *board)
{
    ubyte length;
    Cell *ne = getColumnNotEmpty(column, board, &length);
    for (ubyte i = 0; i < length; i++)
    {
        if (ne[i].value == value)
        {
            free(ne);
            return false;
        }
    }
    free(ne);
    return true;
}

bool isBlockValid(ubyte bx, ubyte by, ubyte value, Cell *board)
{
    ubyte length;
    Cell *ne = getBlockNotEmpty(bx, by, board, &length);
    for (ubyte i = 0; i < length; i++)
    {
        if (ne[i].value == value)
        {
            free(ne);
            return false;
        }
    }
    free(ne);
    return true;
}

bool isValid(ubyte x, ubyte y, ubyte value, Cell *board)
{
    return isRowValid(y, value, board) && isColumnValid(x, value, board) && isBlockValid(x / 3, y / 3, value, board);
}

ubyte *getValidValues(ubyte x, ubyte y, Cell *board, ubyte *length)
{
    ubyte *values = malloc(SUDOKU_SIZE);
    ubyte c = 0;
    for (ubyte i = 1; i <= SUDOKU_SIZE; i++)
    {
        if (isValid(x, y, i, board))
        {
            values[c] = i;
            c++;
        }
    }
    *length = c;
    return values;
}

Cell *getEmpty(Cell *board, ubyte *length)
{
    Cell *emptyCells = malloc(sizeof(Cell) * SUDOKU_SIZE * SUDOKU_SIZE);
    ubyte c = 0;
    for (ubyte i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++)
    {
        if (board[i].value == 0)
        {
            emptyCells[c] = board[i];
            c++;
        }
    }
    *length = c;
    return emptyCells;
}

Cell *createBoard()
{
    Cell *board = malloc(sizeof(Cell) * SUDOKU_SIZE * SUDOKU_SIZE);
    for (ubyte y = 0; y < SUDOKU_SIZE; y++)
    {
        for (ubyte x = 0; x < SUDOKU_SIZE; x++)
        {
            Cell c = {x, y, 0};
            board[y * SUDOKU_SIZE + x] = c;
        }
    }
    return board;
}

void shuffleCellArray(Cell *array, ubyte length)
{
    for (ubyte i = 0; i < length; i++)
    {
        ubyte j = rand() % length;
        Cell c = array[i];
        array[i] = array[j];
        array[j] = c;
    }
}

void shuffleByteArray(ubyte *array, ubyte length)
{
    for (ubyte i = 0; i < length; i++)
    {
        ubyte j = rand() % length;
        ubyte c = array[i];
        array[i] = array[j];
        array[j] = c;
    }
}

ubyte **getValidValuesForArray(Cell *cells, ubyte length, Cell *board, ubyte *lengths)
{
    ubyte **result = malloc(sizeof(ubyte *) * length);
    for (ubyte i = 0; i < length; i++)
    {
        ubyte *res = getValidValues(cells[i].x, cells[i].y, board, lengths + i);
        result[i] = res;
    }
    return result;
}

bool fillGrid(Cell *board)
{
    ubyte emptyLength;
    Cell *empty = getEmpty(board, &emptyLength);
    if (emptyLength == 0)
    {
        free(empty);
        return true;
    }
    shuffleCellArray(empty, emptyLength);
    ubyte *validLengths = malloc(emptyLength);
    ubyte **validValues = getValidValuesForArray(empty, emptyLength, board, validLengths);

    ubyte smallest = 10;
    ubyte smallestId = -1;
    for (ubyte i = 0; i < emptyLength; i++)
    {
        if (validLengths[i] < smallest)
        {
            smallest = validLengths[i];
            smallestId = i;
        }
    }
    free(validLengths);

    Cell cell = empty[smallestId];
    Cell *cellp = getCell(cell.x, cell.y, board);
    ubyte candidatesLength = smallest;
    ubyte *candidates = malloc(candidatesLength);
    memcpy(candidates, validValues[smallestId], candidatesLength);

    for (ubyte i = 0; i < emptyLength; i++)
    {
        free(validValues[i]);
    }
    free(validValues);
    free(empty);

    if (candidatesLength == 0)
    {
        free(candidates);
        return false;
    }

    shuffleByteArray(candidates, candidatesLength);

    for (ubyte i = 0; i < candidatesLength; i++)
    {
        cellp->value = candidates[i];
        bool result = fillGrid(board);
        if (result)
        {
            free(candidates);
            return true;
        }
    }
    cellp->value = 0;

    free(candidates);
    return false;
}

bool solveRecursively(Cell *board, Cell *solution, int *solutionLength)
{
    ubyte emptyLength;
    Cell *empty = getEmpty(board, &emptyLength);
    if (emptyLength == 0)
    {
        free(empty);
        if (*solutionLength > 0)
        {
            return true;
        }
        else
        {
            memcpy(solution, board, sizeof(Cell) * SUDOKU_SIZE * SUDOKU_SIZE);
            *solutionLength = 1;
            return false;
        }
    }
    ubyte *validLengths = malloc(emptyLength);
    ubyte **validValues = getValidValuesForArray(empty, emptyLength, board, validLengths);

    ubyte smallest = 10;
    ubyte smallestId = -1;
    for (ubyte i = 0; i < emptyLength; i++)
    {
        if (validLengths[i] < smallest)
        {
            smallest = validLengths[i];
            smallestId = i;
        }
    }
    free(validLengths);

    Cell cell = empty[smallestId];
    free(empty);
    Cell *cellp = getCell(cell.x, cell.y, board);
    ubyte candidatesLength = smallest;
    ubyte *candidates = malloc(candidatesLength);
    memcpy(candidates, validValues[smallestId], candidatesLength);

    for (ubyte i = 0; i < emptyLength; i++)
    {
        free(validValues[i]);
    }
    free(validValues);

    if (candidatesLength == 0)
    {
        free(candidates);
        return false;
    }

    shuffleByteArray(candidates, candidatesLength);

    for (ubyte i = 0; i < candidatesLength; i++)
    {
        cellp->value = candidates[i];
        if (solveRecursively(board, solution, solutionLength))
        {
            free(candidates);
            return true;
        }
    }
    cellp->value = 0;

    free(candidates);
    return false;
}

Cell *solveGrid(Cell *board)
{
    Cell *oldBoard = malloc(sizeof(Cell) * SUDOKU_SIZE * SUDOKU_SIZE);
    memcpy(oldBoard, board, sizeof(Cell) * SUDOKU_SIZE * SUDOKU_SIZE);

    int solutionLength = 0;
    Cell *solution = malloc(sizeof(Cell) * SUDOKU_SIZE * SUDOKU_SIZE);
    bool result = solveRecursively(board, solution, &solutionLength);
    memcpy(board, oldBoard, sizeof(Cell) * SUDOKU_SIZE * SUDOKU_SIZE);
    free(oldBoard);

    if (solutionLength == 1 && result == 0)
    {
        return solution;
    }
    else
    {
        free(solution);
        return NULL;
    }
}

Cell *generateSudoku(int clues, int maxtries, Cell *solution)
{
    int totalremove = SUDOKU_SIZE * SUDOKU_SIZE - clues;
    Cell *board = createBoard();
    fillGrid(board);
    if(solution != NULL){
        memcpy(solution, board, sizeof(Cell) * SUDOKU_SIZE * SUDOKU_SIZE);
    }

    int removed = 0;
    int i = 0;
    while (removed < totalremove && i < maxtries)
    {
        ubyte x = (rand() & 0xFF) % 9;
        ubyte y = (rand() & 0xFF) % 9;
        Cell *c = getCell(x, y, board);
        ubyte old = c->value;
        if (old == 0)
        {
            continue;
        }
        c->value = 0;

        Cell *sol = solveGrid(board);
        if (sol != NULL)
        {
            removed++;
            free(sol);
        }
        else
        {
            c->value = old;
        }
        i++;
    }

    return board;
}