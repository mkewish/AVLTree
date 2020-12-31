
//----------------------------------------------------------------------
// Author: Makoto Kewish
// File:   avl_collection.h
// Description:
//            This program creates an AVL binary tree key-value
//            pair collection and runs the collection through a series
//            of iterative and recursive operations.
//
//----------------------------------------------------------------------


#ifndef AVL_COLLECTION_H
#define AVL_COLLECTION_H

#include <vector>
#include <algorithm>
#include <string>
#include "collection.h"


template<typename K, typename V>
class AVLCollection : public Collection<K,V>
{
public:

  // create an empty tree
  AVLCollection();

  // tree copy constructor
  AVLCollection(const AVLCollection<K,V>& rhs);

  // tree assignment operator
  AVLCollection<K,V>& operator=(const AVLCollection<K,V>& rhs);

  // delete a tree
  ~AVLCollection();

  // add a new key-value pair into the collection
  void add(const K& a_key, const V& a_val);

  // remove a key-value pair from the collectiona
  void remove(const K& a_key);

  // find and return the value associated with the key
  bool find(const K& search_key, V& the_val) const;

  // find and return the values with keys >= to k1 and <= to k2
  void find(const K& k1, const K& k2, std::vector<V>& vals) const;

  // return all of the keys in the collection
  void keys(std::vector<K>& all_keys) const;

  // return all of the keys in ascending (sorted) order
  void sort(std::vector<K>& all_keys_sorted) const;

  // return the number of key-value pairs in the collection
  int size() const;

  // return the height of the tree
  int height() const;

private:

  // avl tree node structure
  struct Node {
    K key;
    V value;
    int height;
    Node* left;
    Node* right;
  };

  // helper to empty entire tree
  void make_empty(Node* subtree_root);

  // helper to build sorted list of keys (used by keys and sort)
  void inorder(const Node* subtree_root, std::vector<K>& keys) const;

  // helper to recursively find range of values
  void range_search(const Node* subtree_root, const K& k1, const K& k2,
                    std::vector<V>& vals) const;

  // recursively (deep) copy ancestors of src to dst
  void preorder_copy(const Node* subtree_root_src, Node* subtree_root_dst);

  // helper function to remove a node recursively
  Node* remove(const K& key, Node* subtree_root);

  // recursive add helper
  Node* add(Node* subtree_root, const K& a_key, const V& a_val);

  // rotate right helper
  Node* rotate_right(Node* k2);

  // rotate left helper
  Node* rotate_left(Node* k2);

  // rebalance the subtree rooted at subtree_root
  Node* rebalance(Node* subtree_root);

  // number of k-v pairs in the collection
  int tree_size;

  // root node of tree
  Node* root;

  // for testing only: "pretty" prints a tree with node heights
  void print_tree(std::string indent, Node* subtree_root);
};


// constructs an AVL tree key-value pair collection
template<typename K, typename V>
AVLCollection<K,V>::AVLCollection()
{
  root = nullptr;
  tree_size = 0;
}


// copy constructor, utilizes operator=
template<typename K, typename V>
AVLCollection<K,V>::AVLCollection(const AVLCollection<K,V>& rhs)
{
  root = nullptr;
  tree_size = 0;
  *this = rhs;
}


// assignment operator (=)for two unique AVL trees
template<typename K, typename V>
AVLCollection<K,V>& AVLCollection<K,V>::operator=(const AVLCollection<K,V>& rhs)
{
  if(this != &rhs)
  {
    make_empty(root);
    root = nullptr;
    tree_size = 0;
    preorder_copy(rhs.root, root);
  }
  return *this;
}


// destroys an AVL tree collection
template<typename K, typename V>
AVLCollection<K,V>::~AVLCollection()
{
  make_empty(root);
  tree_size = 0;
}


// calls the add helper function to add a node into the collection
template<typename K, typename V>
void AVLCollection<K,V>::add(const K& a_key, const V& a_val)
{
  root = add(root, a_key, a_val);
  //print_tree("", root); // for debugging
  //std::cout << std::endl;
}


// calls the remove helper function to remove a node from the collection
template <typename K, typename V>
void AVLCollection<K,V>::remove(const K& a_key)
{
  root = remove(a_key, root);
  //print_tree("", root); // for debugging
  //std::cout << std::endl;
}


// finds a key-value pair in the collection and returns its associated value
template<typename K, typename V>
bool AVLCollection<K,V>::find(const K& search_key, V& the_val) const
{
  Node* cur = root;
  while(cur != nullptr)
  {
    if(search_key == cur -> key)
    {
      the_val = cur -> value;
      return true;
    }
    else if(search_key < cur -> key)
      cur = cur -> left;
    else if(search_key > cur -> key)
      cur = cur -> right;
  }
  return false;
}


// calls the range-search helper function and collects all the values that fall
// within the two keys
template<typename K, typename V>
void AVLCollection<K,V>::find(const K& k1, const K& k2, std::vector<V>& vals) const
{
  range_search(root, k1, k2, vals);
}


