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
    int elements_; // quantity of elements in this subtree
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
        int height(Node<T> * node) const;
        void set_balance(Node<T> * node);
        void L_rotate(Node<T> * root_node);
        void R_rotate(Node<T> * root_node);
        void LR_rotate(Node<T> * root_node);
        void RL_rotate(Node<T> * root_node);
        void check_and_rotate(Node<T> * node, T item);
        void check_and_rotate(Node<T> * node, Node<T> * parent);
        Node<T> * create_tree(Node<T> * node);
        Node<T> * create_node(T item) const;
        T min(Node<T> * node) const; // finding min element in a branch
        T max(Node<T> * node) const; // finding max element in a branch
        int elements_quantity(Node<T> * node) const;
        void show(Node<T> * node) const;
        void delete_all(Node<T> * node);
    public:
        AVL_tree();
        AVL_tree(const AVL_tree<T> & tree);
        ~AVL_tree();
        AVL_tree<T> & operator=(const AVL_tree<T> & tree);
        bool is_there(T item) const;
        void insert(T item);
        void remove(T item);
        void show() const;
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
int AVL_tree<T>::height(Node<T> * node) const
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
        node->balance_ = height(node->right_branch_) - height(node->left_branch_); // always right_branch - left_branch
    }
}

template<typename T>
void AVL_tree<T>::L_rotate(Node<T> * root_node)
{
    Node<T> * last_root_node = root_node;
    Node<T> * right_branch = root_node->right_branch_;
    Node<T> * left_subtree_of_right_branch = right_branch->left_branch_;


    last_root_node->right_branch_ = left_subtree_of_right_branch;
    right_branch->left_branch_ = last_root_node;
    root_node = right_branch;

    // change quantity of elements after rotate
    root_node->left_branch_->elements_ = elements_quantity(root_node->left_branch_->left_branch_) +
                                         elements_quantity(root_node->left_branch_->right_branch_) + 1;
    root_node->elements_ = elements_quantity(root_node->left_branch_) +
                           elements_quantity(root_node->right_branch_) + 1;
    // change balances after rotate
    set_balance(root_node->left_branch_);
    set_balance(root_node);
}

template<typename T>
void AVL_tree<T>::R_rotate(Node<T> * root_node)
{
    Node<T> * last_root_node = root_node;
    Node<T> * left_branch = root_node->left_branch_;
    Node<T> * right_subtree_of_left_branch = left_branch->right_branch_;

    last_root_node->left_branch_ = right_subtree_of_left_branch;
    left_branch->right_branch_ = last_root_node;
    root_node = left_branch;

    // change quantity of elements after rotate
    root_node->right_branch_->elements_ = elements_quantity(root_node->right_branch_->left_branch_) +
                                          elements_quantity(root_node->right_branch_->right_branch_) + 1;
    root_node->elements_ = elements_quantity(root_node->left_branch_) +
                           elements_quantity(root_node->right_branch_) + 1;
    // change balances after rotate
    set_balance(root_node->right_branch_);
    set_balance(root_node);
}

template<typename T>
void AVL_tree<T>::LR_rotate(Node<T> * root_node)
{
    Node<T> * last_root_node = root_node;
    Node<T> * left_branch = root_node->left_branch_;
    Node<T> * right_subtree_of_left_branch = left_branch->right_branch_;

    left_branch->right_branch_ = right_subtree_of_left_branch->left_branch_;
    last_root_node->left_branch_ = right_subtree_of_left_branch->right_branch_;
    right_subtree_of_left_branch->left_branch_ = left_branch;
    right_subtree_of_left_branch->right_branch_ = last_root_node;
    root_node = right_subtree_of_left_branch;

    // change quantity of elements after rotate
    root_node->left_branch_->elements_ = elements_quantity(root_node->left_branch_->left_branch_) +
                                         elements_quantity(root_node->left_branch_->right_branch_) + 1;
    root_node->right_branch_->elements_ = elements_quantity(root_node->right_branch_->left_branch_) +
                                          elements_quantity(root_node->right_branch_->right_branch_) + 1;
    root_node->elements_ = elements_quantity(root_node->left_branch_) +
                           elements_quantity(root_node->right_branch_) + 1;
    // change balances after rotate
    set_balance(root_node->left_branch_);
    set_balance(root_node->right_branch_);
    set_balance(root_node);
    check_and_rotate(root_node->right_branch_, root_node);
}

