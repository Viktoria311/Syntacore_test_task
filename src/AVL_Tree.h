#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <iostream>
#include <cmath>

template<typename T>
struct Node
{
    T value_;
    int balance_;
    Node<T> * right_branch_;
    Node<T> * left_branch_;
    int elements_; // quantity of elements in this subtree
    Node(T value)
    {
        value_ = value;
        balance_ = 0;
        right_branch_ = nullptr;
        left_branch_ = nullptr;
        elements_ = 1;
    }
    Node<T> * operator=(Node<T> * node);
    void delete_all(Node<T> * node);
};

template<typename T>
Node<T> * Node<T>::operator=(Node<T> * node)
{
    if (this != nullptr && node != nullptr)
    {
        this->value_ = node->value_;
        this->balance_ = node->balance_;
        this->left_branch_->operator=(node->left_branch_);
        this->right_branch_->operator=(node->right_branch_);
        this->elements_ = node->elements_;
    }
    else if (this!= nullptr && node == nullptr)
    {
        delete_all(this);
    }
    else if (this == nullptr && node != nullptr)
    {
        this = new Node<T>;
        this->value_ = node->value_;
        this->balance_ = node->balance_;
        this->left_branch_->operator=(node->left_branch_);
        this->right_branch_->operator=(node->right_branch_);
        this->elements_ = node->elements_;
    }
    return this;
}

template<typename T>
void Node<T>::delete_all(Node<T> * node)
{
    if (node != nullptr)
    {
        Node<T> * l_branch = node->left_branch_;
        Node<T> * r_branch = node->right_branch_;
        delete node;
        node = nullptr;
        delete_all(l_branch);
        delete_all(r_branch);
    }
}

template<typename T>
class AVL_tree;

template<typename T>
std::ostream & operator<<(std::ostream & os, const Node<T> * node);

template<typename T>
std::ostream & operator<<(std::ostream & os, const AVL_tree<T> & tree);


template<typename T>
class AVL_tree
{
    private:
        Node<T> * root;
        int height(const Node<T> * node) const;
        void set_balance(Node<T> * node);
        void L_rotate(Node<T> ** root_node);
        void R_rotate(Node<T> ** root_node);
        void LR_rotate(Node<T> ** root_node);
        void RL_rotate(Node<T> ** root_node);
        void check_and_rotate(Node<T> ** node, T item);
        void check_and_rotate(Node<T> ** node, Node<T> ** parent);
        bool check_and_rotate(Node<T> ** node);
        Node<T> * create_tree(Node<T> * node);
        T min(Node<T> * node) const; // finding min element in a branch
        T max(Node<T> * node) const; // finding max element in a branch
        int elements_quantity(Node<T> * node) const;
        void show(Node<T> * node) const;
        void print_n(const Node<T> * node, int n, int level, int prob) const;
        void print(const Node<T> * node) const;
        void delete_all(Node<T> * node);
    public:
        AVL_tree();
        AVL_tree(const AVL_tree<T> & tree);
        ~AVL_tree();
        AVL_tree<T> & operator=(const AVL_tree<T> & tree);
        bool is_there(T item) const;
        void insert(T item);
        void remove(T item);
        int size() const;
        void show() const;
        void print() const;
        T k_th_order_statistic(int i) const;
        int elem_less_than(T item) const;
        T min() const; // finding min element in a tree
        T max() const; // finding max element in a tree
        friend std::ostream & operator<< <T> (std::ostream & os, const Node<T> * node);
        friend std::ostream & operator<< <T> (std::ostream & os, const AVL_tree<T> & tree);

};

template<typename T>
AVL_tree<T>::AVL_tree()
{
    root = nullptr;
}

template<typename T>
Node<T> * AVL_tree<T>::create_tree(Node<T> * node)
{
    if (node == nullptr) return nullptr;

    Node<T> * new_node = new Node<T>(node->value_);

    if (node->left_branch_ != nullptr)
        new_node->left_branch_ = Node<T>(node->left_branch_->value_);

    if (node->right_branch_ != nullptr)
        new_node->right_branch_ = Node<T>(node->right_branch_->value_);

    return new_node;
}