// collections all the keys in the collection (uses in-order traversal)
template<typename K, typename V>
void AVLCollection<K,V>::keys(std::vector<K>& all_keys) const
{
  inorder(root, all_keys);
}


// sorts all the keys in the AVL tree in ascending order (uses in-order traversal)
template<typename K, typename V>
void AVLCollection<K,V>::sort(std::vector<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
}


// returns the number of key-value pairs in the collection
template<typename K, typename V>
int AVLCollection<K,V>::size() const
{
  return tree_size;
}


// returns the height of a tree
template<typename K, typename V>
int AVLCollection<K,V>::height() const
{
  if (!root)
    return 0;
  return root -> height;
}


//------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------


// empties the entire AVL tree using postorder traversal
template<typename K, typename V>
void AVLCollection<K,V>::make_empty(Node* subtree_root)
{
  if(subtree_root == nullptr)
  {
    root = nullptr;
    return;
  }
  make_empty(subtree_root -> left);
  make_empty(subtree_root -> right);
  delete subtree_root;
}

// utilizes the in-order traversal method to collect all the keys in the collection
template<typename K, typename V>
void AVLCollection<K,V>::inorder(const Node* subtree_root, std::vector<K>& keys) const
{
  if(subtree_root == nullptr)
    return;
  inorder(subtree_root -> left, keys);
  keys.push_back(subtree_root -> key);
  inorder(subtree_root -> right, keys);
}


// finds nodes that fall within two keys and pushes its value into a vector collection
template<typename K, typename V>
void AVLCollection<K,V>::range_search(const Node* subtree_root, const K& k1, const K& k2,
                  std::vector<V>& vals) const
{
  if(subtree_root == nullptr)
    return;
  if(subtree_root && k1 <= subtree_root -> key && k2 >= subtree_root -> key)
  {
    vals.push_back(subtree_root -> value);
    if(subtree_root && k1 > subtree_root -> key)
      subtree_root = subtree_root -> right;
    if(subtree_root && k2 < subtree_root -> key)
      subtree_root = subtree_root -> left;
  }
  range_search(subtree_root -> right, k1, k2, vals);
  range_search(subtree_root -> left, k1, k2, vals);
}


// utilizes the preorder traversal method to copy elements into another AVL tree
template<typename K, typename V>
void AVLCollection<K,V>::preorder_copy(const Node* subtree_root_src, Node* subtree_root_dst)
{
  if(subtree_root_src == nullptr)
    return;
  add(subtree_root_src -> key, subtree_root_src -> value);
  preorder_copy(subtree_root_src -> left, subtree_root_dst);
  preorder_copy(subtree_root_src -> right, subtree_root_dst);
}


// rotates nodes right, used for rebalancing
template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rotate_right(Node* k2)
{
  Node * k1 = k2 -> left;
  k2 -> left = k1 -> right;
  k1 -> right = k2;
  return k1;
}


// rotates nodes left, used for rebalancing
template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rotate_left(Node* k2)
{
  Node * k1 = k2 -> right;
  k2 -> right = k1 -> left;
  k1 -> left = k2;
  return k1;
}


// rebalances the tree using rotate left and right operations
template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rebalance(Node* subtree_root)
{
  if(!subtree_root)
    return subtree_root;

  Node* lptr = subtree_root -> left;
  Node* rptr = subtree_root -> right;

  // obtain left and right subtree heights
  int heightL = 0;
  int heightR = 0;
  int tmpHeight;
  if(lptr != nullptr)
    heightL = lptr -> height;
  if(rptr != nullptr)
    heightR = rptr -> height;

  // if left heavy (balance is greater than 1)
  if(heightL - heightR > 1)
  {
    int heightLR = 0;
    int heightLL = 0;
    if(lptr -> left != nullptr)
      heightLL = lptr -> left -> height;
    if(lptr -> right != nullptr)
      heightLR = lptr -> right -> height;

    // if left-right heavy, double rotate
    if(heightLL <= heightLR)
    {
      tmpHeight = lptr -> right -> height;
      subtree_root -> left = rotate_left(subtree_root -> left);
      subtree_root = rotate_right(subtree_root);
      subtree_root -> height++;
      subtree_root -> left -> height--;
      subtree_root -> right -> height = tmpHeight;
    } else {
      tmpHeight = lptr -> left -> height;
      subtree_root = rotate_right(subtree_root);
      subtree_root -> right -> height = tmpHeight;;
    }

    // if right heavy (balance is less than -1)
  } else if(heightL - heightR < -1)
  {
    int heightRL = 0;
    int heightRR = 0;
    if(rptr -> left != nullptr)
      heightRL = rptr -> left -> height;
    if(rptr -> right != nullptr)
      heightRR = rptr -> right -> height;

    // if right-left heavy, double rotate
    if(heightRL > heightRR)
    {
      tmpHeight = rptr -> left -> height;
      subtree_root -> right = rotate_right(subtree_root -> right);
      subtree_root = rotate_left(subtree_root);
      subtree_root -> height++;
      subtree_root -> right -> height--;
      subtree_root -> left -> height = tmpHeight;
    } else {
      tmpHeight = rptr -> right -> height;
      subtree_root = rotate_left(subtree_root);
      subtree_root -> left -> height = tmpHeight;
    }
  }
  return subtree_root;
}


