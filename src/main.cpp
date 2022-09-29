#include "AVL_Tree.h"
#include <iostream>
#include <cstdlib>
#include <vector>

void result(AVL_tree<int> & tree_, char alpha_, int value_ );
void message1();
void message2();
void message3();
void message4();
void message5(const AVL_tree<int> & tree_);


int main()
{
    char alpha;
    int value;
    AVL_tree<int> tree;

    char space = ' ';

    do
    {
        if (space == '\n')
        {
            break;
        }
        else if (space == ' ')
        {
            std::cin.get(alpha);
        }
        else if (isalpha(space))
        {
            message1();
            alpha = space;
        }

        if (!std::cin)
        {
            std::cin.clear();
        }

        std::cin.get(space);

        if (space == '\n')
        {
            break;
        }
        // enter a number
        else if (space == ' ' && std::cin >> value)
        {
            // output the result
            result(tree, alpha, value);
            std::cin.get(space);
        }
        else if (!std::cin)
        {
            std::cin.clear();
            break;
        }
        else if(space == '-')
        {
            // space is forgotten
            message1();

            std::vector<int> buffer;
            value = 0;

            while(std::cin.get(space) && isdigit(space) )
                buffer.push_back(atoi(const_cast<const char *>(&space)));

            auto it = buffer.rbegin();

            for(int i = 0; it != buffer.rend(); ++it, ++i)
                value += (*it) * std::pow(10, i);

            // output the result
            result(tree, alpha, -1 * value);
        }
        else if (isdigit(space))
        {
            // space is forgotten
            message1();

            std::vector<int> buffer;
            buffer.push_back(atoi(const_cast<const char *>(&space)));
            value = 0;

            while(std::cin.get(space) && isdigit(space) )
                buffer.push_back(atoi(const_cast<const char *>(&space)));

            auto it = buffer.rbegin();

            for(int i = 0; it != buffer.rend(); ++it, ++i)
                value += (*it) * std::pow(10, i);

            // output the result
            result(tree, alpha, value);
        }
        else
        {
            message5(tree);
            break;
        }

    } while(alpha != '\n' && space != '\n');

    std::cout << std::endl;

    return 0;
}

void result(AVL_tree<int> & tree_, char alpha_, int value_ )
{
    if (alpha_ == 'k')
    {
        tree_.insert(value_);
    }
    else if (alpha_ == 'm')
    {
        std::cout << tree_.k_th_order_statistic(value_) << ' ';
    }
    else if (alpha_ == 'n')
    {
        std::cout << tree_.elem_less_than(value_) << ' ';
    }
    else
    {
        message3();
    }
}

void message1()
{
    std::cerr << "\nUncorrect input:\n";
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
    std::cerr << "\nUncorrect letter:\n";
    message2();
}
void message4()
{
    std::cerr << "\nUncorrect input: enter a letter\n";
    message2();
}
void message5(const AVL_tree<int> & tree_)
{
    message1();
    std::cerr << "Enter any number to insert it into a container or to count how many elements less than it.\n";
    std::cerr << "To find k-th order statistic enter any positive number\n"
              << " which is not bigger than quantity of elements in a container in this moment (" << tree_.size() << ").\n";
}
