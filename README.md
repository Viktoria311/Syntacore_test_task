# Syntacore_test_task
Here is a class of a self-balancing binary search tree asked AVL tree. Lookup, insertion, and deletion all take O(log n) time using AVL tree. 

This program takes keys and queries from stdin, inserts elements into a tree and executes queries in runtime. Enter

'k' for insert a number into a tree,

'm' for finding k-th order statistic,

'n' for finding quantity of elements less than set value

and enter a number after letter separated by a space, for example:

k 8 k 2 k -1 m 1 m 2 n 3


Create a folder "build" in the root of the project, go into it and write 

"сmake .." 

on the command line to create a project configuration. After that, write on the command line

"сmake --build ." 

to build the project and get the executable file. To run the program, you can use test files from the "input_files" folder, for example

./creating_avl_tree

or send some file to the program:

./creating_avl_tree < ../input_files/file1.txt
