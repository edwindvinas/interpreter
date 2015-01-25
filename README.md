# Simple parser

## Authors:

* Bernhard Posselt
* Christoph Sladkovsky

## Interpreter notices

* The interpreter accepts only symbols (variable/function identifiers) that are one character long
* The language is function scoped
* The return statement does not break execution but simply stores the return value

## Compiling
Install **bison** and **make** and run:

    make

## Executing a file
To execute a file pass the path as its first argument like:

    ./compiler programs/calculations

## Debugging

Run:

    ./compiler programs/calculations -d