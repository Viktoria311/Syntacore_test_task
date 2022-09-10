#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <iostream>

template<typename T>
struct Node
{
    T value_;
    int balance_;
    Node<T> * right_branch_;
    Node<T> * left_branch_;
    int elements_;
    Node<T> * operator=(const Node<T> * node);
    void delete_all(Node<T> * node);
};

template<typename T>
Node<T> * Node<T>::operator=(const Node<T> * node)
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
class AVL_TREE;

template<typename T>
std::ostream & operator<<(std::ostream & os, const Node<T> * node);

template<typename T>
std::ostream & operator<<(std::ostream & os, const AVL_TREE<T> & tree);


template<typename T>
class AVL_TREE
{
    private:
        Node<T> * root;
        int height(Node<T> * node) const;
        void set_balance(Node<T> * node);
        void L_rotate(Node<T> * root_node);
        void R_rotate(Node<T> * root_node);
        void LR_rotate(Node<T> * root_node);
        void RL_rotate(Node<T> * root_node);
        void check_and_rotate(Node<T> * node, T item);
        Node<T> * create_tree(Node<T> * node);
        Node<T> * create_node(T item) const;
        T min(Node<T> * node) const; // FINDING MIN ELEMENT IN A BRANCH
        T max(Node<T> * node) const; // FINDING MAX ELEMENT IN A BRANCH
        int elements_quantity(Node<T> * node) const;
        void show(Node<T> * node) const;
        void delete_all(Node<T> * node);
    public:
        AVL_TREE();
        AVL_TREE(const AVL_TREE<T> & tree);
        ~AVL_TREE();
        AVL_TREE<T> & operator=(const AVL_TREE<T> & tree);
        bool is_there(T item) const;
        void insert(T item);
        void remove(T item);
        void show() const;
        T smallest_order_statistic(int i) const;
        int elem_less_than(T item) const;
        T min() const; // FINDING MIN ELEMENT IN A TREE
        T max() const; // FINDING MAX ELEMENT IN A TREE
        friend std::ostream & operator<< <T> (std::ostream & os, const Node<T> * node);
        friend std::ostream & operator<< <T> (std::ostream & os, const AVL_TREE<T> & tree);
};

template<typename T>
AVL_TREE<T>::AVL_TREE()
{
    root = nullptr;
}

template<typename T>
Node<T> * AVL_TREE<T>::create_tree(Node<T> * node)
{
    if (node == nullptr) return nullptr;

    Node<T> * new_node = new Node<T>;
    new_node->value_ = node->value_;
    new_node->balance_ = node->balance_;

    if (node->left_branch_ == nullptr)
        new_node->left_branch_ = nullptr;
    else
        new_node->left_branch_ = create_tree(node->left_branch_);

    if (node->right_branch_ == nullptr)
        new_node->right_branch_ = nullptr;
    else
        new_node->right_branch_ = create_tree(node->right_branch_);

    new_node->elements_ = node->elements_;

    return new_node;
}

template<typename T>
AVL_TREE<T>::AVL_TREE(const AVL_TREE<T> & tree)
{
    root = create_tree(tree.root);
}

template<typename T>
void AVL_TREE<T>::delete_all(Node<T> * node)
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
AVL_TREE<T>::~AVL_TREE()
{
    delete_all(root);
}

template<typename T>
AVL_TREE<T> & AVL_TREE<T>::operator=(const AVL_TREE<T> & tree)
{
    if (this == *tree)
        return *this;

    root = tree.root;
    return *this;
}

template<typename T>
bool AVL_TREE<T>::is_there(T item) const
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
int AVL_TREE<T>::height(Node<T> * node) const
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
void AVL_TREE<T>::set_balance(Node<T> * node)
{
    if (node != nullptr)
    {
        node->balance_ = height(node->right_branch_) - height(node->left_branch_); // ALWAYS RIGHT_BRANCH - LEFT_BRANCH
    }
}

