/*******************************************************************************
 * Name        : rbtree.h
 * Author      : Michael Novak & Josh Pirog
 * Version     : 1.0
 * Date        : 11/20/18
 * Description : Implementation of red-black tree.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#ifndef RBTREE_H_
#define RBTREE_H_

#include "node.h"
#include "tree.h"
#include "treeprinter.h"
#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>

// Forward declaration
template <typename K, typename V>
class RedBlackTree;

/**
 * tree_exception class
 * Demonstrates how you can write your own custom exceptions in C++.
 */
class tree_exception: public std::exception {
public:
    tree_exception(const std::string &message) : message_{message} { }

    ~tree_exception() throw() { }

    virtual const char* what() const throw() {
        return message_.c_str();
    }

private:
    std::string message_;
};

template <typename K, typename V>
class RedBlackTreeIterator {
public:
    /**
     * Constructor
     */
    RedBlackTreeIterator() : node_ptr{NULL}, tree{NULL} { }

    /**
     * Equality operator. Compares node pointers.
     */
    bool operator==(const RedBlackTreeIterator &rhs) const {
        return node_ptr == rhs.node_ptr;
    }

    /**
     * Inequality operator. Compares node pointers.
     */
    bool operator!=(const RedBlackTreeIterator &rhs) const {
        return node_ptr != rhs.node_ptr;
    }

    /**
     * Dereference operator. Returns a reference to the Node pointed to
     * by node_ptr.
     */
    Node<K, V>& operator*() const {
        return *node_ptr;
    }

    /**
     * Dereference operator. Returns a pointer to the Node pointed to
     * by node_ptr.
     */
    Node<K, V>* operator->() const {
        return node_ptr;
    }

    /**
     * Preincrement operator. Moves forward to next larger value.
     */
    RedBlackTreeIterator& operator++() {
        Node<K, V> *p;

        if (node_ptr == NULL) {
            // ++ from end(). Get the root of the tree.
            node_ptr = tree->root_;

            // Error, ++ requested for an empty tree.
            if (node_ptr == NULL)
                throw tree_exception(
                        "RedBlackTreeIterator operator++(): tree empty");

            // Move to the smallest value in the tree, which is the first node
            // in an inorder traversal.
            while (node_ptr->left != NULL) {
                node_ptr = node_ptr->left;
            }
        } else {
            if (node_ptr->right != NULL) {
                // Successor is the leftmost node of right subtree.
                node_ptr = node_ptr->right;

                while (node_ptr->left != NULL) {
                    node_ptr = node_ptr->left;
                }
            } else {
                // Have already processed the left subtree, and
                // there is no right subtree. Move up the tree,
                // looking for a parent for which node_ptr is a left child,
                // stopping if the parent becomes NULL (or in this case,
                // root_parent_. A non-NULL parent is the successor. If parent
                // is NULL, the original node was the last node inorder, and
                // its successor is the end of the list.
                p = node_ptr->parent;
                while (p != NULL && node_ptr == p->right) {
                    node_ptr = p;
                    p = p->parent;
                }

                // If we were previously at the rightmost node in
                // the tree, node_ptr = NULL, and the iterator specifies
                // the end of the list.
                node_ptr = p;
            }
        }

        return *this;
    }

    /**
     * Postincrement operator. Moves forward to next larger value.
     */
    RedBlackTreeIterator operator++(int) {
        RedBlackTreeIterator tmp(*this);
        operator++(); // prefix-increment this instance
        return tmp;   // return value before increment
    }

private:
    // node_ptr is the current location in the tree. We can move
    // freely about the tree using left, right, and parent.
    // tree is the address of the RedBlackTree object associated
    // with this iterator. It is used only to access the
    // root pointer, which is needed for ++ and --
    // when the iterator value is end().
    Node<K, V> *node_ptr;
    RedBlackTree<K, V> *tree;
    friend class RedBlackTree<K, V>;

    /**
     * Constructor used to construct an iterator return value from a tree
     * pointer.
     */
    RedBlackTreeIterator(Node<K, V> *p, RedBlackTree<K, V> *t) :
        node_ptr(p), tree(t) { }
};

