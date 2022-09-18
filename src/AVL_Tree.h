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
    Node()
    {
        value_ = 0;
        balance_ = 0;
        right_branch_ = nullptr;
        left_branch_ = nullptr;
        elements_ = 1;
    }
    Node(T value)
    {
        value_ = value;
        balance_ = 0;
        right_branch_ = nullptr;
        left_branch_ = nullptr;
        elements_ = 1;
    }
    void delete_all();
};

template<typename T>
void Node<T>::delete_all()
{
    if (this != nullptr)
    {
        Node<T> * l_branch = this->left_branch_;
        Node<T> * r_branch = this->right_branch_;

        delete this;

        l_branch->delete_all();
        r_branch->delete_all();
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
        void check_and_rotate(Node<T> ** node);
        Node<T> * create_tree(Node<T> * node);
        void insert(Node<T> ** node, T item);
        void remove(Node<T> ** node, T item);
        T min(Node<T> * node) const; // finding min element in a branch
        T max(Node<T> * node) const; // finding max element in a branch
        int elements_quantity(Node<T> * node) const;
        void show(Node<T> * node) const;
        void print_n(const Node<T> * node, int n, int level, int prob) const;
        void print(const Node<T> * node) const;
        friend std::ostream & operator<< <T> (std::ostream & os, const Node<T> * node);
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
        friend std::ostream & operator<< <T> (std::ostream & os, const AVL_tree<T> & tree);
};

template<typename T>
void deep_copy(Node<T> * & current, const Node<T> * other)
{
    if (current != nullptr && other != nullptr)
        {
            current->value_ = other->value_;
            current->balance_ = other->balance_;
            deep_copy(current->left_branch_, other->left_branch_);
            deep_copy(current->right_branch_,other->right_branch_);
            current->elements_ = other->elements_;
        }
    else if (current != nullptr && other == nullptr)
    {
        current->delete_all();
    }
    else if (current == nullptr && other != nullptr)
    {
        current = new Node<T>;
        current->value_ = other->value_;
        current->balance_ = other->balance_;
        deep_copy(current->left_branch_, other->left_branch_);
        deep_copy(current->right_branch_, other->right_branch_);
        current->elements_ = other->elements_;
    }
}
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
        new_node->left_branch_ = create_tree(node->left_branch_);

    if (node->right_branch_ != nullptr)
        new_node->right_branch_ = create_tree(node->right_branch_);

    return new_node;
}

template<typename T>
AVL_tree<T>::AVL_tree(const AVL_tree<T> & tree)
{
    root = create_tree(tree.root);
}

template<typename T>
AVL_tree<T>::~AVL_tree()
{
    root->delete_all();
}

template<typename T>
AVL_tree<T> & AVL_tree<T>::operator=(const AVL_tree<T> & tree)
{
    if (this == &tree)
        return *this;

    deep_copy(this->root, const_cast<const Node<T> *>(tree.root));
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
    //        A                     B
    //      /   \                 /   \
    //    L      B     ---->    A      R
    //         /   \          /  \
    //       C      R        L    C

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
    //            A                     B
    //          /   \                 /   \
    //        B      R     ---->    L      A
    //      /   \                        /  \
    //    L      C                      C    R
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
    // combination of small L_rotate for B and R_rotate for A
    //        A                     B
    //      /   \                 /   \
    //    B      R    ---->     C      A
    //  /   \                 /  \    / \
    // L     C               L    M  N   R
    //      / \
    //    M    N
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
}

