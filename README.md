## UID: 305965764

## Pipe Up

Similar program to how UNIX pipe works where the standard output of a child is redirected into the
standard input of the 2nd child and it keeps looping until the last process.

## Building

You do make

## Running

./pipe ls wc cat -> outputs the word count program for the files in my directory
./pipe ls wc ls -> just lists my directory
./pipe ls cat ls sort more ls ls cat more  wc ls ls wc wc more more more cat cat cat cat cat ls ls ls ls -> just lists my directory


## Cleaning up

make clean