// adds a key-value pair to the collection
template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::add(Node* subtree_root, const K& a_key, const V& a_val)
{
  // if spot is open / null
  if(!subtree_root)
  {
    Node* tmp = new Node;
    tmp -> key = a_key;
    tmp -> value = a_val;
    tmp -> left = nullptr;
    tmp -> right = nullptr;
    subtree_root = tmp;
    subtree_root -> height = 1;
    tree_size++;
  } else {
    if(a_key < subtree_root -> key)
      subtree_root -> left = add(subtree_root -> left, a_key, a_val);
    else
      subtree_root -> right = add(subtree_root -> right, a_key, a_val);

    // reassigns new height values
    int treeH = subtree_root -> height;
    int heightL = 0;
    int heightR = 0;
    if(subtree_root -> left != nullptr)
      heightL = subtree_root -> left -> height;
    if(subtree_root -> right != nullptr)
      heightR = subtree_root -> right -> height;

    int biggerH = std::max(heightL, heightR);
    if(treeH - biggerH <= 0)
      subtree_root -> height++;
  }
  // rebalance after insertion
  return rebalance(subtree_root);
}


// removes a key-value pair from the collection
template <typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::remove(const K& key, Node* subtree_root)
{
  int tmpHeight = subtree_root -> height;
  V val;

  // checks if key is in the collection
  bool found = find(key, val);
  if(found == false)
    return subtree_root;

  if(subtree_root && key < subtree_root -> key)
  {
    subtree_root -> left = remove(key, subtree_root -> left);

    // update the height of ancestor nodes
    if(!subtree_root -> right)
      subtree_root -> height--;
    else if(subtree_root -> left)
    {
      if(subtree_root -> right -> height < subtree_root -> left -> height)
      {
        if(subtree_root -> height - subtree_root -> left -> height > 1)
          subtree_root -> height--;
      } else if(subtree_root -> left -> height == subtree_root -> right -> height) {
        if(subtree_root -> height - subtree_root -> left -> height > 1)
          subtree_root -> height--;
        }
    }
    // rebalances as it backtracks
    subtree_root = rebalance(subtree_root);

  } else if(subtree_root && key > subtree_root -> key)
  {
    subtree_root -> right = remove(key, subtree_root -> right);

    // update the height of ancestor nodes
    if(!subtree_root -> left)
      subtree_root -> height--;
    else if(subtree_root -> right)
    {
      if(subtree_root -> left -> height < subtree_root -> right -> height)
      {
        if(subtree_root -> height - subtree_root -> right -> height > 1)
          subtree_root -> height--;
      } else if(subtree_root -> left -> height == subtree_root -> right -> height) {
        if(subtree_root -> height - subtree_root -> right -> height > 1)
          subtree_root -> height--;
      }
    }
    // rebalances as it backtracks
    subtree_root = rebalance(subtree_root);

  } else if(subtree_root && key == subtree_root -> key) {
    Node* cur = subtree_root;
    int prevHeight;

    // hold on to height of subtree_root
    tmpHeight = subtree_root -> height;

    // if subtree_root doesn't have any children
    if(subtree_root -> left == nullptr && subtree_root -> right == nullptr)
    {
      subtree_root = nullptr;
      delete cur;

    // if subtree_root does have children, then it searches for its inorder successor
    } else if(subtree_root -> left != nullptr && subtree_root -> right != nullptr) {
      Node* prev = subtree_root;
      cur = subtree_root -> right;
      while(cur -> left != nullptr)
      {
        prev = cur;
        cur = cur -> left;
      }

      // checks if height of previous subtree_root will change after removal
      if(prev -> right == nullptr)
        prev -> height--;

      // replaces subtree_root with inorder successor
      if(prev != subtree_root)
        prev -> left = cur -> right;
      cur -> left = subtree_root -> left;
      if(subtree_root -> right != cur)
        cur -> right = subtree_root -> right;
      delete subtree_root;
      subtree_root = cur;

      // reassign new height
      subtree_root -> height = tmpHeight;
      prev = nullptr;
      cur = nullptr;

      // if subtree_root only has one child
    } else {
      if(subtree_root -> left != nullptr)
      {
        subtree_root -> height = subtree_root -> left -> height;
        subtree_root = subtree_root -> left;
        delete cur;
      } else {
        subtree_root -> height = subtree_root -> right -> height;
        subtree_root = subtree_root -> right;
        delete cur;
      }
    }
    tree_size--;
  }
  // final rebalance
  return rebalance(subtree_root);
}


// prints tree using preorder traversal method
template<typename K, typename V>
void AVLCollection<K,V>::print_tree(std::string indent, Node* subtree_root)
{
  if (!subtree_root)
    return;

  std::cout << indent << subtree_root -> key << " (h="
            << subtree_root -> height << ")" << std::endl;
  print_tree(indent + "  ", subtree_root -> left);
  print_tree(indent + "  ", subtree_root -> right);
}


#endif
