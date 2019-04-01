from setuptools import setup, Extension

sudoku_native = Extension("_sudoku_native", ["sudoku_native.c"])


def readme():
    with open("README.rst", "r") as f:
        return f.read()


setup(name="sudoku",
      version="1.0",
      description="A python package to generate sudokus.",
      long_description=readme(),
      author="Ian Rehwinkel",
      author_email="ian.rehwinkel02@gmail.com",
      classifiers=[
          "Operating System :: OS Independent"
      ],
      license='MIT',
      ext_modules=[sudoku_native],
      py_modules=["sudoku", "_sudoku_python"])
