#include "AVL_Tree.h"
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[])
{
    std::ifstream fin;
    fin.open(argv[1]);

    if (!fin.is_open())
    {
        std::cout << "Can`t open this file\n";
    }
    else
    {
        char alpha;
        int value;
        std::vector<std::pair<char, int>> program_input;
        AVL_TREE<int> tree;

        while(!fin.eof())
        {
            fin.get(alpha);
            fin.get();

            fin >> value;
            fin.get();

            if (alpha == 'k')
            {
                tree.insert(value);
            }
            if (!fin.eof() && (alpha == 'm' || alpha == 'n'))
            {
                program_input.push_back(std::make_pair(alpha, value));
            }
        }
        fin.clear();

        for(auto & i : program_input)
        {
            if (i.first == 'm')
            {
                std::cout << tree.smallest_order_statistic(i.second);
            }
            else
            {
                std::cout << tree.elem_less_than(i.second);
            }
            std::cout << ' ';
        }
        std::cout << std::endl;


        fin.close();
    }

    return 0;
}