template<typename T>
AVL_tree<T>::AVL_tree(const AVL_tree<T> & tree)
{
    root = create_tree(tree.root);
}

template<typename T>
void AVL_tree<T>::delete_all(Node<T> * node)
{
    if (node != nullptr)
    {
        Node<T> * l_branch = node->left_branch_;
        Node<T> * r_branch = node->right_branch_;
        delete node;
        node = nullptr;
        delete_all(l_branch);
        delete_all(r_branch);
    }
}

template<typename T>
AVL_tree<T>::~AVL_tree()
{
    delete_all(root);
}

template<typename T>
AVL_tree<T> & AVL_tree<T>::operator=(const AVL_tree<T> & tree)
{
    if (this == *tree)
        return *this;

    root = tree.root;
    return *this;
}

template<typename T>
bool AVL_tree<T>::is_there(T item) const
{
    Node<T> * current_node = root;

    while(current_node != nullptr)
    {
        if (current_node->value_ == item)
        {
            return true;
        }
        else if (current_node->value_ < item)
        {
            current_node = current_node->right_branch_;
        }
        else
        {
            current_node = current_node->left_branch_;
        }
    }
    return false;
}

template<typename T>
int AVL_tree<T>::height(const Node<T> * node) const
{
    if (node == nullptr)
    {
        return 0;
    }

    int right_height = height(node->right_branch_);
    int left_height = height(node->left_branch_);

    if (right_height >= left_height)
    {
        return right_height + 1;
    }
    else
    {
        return left_height + 1;
    }
}

template<typename T>
void AVL_tree<T>::set_balance(Node<T> * node)
{
    if (node != nullptr)
    {
        node->balance_ = abs(height(node->right_branch_) - height(node->left_branch_)); // always right_branch - left_branch
    }
}

template<typename T>
void AVL_tree<T>::L_rotate(Node<T> ** root_node)
{
    Node<T> * right_branch = (*root_node)->right_branch_;
    Node<T> * left_subtree_of_right_branch = (*root_node)->right_branch_->left_branch_;

    (*root_node)->right_branch_->left_branch_ = *root_node;
    (*root_node)->right_branch_ = left_subtree_of_right_branch;
    *root_node = right_branch;

    // change quantity of elements after rotate
    (*root_node)->left_branch_->elements_ = elements_quantity((*root_node)->left_branch_->left_branch_) +
                                            elements_quantity((*root_node)->left_branch_->right_branch_) + 1;
    (*root_node)->elements_ = elements_quantity((*root_node)->left_branch_) +
                              elements_quantity((*root_node)->right_branch_) + 1;
    // change balances after rotate
    set_balance((*root_node)->left_branch_);
    set_balance(*root_node);
}

template<typename T>
void AVL_tree<T>::R_rotate(Node<T> ** root_node)
{
    Node<T> * left_branch = (*root_node)->left_branch_;
    Node<T> * right_subtree_of_left_branch = (*root_node)->left_branch_->right_branch_;

    (*root_node)->left_branch_->right_branch_ = *root_node;
    (*root_node)->left_branch_ = right_subtree_of_left_branch;
    *root_node = left_branch;

    // change quantity of elements after rotate
    (*root_node)->right_branch_->elements_ = elements_quantity((*root_node)->right_branch_->left_branch_) +
                                             elements_quantity((*root_node)->right_branch_->right_branch_) + 1;
    (*root_node)->elements_ = elements_quantity((*root_node)->left_branch_) +
                              elements_quantity((*root_node)->right_branch_) + 1;
    // change balances after rotate
    set_balance((*root_node)->right_branch_);
    set_balance(*root_node);
}