template<typename T>
void AVL_TREE<T>::L_rotate(Node<T> * root_node)
{
    Node<T> * right_branch = root_node->right_branch_;
    Node<T> * left_subtree_of_right_branch = right_branch->left_branch_;

    right_branch->left_branch_ = root_node;
    root_node->right_branch_ = left_subtree_of_right_branch;

    root_node = right_branch;
    set_balance(root->left_branch_);
    set_balance(root);
}

template<typename T>
void AVL_TREE<T>::R_rotate(Node<T> * root_node)
{
    Node<T> * left_branch = root_node->left_branch_;
    Node<T> * right_subtree_of_left_branch = left_branch->right_branch_;

    left_branch->right_branch_ = root_node;
    root_node->left_branch_ = right_subtree_of_left_branch;

    root_node = left_branch;
    set_balance(root->right_branch_);
    set_balance(root);
}

template<typename T>
void AVL_TREE<T>::LR_rotate(Node<T> * root_node)
{
    Node<T> * left_branch = root_node->left_branch_;
    Node<T> * right_subtree_of_left_branch = left_branch->right_branch_;

    left_branch->right_branch_ = right_subtree_of_left_branch->left_branch_;
    right_subtree_of_left_branch->left_branch_ = left_branch;
    root_node->left_branch_ = right_subtree_of_left_branch->right_branch_;
    right_subtree_of_left_branch->right_branch_ = root_node;
    root_node = right_subtree_of_left_branch;

    set_balance(root_node->left_branch_);
    set_balance(root_node->right_branch_);
    set_balance(root_node);
}

template<typename T>
void AVL_TREE<T>::RL_rotate(Node<T> * root_node)
{
    Node<T> * right_branch = root_node->right_branch_;
    Node<T> * left_subtree_of_right_branch = right_branch->left_branch_;

    right_branch->left_branch_ = left_subtree_of_right_branch->right_branch_;
    left_subtree_of_right_branch->right_branch_ = right_branch;
    root_node->right_branch_ = left_subtree_of_right_branch->left_branch_;
    left_subtree_of_right_branch->left_branch_ = root_node;
    root_node = left_subtree_of_right_branch;

    set_balance(root_node->left_branch_);
    set_balance(root_node->right_branch_);
    set_balance(root_node);
}

template<typename T>
void AVL_TREE<T>::check_and_rotate(Node<T> * node, T item)
{
    if (height(node->right_branch_) - height(node->left_branch_) > 1 || height(node->right_branch_) - height(node->left_branch_) < -1)
    {
           if (root->value_ < item && node->value_ < item)
               L_rotate(node);
           else if (root->value_ > item && node->value_ > item)
               R_rotate(node);
           else if (root->value_ > item && node->value_ < item)
               LR_rotate(node);
           else // if (root->value_ < item && node->value_ > item)
               RL_rotate(node);
    }
}
template<typename T>
Node<T> * AVL_TREE<T>::create_node(T item) const
{
    auto new_node = new Node<T>;
    new_node->value_ = item;
    new_node->balance_ = 0;
    new_node->left_branch_ = nullptr;
    new_node->right_branch_ = nullptr;
    new_node->elements_ = 1;

    return new_node;
}

template<typename T>
void AVL_TREE<T>::insert(T item)
{
    if (root == nullptr)
    {
        root = create_node(item);
    }
    else
    {
        if (!is_there(item))
        {
            Node<T> * current_node = root;
            Node<T> * parent = root;

            while(current_node != nullptr && current_node->value_ != item)
            {
                if (current_node->value_ < item)
                {
                    parent = current_node;
                    parent->elements_ += 1;
                    current_node = current_node->right_branch_;
                }
                else
                {
                    parent = current_node;
                    parent->elements_ += 1;
                    current_node = current_node->left_branch_;
                }
            }


            if (parent->value_ < item)
            {
                parent->right_branch_ = create_node(item);
            }
            else
            {
                parent->left_branch_ = create_node(item);
            }

            check_and_rotate(parent, item);
        }

    }
}

