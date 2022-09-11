#include "AVL_Tree.h"
#include <iostream>
#include <fstream>
#include <vector>

int main()
{
        AVL_tree<int> tree3;
        int arr3[] = {12, 19, 23, 50, 2, 9, 11, 43, 51, 7, 30, 5};

        for(auto & i : arr3)
            tree3.insert(i);

        std::cout << tree3 << std::endl;

        /*
        char alpha;
        int value;
        AVL_tree<int> tree;

        while(std::cin.get(alpha) && alpha != '\n')
        {
            std::cin.get();

            if (!(std::cin >> value))
            {
                break;
            }
            std::cin.get();

            if (alpha == 'k')
            {
                tree.insert(value);
            }
            else if (alpha == 'm')
            {
                std::cout << tree.k_th_order_statistic(value);
            }
            else if (alpha == 'n')
            {
                std::cout << tree.elem_less_than(value);
            }
            else
            {
                std::cout << "Error input" << std::endl;
            }
            std::cout << ' ';
        }
        std::cin.clear();

        */
        std::cout << "Bye" << std::endl;

    return 0;
}
