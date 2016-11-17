# SIMPOL INTERPRETER 

## Authors:

* Bernhard Posselt
* Christoph Sladkovsky

## Interpreter notices

* The interpreter accepts only symbols (variable/function identifiers) that are one character long (because hashmaps in C are hard :P)
* The language is function scoped

## Design questions

### Should there be a program entry point like a main procedure?

Not really needed but it can be done like:

    sub m () begin

    end

    m();

### Should global variables be allowed?

Globals are read only since the scope is copied for function calls

### Can some syntactic restrictions be relaxed?

Could be but was not done in this example

### What further data types should I use?

There are currently only ints for simplicity

### Should the declaration of variables be optional?

Integer variable declaration is optional. All int variables are initialized with 0

### Should I add further keywords like true or false?

Not done for the sake of a common return type (int)


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
