# Simple parser

## Authors:

* Bernhard Posselt
* Christoph Sladkovsky

## Interpreter notices

* The interpreter accepts only symbols (variable/function identifiers) that are one character long (because hashmaps are hard :P)
* The language is function scoped

## Dependencies

* getopt
* bison
* make

## Compiling
Run:

    make

## Executing a file
To execute a file pass the path as its first argument like:

    ./interpreter programs/fibonacci

This example program has to functions, one which calculates the fibonacci numbers in a loop and one which calculates the numbers recursively

## Debugging

Run:

    ./interpreter programs/fibonacci -d