template<typename T>
void AVL_tree<T>::LR_rotate(Node<T> ** root_node)
{
    Node<T> * last_root_node = *root_node;
    Node<T> * left_branch = (*root_node)->left_branch_;
    Node<T> * right_subtree_of_left_branch = left_branch->right_branch_;

    left_branch->right_branch_ = right_subtree_of_left_branch->left_branch_;
    last_root_node->left_branch_ = right_subtree_of_left_branch->right_branch_;
    right_subtree_of_left_branch->left_branch_ = left_branch;
    right_subtree_of_left_branch->right_branch_ = last_root_node;
    *root_node = right_subtree_of_left_branch;

    // change quantity of elements after rotate
    (*root_node)->left_branch_->elements_ = elements_quantity((*root_node)->left_branch_->left_branch_) +
                                            elements_quantity((*root_node)->left_branch_->right_branch_) + 1;
    (*root_node)->right_branch_->elements_ = elements_quantity((*root_node)->right_branch_->left_branch_) +
                                             elements_quantity((*root_node)->right_branch_->right_branch_) + 1;
    (*root_node)->elements_ = elements_quantity((*root_node)->left_branch_) +
                              elements_quantity((*root_node)->right_branch_) + 1;
    // change balances after rotate
    set_balance((*root_node)->left_branch_);
    set_balance((*root_node)->right_branch_);
    set_balance(*root_node);
    check_and_rotate(&(*root_node)->right_branch_, root_node);
}

template<typename T>
void AVL_tree<T>::RL_rotate(Node<T> ** root_node)
{
    //        A                     C
    //      /   \                 /   \
    //    L      B    ---->     A      B
    //         /   \          /  \    / \
    //       C      R        L    M  N   R
    //      / \
    //    M    N

    Node<T> * left_subtree_of_right_branch =  (*root_node)->right_branch_->left_branch_; // C
    Node<T> * M = (*root_node)->right_branch_->left_branch_->left_branch_;

    (*root_node)->right_branch_->left_branch_ = left_subtree_of_right_branch->right_branch_;
    left_subtree_of_right_branch->right_branch_ = (*root_node)->right_branch_;
    (*root_node)->right_branch_ = M;
    left_subtree_of_right_branch->left_branch_ = *root_node;
    *root_node = left_subtree_of_right_branch;


    // change quantity of elements after rotate
    (*root_node)->left_branch_->elements_ = elements_quantity((*root_node)->left_branch_->left_branch_) +
                                            elements_quantity((*root_node)->left_branch_->right_branch_) + 1;
    (*root_node)->right_branch_->elements_ = elements_quantity((*root_node)->right_branch_->left_branch_) +
                                             elements_quantity((*root_node)->right_branch_->right_branch_) + 1;
    (*root_node)->elements_ = elements_quantity((*root_node)->left_branch_) +
                              elements_quantity((*root_node)->right_branch_) + 1;
    // change balances after rotate
    set_balance((*root_node)->left_branch_);
    set_balance((*root_node)->right_branch_);
    set_balance(*root_node);
    check_and_rotate(&((*root_node)->left_branch_), root_node);
}

template<typename T>
void AVL_tree<T>::check_and_rotate(Node<T> ** node, T item)
{
    if (abs(height((*node)->right_branch_) - height((*node)->left_branch_)) > 1)
    {
           if ((*node)->value_ < item && (*node)->right_branch_->value_ < item)
               L_rotate(node);
           else if ((*node)->value_ > item && (*node)->left_branch_->value_ > item)
               R_rotate(node);
           else if ((*node)->value_ > item && (*node)->left_branch_->value_ < item)
               LR_rotate(node);
           else if ((*node)->value_ < item && (*node)->right_branch_->value_ > item)
               RL_rotate(node);
    }
}

