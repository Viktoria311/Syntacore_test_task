#include "AVL_Tree.h"
#include <iostream>
#include <fstream>
#include <vector>

int main()
{
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

        std::cout << "Bye" << std::endl;

    return 0;
}