template<typename T>
void AVL_TREE<T>::remove(T item)
{
    if (is_there(item))
    {
        Node<T> * current_node = root;
        Node<T> * parent = root;

        while(current_node != nullptr && current_node->value != item)
        {
            if (current_node->value_ < item)
            {
                parent = current_node;
                parent->elements_ -= 1;
                current_node = current_node->right_branch_;
            }
            else
            {
                parent = current_node;
                parent->elements_ -= 1;
                current_node = current_node->left_branch_;
            }
        }

        // IF IT IS A LEAF
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
        }
        else if (current_node->left_branch_ == nullptr) // IF NODE HAS ONLY RIGHT BRANCH
        {
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
        }
        else if (current_node->right_branch_ == nullptr) // IF NODE HAS ONLY LEFT BRANCH
        {
            Node<T> * l_branch = current_node->left_branch_;
            delete current_node;

            if (parent->value_ < item)
            {
                parent->right_branch_ = l_branch;
            }
            else
            {
                parent->right_branch_ = l_branch;
            }
        }
        else // THERE ARE 2 BRANCHES
        {
            Node<T> * r_branch = current_node->right_branch_;
            Node<T> * l_branch = current_node->left_branch_;
            delete current_node;

            //FIND MIN ELEM IN THE RIGHT SUBTREE
            Node<T> * the_smalest_elem_in_right_branch = r_branch;
            Node<T> * parent_of_the_smalest_elem_in_right_branch = r_branch;

            while(the_smalest_elem_in_right_branch->left_branch_ != nullptr)
            {
                parent_of_the_smalest_elem_in_right_branch = the_smalest_elem_in_right_branch;
                the_smalest_elem_in_right_branch = the_smalest_elem_in_right_branch->left_branch_;
            }
            the_smalest_elem_in_right_branch->left_branch_ = l_branch;
            the_smalest_elem_in_right_branch->right_branch_ = r_branch;
            parent_of_the_smalest_elem_in_right_branch->left_branch_ = nullptr;
            parent_of_the_smalest_elem_in_right_branch->elements_ -= 1;
            the_smalest_elem_in_right_branch->elements_ = the_smalest_elem_in_right_branch->left_branch_->elements_ +
                                                          the_smalest_elem_in_right_branch->right_branch_->elements_ + 1;

            if (parent->value_ < the_smalest_elem_in_right_branch->value_)
            {
                parent->right_branch_ = the_smalest_elem_in_right_branch;
            }
            else
            {
                parent->left_branch_ = the_smalest_elem_in_right_branch;
            }

            set_balance(the_smalest_elem_in_right_branch);
            set_balance(parent);
        }
        check_and_rotate(parent);
    }
}

template<typename T>
void AVL_TREE<T>::show(Node<T> * node) const
{
    if (node != nullptr)
    {
        show(node->left_branch_);
        std::cout << node->value_ << ' ';
        show(node->right_branch_);
    }
}

template<typename T>
void AVL_TREE<T>::show() const
{
    show(root);
    std::cout << '\n';
}

template<typename T>
T AVL_TREE<T>::min(Node<T> * node) const
{
    Node<T> * current_node = node;

    while(current_node->left_branch_ != nullptr)
        current_node = current_node->left_branch_;

    return current_node->value_;
}

template<typename T>
T AVL_TREE<T>::max(Node<T> * node) const
{
    Node<T> * current_node = node;

    while(current_node->right_branch_ != nullptr)
        current_node = current_node->right_branch_;

    return current_node->value_;
}

template<typename T>
int AVL_TREE<T>::elements_quantity(Node<T> * node) const
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
T AVL_TREE<T>::smallest_order_statistic(int i) const
{
    if (i <= 0 || i > elements_quantity(root))
    {
        std::cout << "Uncorrect element number" << std::endl;
    }
    else
    {
        Node<T> * current_node = root;
        Node<T> * parent = root;

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
int AVL_TREE<T>::elem_less_than(T item) const
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
T AVL_TREE<T>::min() const
{
    Node<T> * current_node = root;

    while(current_node->left_branch_ != nullptr)
        current_node = current_node->left_branch_;

    return current_node->value_;
}

template<typename T>
T AVL_TREE<T>::max() const
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
std::ostream & operator<<(std::ostream & os, const AVL_TREE<T> & tree)
{
    os << tree.root;

    return os;
}

#endif
