#include <iostream>
#include <vector>
#include "AVL_Tree.h"

int main()
{
    AVL_TREE<int> my_tree;
    int arr[] = {7, 3, 5, 9, 8, 2, 13, 11, 1, 4, 6};

    for(auto & i : arr)
        my_tree.insert(i);

    std::cout << my_tree << std::endl;
    my_tree.show();

    return 0;
}