template<typename K, typename V>
class RedBlackTree : public Tree {
public:
    typedef RedBlackTreeIterator<K, V> iterator;

    /**
     * Constructor to create an empty red-black tree.
     */
    RedBlackTree() : root_{NULL}, size_{0} { }

    /**
     * Constructor to create a red-black tree with the elements from the
     * vector.
     */
    RedBlackTree(std::vector< std::pair<K, V> > &elements) :
                                            root_(NULL), size_(0) {
        insert_elements(elements);
    }

    /**
     * Destructor.
     */
    ~RedBlackTree() {
        delete_tree(root_);
    }

    /**
     * Inserts elements from the vector into the red-black tree.
     * Duplicate elements are not inserted.
     */
    void insert_elements(std::vector< std::pair<K, V> > &elements) {
        for (size_t i = 0, len = elements.size(); i < len; ++i) {
            try {
                insert(elements[i].first, elements[i].second);
            } catch (const tree_exception &te) {
                std::cerr << "Warning: " << te.what() << std::endl;
            }
        }
    }

    /**
     * Inserts a key-value pair into the red black tree.
     * const iterator &it indicates where to start the search for the place to
     * insert the node. If it == end(), the search starts at the root.
     */
    void insert(const iterator &it, const std::pair<K, V> &key_value) {
        // TODO
        const K& key = key_value.first;
        const V& value = key_value.second;
        Node<K, V> *x, *y;

        if (find(key) != end()) {
        	std::ostringstream oss;
        	oss << "Attempt to insert duplicate key '" << key << "'.";
        	throw tree_exception(oss.str());
        }

        if (it != end()) {
        	x = it.node_ptr;
        	y = x->parent;
        } else {
        	x = root_;
        	y = NULL;
        }

        while (x != NULL) {
        	y = x;
        	if (key < x->key()) {
        		x = x->left;
        	} else {
        		x = x->right;
        	}
        }
        Node<K, V> *z = new Node<K, V>(key, value);
        z->parent = y;
        if (y == NULL) {
        	root_ = z;
        } else if (z->key() < y->key()) {
        	y->left = z;
        } else {
        	y->right = z;
        }
        z->left = NULL;
        z->right = NULL;
        z->color = RED;
        insert_fixup(z);
        ++size_;
    }

    /**
     * Inserts a key-value pair into the red-black tree.
     */
    void insert(const K &key, const V &value) {
        iterator e = end();
        insert(e, std::pair<K, V>(key, value));
    }

    /**
     * Returns an ASCII representation of the red-black tree.
     */
    std::string to_ascii_drawing() {
        BinaryTreePrinter<K, V> printer(root_);
        return printer.to_string();
    }

    /**
     * Returns the height of the red-black tree.
     */
    int height() const {
        return height(root_);
    }

    /**
     * Returns the number of nodes in the red-black tree.
     */
    size_t size() const {
        return size_;
    }

    /**
     * Returns the leaf count of the red-black tree.
     */
    size_t leaf_count() const {
        return leaf_count(root_);
    }

    /**
     * Returns the internal node count of the red-black tree.
     */
    size_t internal_node_count() const {
        return internal_node_count(root_);
    }

    /**
     * Returns the diameter of the red-black tree.
     */
    size_t diameter() const {
        return diameter(root_);
    }

    /**
     * Returns the max width of the red-black tree, i.e. the largest number of
     * nodes on any level.
     */
    size_t max_width() const {
        size_t max_width = 0;
        for (int i = 0, h = height(root_) + 1; i < h; ++i) {
            size_t w = width(root_, i);
            if (w > max_width) {
                max_width = w;
            }
        }
        return max_width;
    }

    /**
     * Returns the successful search cost, i.e. the average number of nodes
     * visited to find a key that is present.
     */
    double successful_search_cost() const {
        return size_ == 0 ? 0 : 1 + (double)sum_levels() / size_;
    }

    /**
     * Returns the unsuccessful search cost, i.e. the average number of nodes
     * visited to find a key that is not present.
     */
    double unsuccessful_search_cost() const {
        return (double)sum_null_levels() / null_count();
    }