template<typename T>
void AVL_tree<T>::RL_rotate(Node<T> * root_node)
{
    Node<T> * last_root_node = root_node;
    Node<T> * right_branch = root_node->right_branch_;
    Node<T> * left_subtree_of_right_branch = right_branch->left_branch_;

    right_branch->left_branch_ = left_subtree_of_right_branch->right_branch_;
    last_root_node->right_branch_ = left_subtree_of_right_branch->left_branch_;
    left_subtree_of_right_branch->right_branch_ = right_branch;
    left_subtree_of_right_branch->left_branch_ = last_root_node;
    root_node = left_subtree_of_right_branch;

    // change quantity of elements after rotate
    root_node->left_branch_->elements_ = elements_quantity(root_node->left_branch_->left_branch_) +
                                         elements_quantity(root_node->left_branch_->right_branch_) + 1;
    root_node->right_branch_->elements_ = elements_quantity(root_node->right_branch_->left_branch_) +
                                          elements_quantity(root_node->right_branch_->right_branch_) + 1;
    root_node->elements_ = elements_quantity(root_node->left_branch_) +
                           elements_quantity(root_node->right_branch_) + 1;
    // change balances after rotate
    set_balance(root_node->left_branch_);
    set_balance(root_node->right_branch_);
    set_balance(root_node);
    check_and_rotate(root_node->left_branch_, root_node);
}

template<typename T>
void AVL_tree<T>::check_and_rotate(Node<T> * node, T item)
{
    if (height(node->right_branch_) - height(node->left_branch_) > 1 || height(node->right_branch_) - height(node->left_branch_) < -1)
    {
           if (node->value_ < item && node->right_branch_->value_ < item)
               L_rotate(node);
           else if (node->value_ > item && node->left_branch_->value_ > item)
               R_rotate(node);
           else if (node->value_ > item && node->left_branch_->value_ < item)
               LR_rotate(node);
           else if (node->value_ < item && node->right_branch_->value_ > item)
               RL_rotate(node);
    }
}

template<typename T>
void AVL_tree<T>::check_and_rotate(Node<T> * node, Node<T> * parent)
{
    if (height(node->right_branch_) - height(node->left_branch_) > 1 || height(node->right_branch_) - height(node->left_branch_) < -1)
    {
        if (node->value_ > parent->value_ && height(node->right_branch_) > height(node->left_branch_))
            L_rotate(node);
        else if (node->value_ < parent->value_ && height(node->right_branch_) < height(node->left_branch_))
            R_rotate(node);
        else if (node->value_ < parent->value_ && height(node->right_branch_) > height(node->left_branch_))
            LR_rotate(node);
        else if (node->value_ > parent->value_ && height(node->right_branch_) < height(node->left_branch_))
            RL_rotate(node);
    }
}

template<typename T>
Node<T> * AVL_tree<T>::create_node(T item) const
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
void AVL_tree<T>::insert(T item)
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
            Node<T> * grand_parent = root;

            while(current_node != nullptr)
            {
                grand_parent = parent;
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
                parent->right_branch_ = create_node(item);
            }
            else
            {
                parent->left_branch_ = create_node(item);
            }

            check_and_rotate(grand_parent, item);
        }

    }
}

template<typename T>
void AVL_tree<T>::remove(T item)
{
    if (is_there(item))
    {
        Node<T> * current_node = root;
        Node<T> * parent = root;
        Node<T> * grand_parent = root;

        while(current_node->value != item)
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
        }
        else if (current_node->left_branch_ == nullptr) // if node has only right_branch
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
        else if (current_node->right_branch_ == nullptr) // if node has only left_branch
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
        else // there are 2 branches
        {
            Node<T> * r_branch = current_node->right_branch_;
            Node<T> * l_branch = current_node->left_branch_;
            delete current_node;

            // find min elem in the right subtree
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
        check_and_rotate(grand_parent);
    }
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
        std::cout << "Uncorrect element number" << std::endl;
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
