#include <Python.h>
#include "sudoku.c"
#include <time.h>

static PyObject *ext_generate_sudoku(PyObject *self, PyObject *args) {
    int clues;
    if(!PyArg_ParseTuple(args, "i", &clues)) return NULL;
    srand(clock());
    Cell *solution = malloc(sizeof(Cell) * SUDOKU_SIZE * SUDOKU_SIZE);
    Cell* sudoku = generateSudoku(clues, 1000, solution);

    PyObject *sudokuList = PyList_New(SUDOKU_SIZE * SUDOKU_SIZE);
    for(int i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++) {
        PyList_SetItem(sudokuList, i, PyLong_FromLong(sudoku[i].value));
    }
    PyObject *solutionList = PyList_New(SUDOKU_SIZE * SUDOKU_SIZE);
    for(int i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++) {
        PyList_SetItem(solutionList, i, PyLong_FromLong(solution[i].value));
    }

    free(sudoku);
    free(solution);
    return Py_BuildValue("OO", sudokuList, solutionList);
}

static PyObject *ext_solve_sudoku(PyObject *self, PyObject *args) {
    PyObject *list;
    if(!PyArg_ParseTuple(args, "O", &list)) return NULL;
    Cell *sudoku = createBoard();
    for(int i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++) {
        (sudoku + i)->value = PyLong_AsLong(PyList_GetItem(list, i));
    }
    Cell *solution = solveGrid(sudoku);
    free(sudoku);
    
    PyObject *solutionList = PyList_New(SUDOKU_SIZE * SUDOKU_SIZE);
    for(int i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++) {
        PyList_SetItem(solutionList, i, PyLong_FromLong(solution[i].value));
    }
    free(solution);

    return solutionList;
}

static PyMethodDef PyNativeSudokuMethods[] = {
    {"generate_sudoku", ext_generate_sudoku, METH_VARARGS, "Generates a sudoku using native C code."},
    {"solve_sudoku", ext_solve_sudoku, METH_VARARGS, "Solves a given sudoku using native C code."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef sudokuModule = {
    PyModuleDef_HEAD_INIT,
    "_sudoku_native",
    NULL,
    -1,
    PyNativeSudokuMethods
};

PyMODINIT_FUNC PyInit__sudoku_native() {
    PyObject* module = PyModule_Create(&sudokuModule);
    if (module == NULL) return NULL;
    return module;
}

int main(int argc, char const *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    PyImport_AppendInittab("_sudoku_native", PyInit__sudoku_native);

    Py_SetProgramName(program);

    Py_Initialize();

    PyImport_ImportModule("_sudoku_native");

    PyMem_RawFree(program);
    return 0;
}