    /**
     * Searches for item. If found, returns an iterator pointing
     * at it in the tree; otherwise, returns end().
     */
    iterator find(const K &key) {
        Node<K, V> *x = root_;
        while (x != NULL) {
            const K& current_key = x->key();
            if (key == current_key) {
                break; // Found!
            } else if (key < current_key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return iterator(x, this);
    }

    /**
     * Returns an iterator pointing to the first item in order.
     */
    iterator begin() {
        Node<K, V> *curr = root_;

        // if the tree is not empty, the first node
        // in order is the farthest node left from root
        if (curr != NULL) {
            while (curr->left != NULL) {
                curr = curr->left;
            }
        }

        // build return value using private constructor
        return iterator(curr, this);
    }

    /**
     * Returns an iterator pointing just past the end of the tree data.
     */
    iterator end() {
        return iterator(NULL, this);
    }

private:
    Node<K, V> *root_;
    size_t size_;
    friend class RedBlackTreeIterator<K, V>;

    /**
     * Deletes all nodes from the red-black tree.
     */
    void delete_tree(Node<K, V> *n) {
        // TODO
    	if (n == NULL) {
    		return;
    	}
    	if (n->left != NULL) {
    		delete_tree(n->left);
    	}
    	if (n->right != NULL) {
    		delete_tree(n->right);
    	}
    	delete n;
    }

    /**
     * Fixup method described on p. 316 of CLRS.
     */
    void insert_fixup(Node<K, V> *z) {
    	// TODO
    	if (z == root_) {
    		z->color = BLACK;
    	}
    	while (z != root_ && z->parent->color == RED && z->color == RED) {
    		Node<K, V> *y;  // z's uncle

    		if (z->parent == z->parent->parent->left) {
    			y = z->parent->parent->right;

    			if (y == NULL) {
    				if (z == z->parent->right) {
    					z = z->parent;
    					left_rotate(z);
    				} else {
    					z->parent->color = BLACK;
    					z->parent->parent->color = RED;
    					right_rotate(z->parent->parent);
    				}
    			} else {
    				if (y->color == RED) {
    					z->parent->color = BLACK;
    					y->color = BLACK;
    					z->parent->parent->color = RED;
    					z = z->parent->parent;
    				} else if (z == z->parent->right) {
    					z = z->parent;
    					left_rotate(z);
    				} else {
    					z->parent->color = BLACK;
    					z->parent->parent->color = RED;
    					right_rotate(z->parent->parent);
    				}
    			}
    		} else {
    			y = z->parent->parent->left;

    			if (y == NULL) {
    				if (z == z->parent->left) {
    					z = z->parent;
    					right_rotate(z);
    				} else {
    					z->parent->color = BLACK;
    					z->parent->parent->color = RED;
    					left_rotate(z->parent->parent);
    				}
    			} else {
    				if (y->color == RED) {
    					z->parent->color = BLACK;
    					y->color = BLACK;
    					z->parent->parent->color = RED;
    					z = z->parent->parent;
    				} else if (z == z->parent->left) {
    					z = z->parent;
    					right_rotate(z);
    				} else {
    					z->parent->color = BLACK;
    					z->parent->parent->color = RED;
    					left_rotate(z->parent->parent);
    				}
    			}
    		}
    	}

    	// Last line below
    	root_->color = BLACK;
    }

    /**
     * Left-rotate method described on p. 313 of CLRS.
     */
    void left_rotate(Node<K, V> *x) {
        // TODO
    	x = static_cast<Node<K, V>*>(x);

    	Node<K, V> *y = static_cast<Node<K, V>*>(x->right); // set y
    	x->right = y->left; // turn y�s left subtree into x�s right subtree
    	if (y->left != NULL) {
    		y->left->parent = x;
    	}
    	y->parent = x->parent; // link x's parent to y
    	if (x->parent == NULL) {
    		root_ = y;
    	} else if (x == x->parent->left) {
    		x->parent->left = y;
    	} else {
    		x->parent->right = y;
    	}
    	y->left = x; // put x on y's left
    	x->parent = y;
    }

    /**
     * Right-rotate method described on p. 313 of CLRS.
     */
    void right_rotate(Node<K, V> *x) {
        // TODO
    	x = static_cast<Node<K, V>*>(x);

    	Node<K, V> *y = static_cast<Node<K, V>*>(x->left); // set y
    	x->left = y->right; // turn y's right subtree into x's left subtree
    	if (y->right != NULL) {
    		y->right->parent = x;
    	}
    	y->parent = x->parent; // link x's parent to y
    	if (x->parent == NULL) {
    		root_ = y;
    	} else if (x == x->parent->right) {
    		x->parent->right = y;
    	} else {
    		x->parent->left = y;
    	}
    	y->right = x; // put x on y's right
    	x->parent = y;
    }

    /**
     * Returns the height of the red-black tree starting at node.
     * A null node starts at height -1.
     */
    int height(Node<K, V> *node) const {
        // TODO
    	if (node == NULL) {
    		return -1;
    	}
    	return 1 + std::max(height(node->left), height(node->right));
    }

    /**
     * Returns the count of leaves in the red-black tree starting at node.
     * For this method, a leaf is a non-null node that has no children.
     */
    size_t leaf_count(Node<K, V> *node) const {
        // TODO
    	if (node == NULL) {
    		return 0;
    	}
    	return (node->left == NULL && node->right == NULL)
    			+ leaf_count(node->left) + leaf_count(node->right);
    }

    /**
     * Returns the count of internal nodes in the red-black tree starting at
     * node.
     * An internal node has at least one child.
     */
    size_t internal_node_count(Node<K, V> *node) const {
        // TODO
    	if (node == NULL) {
    		return 0;
    	}
    	return (node->left != NULL || node->right != NULL)
    			+ internal_node_count(node->left)
				+ internal_node_count(node->right);
    }

    /**
     * Returns the diameter of the red-black tree rooted at node.
     * Diameter is defined as the number of nodes on the longest path between
     * two (non-null) leaves in the tree. The path does not necessarily have to
     * pass through the root.
     */
    int diameter(Node<K, V> *node) const {
        // TODO
    	if (node == NULL) {
    		return 0;
    	}
    	return std::max((2 + height(node->left) + height(node->right)),
    			std::max(diameter(node->left), diameter(node->right)));
    }

    /**
     * Returns the width of the red-black tree at the designated level.
     * Width is defined as the number of nodes residing at a level.
     */
    size_t width(Node<K, V> *node, size_t level) const {
        // TODO
    	if (node == NULL) {
    		return 0;
    	}
    	if (level == 0) {
    		return 1;
    	}
    	return width(node->left, level - 1) + width(node->right, level - 1);
    }

    size_t null_count() const {
        return null_count(root_);
    }

    /**
     * Returns the count of null nodes in the red-black tree starting at node.
     */
    size_t null_count(Node<K, V> *node) const {
        // TODO
    	if (node == NULL) {
    		return 1;
    	}
    	return null_count(node->right) + null_count(node->left);
    }

    size_t sum_levels() const {
        return sum_levels(root_, 0);
    }

    /**
     * Returns the sum of the levels of each non-null node in the red-black
     * tree starting at node.
     * For example, the tree
     *   5 <- level 0
     *  / \
     * 2   8 <- level 1
     *      \
     *       10 <- level 2
     * has sum 0 + 2(1) + 2 = 4.
     */
    size_t sum_levels(Node<K, V> *node, size_t level) const {
        // TODO
    	if (node == NULL) {
    		return 0;
    	}
    	return level + sum_levels(node->left, level + 1)
    			+ sum_levels(node->right, level + 1);
    }

    size_t sum_null_levels() const {
        return sum_null_levels(root_, 0);
    }

    /**
     * Returns the sum of the levels of each null node in the red-black tree
     * starting at node.
     * For example, the tree
     *    5 <- level 0
     *   / \
     *  2   8 <- level 1
     * / \ / \
     * * * * 10 <- level 2
     *       / \
     *       * * <- level 3
     * has sum 3(2) + 2(3) = 12.
     */
    size_t sum_null_levels(Node<K, V> *node, size_t level) const {
        // TODO
    	if (node == NULL) {
    		return level;
    	}
    	return sum_null_levels(node->left, level + 1)
    			+ sum_null_levels(node->right, level + 1);
    }
};

#endif /* RBTREE_H_ */