template<typename T>
void AVL_tree<T>::check_and_rotate(Node<T> ** node, Node<T> ** parent)
{
    if (abs(height((*node)->right_branch_) - height((*node)->left_branch_)) > 1)
    {
        if ((*node)->value_ > (*parent)->value_ && height((*node)->right_branch_) > height((*node)->left_branch_))
            L_rotate(node);
        else if ((*node)->value_ < (*parent)->value_ && height((*node)->right_branch_) < height((*node)->left_branch_))
            R_rotate(node);
        else if ((*node)->value_ < (*parent)->value_ && height((*node)->right_branch_) > height((*node)->left_branch_))
            LR_rotate(node);
        else if ((*node)->value_ > (*parent)->value_ && height((*node)->right_branch_) < height((*node)->left_branch_))
            RL_rotate(node);
    }
}

template<typename T>
bool AVL_tree<T>::check_and_rotate(Node<T> ** node)
{
    if (abs(height((*node)->right_branch_) - height((*node)->left_branch_)) > 1)
    {
        if (node == &root)
        {
            if (height((*node)->right_branch_) > height((*node)->left_branch_))
                L_rotate(node);
            else if (height((*node)->right_branch_) < height((*node)->left_branch_))
                R_rotate(node);
        }
        else
        {
            // finding parent and grand_parent
            Node<T> * grand_parent = root;
            Node<T> * parent = root;
            Node<T> * the_same_node = root;

            while(the_same_node != *node)
            {
                grand_parent = parent;
                parent = the_same_node;

                if (the_same_node->value_ < (*node)->value_)
                {
                    the_same_node = the_same_node->right_branch_;
                }
                else
                {
                    the_same_node = the_same_node->left_branch_;
                }
            }
            // finded node with disbalance, its grand_parent and parent
            // rotations depending on the position of the node
            if (grand_parent == parent)
            {
                if (height((*node)->right_branch_) > height((*node)->left_branch_))
                    L_rotate(node);
                else
                    R_rotate(node);
            }
            else
            {
                if ((*node)->value_ > parent->value_ && (*node)->value_ > grand_parent->value_)
                    L_rotate(node);
                else if ((*node)->value_ < parent->value_ && (*node)->value_ < grand_parent->value_)
                    R_rotate(node);
                else if ((*node)->value_ < parent->value_ && (*node)->value_ > grand_parent->value_)
                    LR_rotate(node);
                else if ((*node)->value_ > parent->value_ && (*node)->value_ < grand_parent->value_)
                    RL_rotate(node);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

template<typename T>
void AVL_tree<T>::insert(T item)
{
    if (root == nullptr)
    {
        root = new Node<T>(item);
    }
    else
    {
        if (!is_there(item))
        {
            Node<T> * current_node = root;
            Node<T> * parent = root;
            Node<T> ** grand_parent = &root;

            while(current_node != nullptr)
            {
                grand_parent = &parent;
                parent = current_node;
                parent->elements_ += 1;

                if (current_node->value_ < item)
                {
                    current_node = current_node->right_branch_;
                }
                else
                {
                    current_node = current_node->left_branch_;
                }
            }

            // connect parent and new node
            if (parent->value_ < item)
            {
                parent->right_branch_ = new Node<T>(item);
            }
            else
            {
                parent->left_branch_ = new Node<T>(item);
            }

            check_and_rotate(grand_parent, item);
        }
        else
        {
            std::cerr << "Value " << item << " is already in the tree" << std::endl;
        }

    }
}

template<typename T>
void AVL_tree<T>::remove(T item)
{
    if (is_there(item))
    {
        if (root->value_ == item)
        {
            // if it is a leaf
            if (root->left_branch_ == nullptr && root->right_branch_ == nullptr)
            {
                delete root;
                root = nullptr;
            }
            else if (root->right_branch_ != nullptr && root->left_branch_ == nullptr) // if node has only right_branch
            {
                Node<T> * r_branch = root->right_branch_;
                delete root;
                root = r_branch;
            }
            else if (root->right_branch_ == nullptr && root->left_branch_ != nullptr) // if node has only left_branch
            {
                Node<T> * l_branch = root->left_branch_;
                delete root;
                root = l_branch;
            }
            else// there are 2 branches
            {
                // find min elem in the right subtree
                Node<T> * the_smallest_elem_in_right_branch = root->right_branch_;
                Node<T> * parent_of_the_smallest_elem_in_right_branch = root->right_branch_;

                while(the_smallest_elem_in_right_branch->left_branch_ != nullptr)
                {
                    parent_of_the_smallest_elem_in_right_branch = the_smallest_elem_in_right_branch;
                    the_smallest_elem_in_right_branch = the_smallest_elem_in_right_branch->left_branch_;
                    parent_of_the_smallest_elem_in_right_branch->elements_ -= 1;
                }

                the_smallest_elem_in_right_branch->left_branch_ = root->left_branch_;

                if (the_smallest_elem_in_right_branch == parent_of_the_smallest_elem_in_right_branch)
                {
                    //              root                                    smalest
                    //            /      \            ------->            /        \
                    //      l_branch    smalest                      l_branch     r_branch
                    //                  /     \
                    //                NULL   r_branch

                }
                else
                {
                    //           root                                       smallest
                    //         /      \                    ------>         /        \
                    //   l_branch   parent_of_the_smallest           l_branch     parent_of_the_smallest
                    //                 /       \                                    /               \
                    //             smalest    r_branch                      smallest_r_branch     r_branch
                    //             /    \
                    //          NULL    smallest_r_branch
                    Node<T> * smallest_r_branch = the_smallest_elem_in_right_branch->right_branch_;
                    the_smallest_elem_in_right_branch->right_branch_ = root->right_branch_;
                    parent_of_the_smallest_elem_in_right_branch->left_branch_ = smallest_r_branch;
                    parent_of_the_smallest_elem_in_right_branch->elements_ -= 1;
                }
                delete root;
                root = the_smallest_elem_in_right_branch;
                root->elements_ = elements_quantity(root->left_branch_) + elements_quantity(root->right_branch_) + 1;

                set_balance(parent_of_the_smallest_elem_in_right_branch);
                set_balance(root->right_branch_);
                set_balance(root);
                check_and_rotate(&parent_of_the_smallest_elem_in_right_branch);
                check_and_rotate(&root);
            }
        }
        else
        {
            Node<T> * current_node = root;
            Node<T> * parent = root;
            Node<T> * grand_parent = root;

            while(current_node->value_ != item)
            {
                grand_parent = parent;
                parent = current_node;
                parent->elements_ -= 1;

                if (current_node->value_ < item)
                {
                    current_node = current_node->right_branch_;
                }
                else
                {
                    current_node = current_node->left_branch_;
                }
            }

            // if it is a leaf
            if (current_node->left_branch_ == nullptr && current_node->right_branch_ == nullptr)
            {
                delete current_node;

                if (parent->value_ < item)
                {
                    parent->right_branch_ = nullptr;
                }
                else
                {
                    parent->left_branch_ = nullptr;
                }

                check_and_rotate(&grand_parent);
            }
            else if (current_node->left_branch_ == nullptr) // if node has only right_branch
            {
                //          for_delete
                //          /        \            ------->          r_branch
                //        NULL     r_branch

                Node<T> * r_branch = current_node->right_branch_;
                delete current_node;

                if (parent->value_ < item)
                {
                    parent->right_branch_ = r_branch;
                }
                else
                {
                    parent->left_branch_ = r_branch;
                }

                check_and_rotate(&parent, &grand_parent);
                check_and_rotate(&grand_parent);
            }
            else if (current_node->right_branch_ == nullptr) // if node has only left_branch
            {
                //          for_delete
                //          /        \            ------->          l_branch
                //     l_branch      NULL

                Node<T> * l_branch = current_node->left_branch_;
                delete current_node;

                if (parent->value_ < item)
                {
                    parent->right_branch_ = l_branch;
                }
                else
                {
                    parent->left_branch_ = l_branch;
                }
                // НУЖНО У parent И ПРОВЕРИТЬ grand_parent
                check_and_rotate(&parent, &grand_parent);
                check_and_rotate(&grand_parent);
            }
            else // there are 2 branches
            {
                Node<T> * r_branch = current_node->right_branch_;
                Node<T> * l_branch = current_node->left_branch_;
                delete current_node;

                // find min elem in the right subtree
                Node<T> * the_smallest_elem_in_right_branch = r_branch;
                Node<T> * parent_of_the_smallest_elem_in_right_branch = r_branch;

                while(the_smallest_elem_in_right_branch->left_branch_ != nullptr)
                {
                    parent_of_the_smallest_elem_in_right_branch = the_smallest_elem_in_right_branch;
                    the_smallest_elem_in_right_branch = the_smallest_elem_in_right_branch->left_branch_;
                }

                the_smallest_elem_in_right_branch->left_branch_ = l_branch; // И ТАМ И ТАМ ЭТА СТРОКА

                if (the_smallest_elem_in_right_branch == parent_of_the_smallest_elem_in_right_branch)
                {
                    //          for_delete                                  smalest
                    //          /        \            ------->            /        \
                    //     l_branch     smalest                      l_branch     r_branch
                    //                  /     \
                    //                NULL   r_branch

                }
                else if (the_smallest_elem_in_right_branch != parent_of_the_smallest_elem_in_right_branch)
                {
                    //         for_delete                                      smallest
                    //         /        \                    ------>         /         \
                    //   l_branch     parent_of_the_smallest           l_branch      parent_of_the_smallest
                    //                    /         \                                  /         \
                    //                smalest      r_branch                smallest_r_branch    r_branch
                    //                 /    \
                    //              NULL   smallest_r_branch
                    Node<T> * smallest_r_branch = the_smallest_elem_in_right_branch->right_branch_;
                    the_smallest_elem_in_right_branch->right_branch_ = r_branch;
                    parent_of_the_smallest_elem_in_right_branch->left_branch_ = smallest_r_branch;
                    parent_of_the_smallest_elem_in_right_branch->elements_ -= 1;
                }

                the_smallest_elem_in_right_branch->elements_ = elements_quantity(the_smallest_elem_in_right_branch->left_branch_) +
                                                               elements_quantity(the_smallest_elem_in_right_branch->right_branch_) + 1;

                if (parent->value_ < the_smallest_elem_in_right_branch->value_)
                {
                    parent->right_branch_ = the_smallest_elem_in_right_branch;
                }
                else
                {
                    parent->left_branch_ = the_smallest_elem_in_right_branch;
                }

                set_balance(the_smallest_elem_in_right_branch);
                set_balance(parent);

                // FOR THE CHECKING FIND A NODE
                Node<T> ** the_smallest = &root;
                Node<T> ** parent = &root;
                Node<T> ** grand_parent = &root;

                while((*the_smallest)->value_ != the_smallest_elem_in_right_branch->value_)
                {
                    grand_parent = parent;
                    parent = the_smallest;

                    if ((*the_smallest)->value_ < the_smallest_elem_in_right_branch->value_)
                    {
                        the_smallest = &(*the_smallest)->right_branch_;
                    }
                    else
                    {
                        the_smallest = &(*the_smallest)->left_branch_;
                    }
                }
                check_and_rotate(the_smallest);
                check_and_rotate(parent);
            }
        }
    }
    else
    {
        std::cerr << "There isn`t " << item << " in the tree." << std::endl;
    }
}

template<typename T>
int AVL_tree<T>::size() const
{
    return this->root->elements_;
}

template<typename T>
void AVL_tree<T>::show(Node<T> * node) const
{
    if (node != nullptr)
    {
        show(node->left_branch_);
        std::cout << node->value_ << ' ';
        show(node->right_branch_);
    }
}

template<typename T>
void AVL_tree<T>::show() const
{
    show(root);
    std::cout << '\n';
}

template<typename T>
void AVL_tree<T>::print_n(const Node<T> * node, int n, int level, int prob) const
{
    if (node != nullptr)
    {
        if (level == n)
        {
            for (int i = 1; i <= prob; ++i)
                std::cout << ' ';
            std::cout << node->value_;
        }
        else
        {
            print_n(node->left_branch_, n, level + 1, prob);
            print_n(node->right_branch_, n, level + 1, prob);
        }
    }
}

template<typename T>
void AVL_tree<T>::print(const Node<T> * node) const
{
    int h = height(node);
    int prob = 4;
    if (node != nullptr)
    {
        for (int i = 0; i <= h; ++i)
        {
            print_n(node, i, 0, prob*(h - i));
            std::cout << std::endl;
        }
    }
}

template<typename T>
void AVL_tree<T>::print() const
{
    print(root);
}

template<typename T>
T AVL_tree<T>::min(Node<T> * node) const
{
    Node<T> * current_node = node;

    while(current_node->left_branch_ != nullptr)
        current_node = current_node->left_branch_;

    return current_node->value_;
}

template<typename T>
T AVL_tree<T>::max(Node<T> * node) const
{
    Node<T> * current_node = node;

    while(current_node->right_branch_ != nullptr)
        current_node = current_node->right_branch_;

    return current_node->value_;
}

template<typename T>
int AVL_tree<T>::elements_quantity(Node<T> * node) const
{
    if (node == nullptr)
    {
        return 0;
    }
    else
    {
        return node->elements_;
    }
}

template<typename T>
T AVL_tree<T>::k_th_order_statistic(int i) const
{
    if (i <= 0 || i > elements_quantity(root))
    {
        std::cerr << "Uncorrect element number" << std::endl;
    }
    else
    {
        Node<T> * current_node = root;

        while(true)
        {
            if (elements_quantity(current_node) == i)
            {
                return max(current_node);
            }
            else if (i == 1)
            {
                return min(current_node);
            }
            else if (elements_quantity(current_node->left_branch_) == i)
            {
                return max(current_node->left_branch_);
            }
            else if (elements_quantity(current_node->left_branch_) + 1 == i)
            {
                return current_node->value_;
            }
            else if (elements_quantity(current_node->left_branch_) < i)
            {
                i -= (elements_quantity(current_node->left_branch_) + 1);
                current_node = current_node->right_branch_;
            }
            else if (elements_quantity(current_node->left_branch_) > i)
            {
                current_node = current_node->left_branch_;
            }
        }
    }
}

template<typename T>
int AVL_tree<T>::elem_less_than(T item) const
{
    Node<T> * current_node = root;
    Node<T> * parent = root;
    int count = 0;

    while(current_node != nullptr && current_node->right_branch_ != nullptr)
    {
        if (current_node->value_ >= item)
        {
            parent = current_node;
            current_node = current_node->left_branch_;
        }
        else
        {
            count += current_node->elements_ - current_node->right_branch_->elements_;
            parent = current_node;
            current_node = current_node->right_branch_;
        }
    }

    if (current_node->value_ < item)
    {
        count += current_node->elements_;
    }

    return count;
}

template<typename T>
T AVL_tree<T>::min() const
{
    Node<T> * current_node = root;

    while(current_node->left_branch_ != nullptr)
        current_node = current_node->left_branch_;

    return current_node->value_;
}

template<typename T>
T AVL_tree<T>::max() const
{
    Node<T> * current_node = root;

    while(current_node->right_branch_ != nullptr)
        current_node = current_node->right_branch_;

    return current_node->value_;
}

template<typename T>
std::ostream & operator<<(std::ostream & os, const Node<T> * node)
{
    if (node != nullptr)
    {
        operator<<(os, node->left_branch_);
        os << node->value_ << ' ';
        operator<<(os, node->right_branch_);
    }
    return os;
}

template<typename T>
std::ostream & operator<<(std::ostream & os, const AVL_tree<T> & tree)
{
    os << tree.root;

    return os;
}

#endif