template<typename T>
void AVL_tree<T>::RL_rotate(Node<T> ** root_node)
{
    // combination of small R_rotate for B and L_rotate for A
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
void AVL_tree<T>::check_and_rotate(Node<T> ** node)
{
    if (abs(height((*node)->right_branch_) - height((*node)->left_branch_)) > 1)
    {
        if (height((*node)->right_branch_) > height((*node)->left_branch_))
        {
            // if there i a disbalance in a right_branch of node
            if (height((*node)->right_branch_->right_branch_) < height((*node)->right_branch_->left_branch_) )
                R_rotate(&(*node)->right_branch_);
            L_rotate(node);
        }
        else
        {
            // if there i a disbalance in a left_branch of node
            if (height((*node)->left_branch_->left_branch_) < height((*node)->left_branch_->right_branch_) )
                L_rotate(&(*node)->left_branch_);
            R_rotate(node);
        }
    }
}

template<typename T>
void AVL_tree<T>::insert(Node<T> ** node, T item)
{
    if (*node == nullptr)
    {
        *node = new Node<T>(item);
    }
    else
    {
        if ((*node)->value_ < item)
        {
            insert(&(*node)->right_branch_, item);
        }
        else if ((*node)->value_ > item)
        {
            insert(&(*node)->left_branch_, item);
        }
        check_and_rotate(node, item);
        set_balance(*node);
        (*node)->elements_ = elements_quantity((*node)->left_branch_) +
                             elements_quantity((*node)->right_branch_) + 1;;
    }
}

template<typename T>
void AVL_tree<T>::insert(T item)
{
    if (!is_there(item))
    {
        insert(&root, item);
    }
    else
    {
        std::cerr << "\nValue " << item << " is already in the tree" << std::endl;
    }
}

template<typename T>
void AVL_tree<T>::remove(Node<T> ** node, T item)
{
    // node for delete was found
    if ((*node)->value_ == item)
    {
        // if it is a leaf
        if ((*node)->left_branch_ == nullptr && (*node)->right_branch_ == nullptr)
        {
            delete *node;
            *node = nullptr;
        }
        // if node has only right_branch
        else if ((*node)->left_branch_ == nullptr)
        {
            //          for_delete
            //          /        \            ------->          r_branch
            //        NULL     r_branch

            Node<T> * r_branch = (*node)->right_branch_;
            delete *node;
            *node = r_branch;
        }
         // if node has only left_branch
        else if ((*node)->right_branch_ == nullptr)
        {
            //          for_delete
            //          /        \            ------->          l_branch
            //     l_branch      NULL

            Node<T> * l_branch = (*node)->left_branch_;
            delete *node;
            *node = l_branch;
        }
        else // there are 2 branches
        {
            Node<T> * r_branch = (*node)->right_branch_;
            Node<T> * l_branch = (*node)->left_branch_;
            delete *node;

            // find min elem in the right subtree
            Node<T> * the_smallest_elem_in_right_branch = r_branch;
            Node<T> * parent_of_the_smallest_elem_in_right_branch = r_branch;

            while(the_smallest_elem_in_right_branch->left_branch_ != nullptr)
            {
                parent_of_the_smallest_elem_in_right_branch = the_smallest_elem_in_right_branch;
                the_smallest_elem_in_right_branch = the_smallest_elem_in_right_branch->left_branch_;
                parent_of_the_smallest_elem_in_right_branch->elements_ -= 1;
            }

            the_smallest_elem_in_right_branch->left_branch_ = l_branch;

            if (the_smallest_elem_in_right_branch != parent_of_the_smallest_elem_in_right_branch)
            {
                //            for_delete                                       smallest
                //          /           \                    ------>         /         \
                //  l_branch            r_branch                       l_branch         r_branch
                //                     /       /_\                                     /        /_\
                //     parent_of_the_smallest                           parent_of_the_smallest
                //          /          /_\                                   /         /_\
                //    smallest                                  smallest_r_branch
                //     /    \
                //  NULL   smallest_r_branch

                Node<T> * smallest_r_branch = the_smallest_elem_in_right_branch->right_branch_;
                the_smallest_elem_in_right_branch->right_branch_ = r_branch;
                parent_of_the_smallest_elem_in_right_branch->left_branch_ = smallest_r_branch;
            }

            *node = the_smallest_elem_in_right_branch;

            check_and_rotate(&parent_of_the_smallest_elem_in_right_branch);
            check_and_rotate(&r_branch);

            (*node)->elements_ = elements_quantity((*node)->left_branch_) +
                                 elements_quantity((*node)->right_branch_) + 1;
            set_balance(*node);
        }
    }
    // finding node for delete
    else
    {
        if ((*node)->value_ < item)
        {
            remove(&(*node)->right_branch_, item);
        }
        else if ((*node)->value_ > item)
        {
            remove(&(*node)->left_branch_, item);
        }

        check_and_rotate(node, item);
        set_balance(*node);
        (*node)->elements_ = elements_quantity((*node)->left_branch_) +
                             elements_quantity((*node)->right_branch_) + 1;
    }
}

template<typename T>
void AVL_tree<T>::remove(T item)
{

    if (is_there(item))
    {
        remove(&root, item);
    }
    else
    {
        std::cerr << "\nThere isn`t " << item << " in the tree." << std::endl;
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
        std::cerr << "Uncorrect element number." << std::endl;
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
