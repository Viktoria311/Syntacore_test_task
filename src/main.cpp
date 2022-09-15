#include "AVL_Tree.h"
#include <iostream>
#include <fstream>
#include <vector>

void result(AVL_tree<int> & tree_, char & alpha_, int& value_ );
void message1();
void message2();
void message3();
void message4();
void message5(const AVL_tree<int> & tree_);

int main()
{
        AVL_tree<int> tree3;
        //int arr3[] = {20, 12, 43, 9, 27, 50, 19, 11, 15, 23, 51, 30};
        int arr3[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
                      21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
                      41,42,43,44,45,46,47,48,49,50,};
        for(auto & i : arr3)
            tree3.insert(i);

        std::cout << tree3 << std::endl;

        tree3.remove(50);

        std::cout << tree3 << std::endl;
        tree3.print();

        /*
        char alpha;
        int value;
        AVL_tree<int> tree;

        while(std::cin.get(alpha) && alpha != '\n')
        {
            std::cin.get();

            if (!(std::cin >> value))
            {
                message5();
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
                message3();
            }
            std::cout << ' ';
        }
        if (!std::cin)
        {
            message4();
            std::cin.clear();
        }

        */
        std::cout << "Bye" << std::endl;

    return 0;
}

void result(AVL_tree<int> & tree_, char & alpha_, int& value_ )
{
    if (alpha_ == 'k')
    {
        //tree_.insert(value_);
    }
    else if (alpha_ == 'm')
    {
        //std::cout << tree_.k_th_order_statistic(value);
    }
    else if (alpha_ == 'n')
    {
        //std::cout << tree_.elem_less_than(value_);
    }
    else
    {
        message3();
    }
}

void message1()
{
    std::cerr << "Uncorrect input:\n";
    std::cerr << "Maybe you missed a space.\n";
}
void message2()
{
    std::cerr << "'k' to insert an element into a tree.\n";
    std::cerr << "'m' to find k-th order statistic.\n";
    std::cerr << "'n' to count how many elements less than it.\n";
}
void message3()
{
    std::cerr << "Uncorrect letter:\n";
    message2();
}
void message4()
{
    std::cerr << "Uncorrect input: enter a letter\n";
    message2();
}
void message5(const AVL_tree<int> & tree_)
{
    message1();
    std::cerr << "Enter any number to insert it into a container or to count how many elements less than it.\n";
    std::cerr << "To find k-th order statistic enter any positive number\n"
              << " which is not bigger than quantity of elements in a container in this moment (" << tree_.size() << ").\n";
}
