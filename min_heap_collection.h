#ifndef MINHEAP_COLLECTION_H
#define MINHEAP_COLLECTION_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#define COUNT 10

template <typename K, typename V>
class MinHeapCollection 
{
	public:
	// create an empty linked list
	MinHeapCollection();													

	// copy a linked list
	MinHeapCollection(const MinHeapCollection<K,V>& rhs);						

	// assign a linked list
	MinHeapCollection<K,V>& operator=(const MinHeapCollection<K,V>& rhs);		

	// delete a linked list
	~MinHeapCollection();													
	
	// insert a key-value pair into the collection
	void insert(const K& key, const V& val);							
	
	// remove the minimum key-value pair from the collection
	void remove_min();													
	
	// returns the smallest key-value pair
	bool find_min(K& key, V& val) const;										
	
	// return all of the keys in ascending (sorted) order
	void sort(std::vector<K>& keys) const;
	
	int size() const;													
	
	void print() const;													
	
	void print2D();		

	int height() const;
	
	
	private:
	
	// heap node structure
	struct Node {
		K key;
		V value;
		Node* parent;
		Node* left;
		Node* right;
	};
	Node* root;
	
	// number of k-v pairs in the collection
	int collection_size;
	
	// helper to recursively empty search tree
	void make_empty(Node* subtree_root);								
	
	// helper to recursively build sorted list of keys
	void preorder(const Node* subtree, std::vector<K>& ks, std::vector<V>& vs) const;		
	
	Node* remove(K& key, Node* subtree_root);
	
	void print2Dutil(Node* subtree_root, int space);					
};

template <typename K, typename V>
MinHeapCollection<K,V>::MinHeapCollection() :
  collection_size(0), root(nullptr)
{ }

template <typename K, typename V>
void MinHeapCollection<K,V>::make_empty(Node* subtree_root) {
	while (collection_size > 0) {
		remove_min();
	}
}

template <typename K, typename V>
MinHeapCollection<K,V>::~MinHeapCollection() {
	make_empty(root);
}

template <typename K, typename V>
MinHeapCollection<K,V>::MinHeapCollection(const MinHeapCollection<K,V>& rhs) : collection_size(0), root(nullptr) {
	*this = rhs;
}

template <typename K, typename V>
MinHeapCollection<K,V>& MinHeapCollection<K,V>::operator=(const MinHeapCollection<K,V>& rhs) {
	if (this == &rhs) {
		return *this;
	}
	// delete current
	make_empty(root);
	// build tree
	std::vector<K> ks;
	std::vector<V> vs;
	preorder(rhs.root, ks, vs);	
	for (int i = 0; i < ks.size(); ++i) {
		insert(ks[i], vs[i]);
	}	
	return *this;
}

template <typename K, typename V>
void MinHeapCollection<K,V>::preorder(const Node* subtree, std::vector<K>& ks, std::vector<V>& vs) const {
	if (subtree == nullptr) {
		return;
	}
	ks.push_back(subtree->key);		// visit node
	vs.push_back(subtree->value);
	preorder(subtree->left, ks, vs);	// traverse left
	preorder(subtree->right, ks, vs);	// traverse right
}
	
template <typename K, typename V>
bool MinHeapCollection<K,V>::find_min(K& key, V& val) const {
	if (!root)
		return false;
	
	key = root -> key;
	val = root -> value;
	return true;
}

template <typename K, typename V>
typename MinHeapCollection<K,V>::Node*
MinHeapCollection<K,V>::remove(K& key, Node* subtree_root) {
	if (!subtree_root) {
		return subtree_root;
	} else if (!subtree_root -> left and !subtree_root -> right) {		// heap size 1
		delete subtree_root;
		subtree_root = nullptr;
		return subtree_root;
	} else {															// heap size >= 3
		int h = height();
		int* direction = new int[h];
		int size = collection_size;
		for (int i = h - 1; i >= 0; i--) {
			direction[i] = size % 2;
			size = std::floor(size/2);
		}
		Node* ptr = root;
		for (int i = 1; i < h - 1; i++) {
			if (direction[i] == 0) {
				ptr = ptr -> left;
			} else if (direction[i] == 1) {
				ptr = ptr -> right;
			}
		}
		if (ptr -> left and !ptr -> right) {
			subtree_root -> key = ptr -> left -> key;
			subtree_root -> value = ptr -> left -> value;
			delete ptr -> left;
			ptr -> left = nullptr;
		} else if (ptr -> left and ptr -> right) {
			subtree_root -> key = ptr -> right -> key;
			subtree_root -> value = ptr -> right -> value;
			delete ptr -> right;
			ptr -> right = nullptr;
		}
		
		
		// Trickle Down
		Node* perc_ptr = subtree_root;
		Node* min_child;
		K tmp_key;
		V tmp_val;
		bool is_valid_heap = false;
		
		if (collection_size == 3) {
			if (subtree_root -> left and !subtree_root -> right and subtree_root -> key > subtree_root -> left -> key) {
				std::swap(subtree_root -> key, subtree_root -> left -> key);
				std::swap(subtree_root -> value, subtree_root -> left -> value);
				return subtree_root;
			}
		}
		
		while (!is_valid_heap) {
			if (perc_ptr -> left and perc_ptr -> right) {
				if (perc_ptr -> left -> key < perc_ptr -> right -> key) {
					min_child = perc_ptr -> left;
				} else {
					min_child = perc_ptr -> right;
				}
			} else {
				break;
			}
			if (perc_ptr -> key > min_child -> key) {
				tmp_key = perc_ptr -> key;
				tmp_val = perc_ptr -> value;
				perc_ptr -> key = min_child -> key;
				perc_ptr -> value = min_child -> value;
				min_child -> key = tmp_key;
				min_child -> value = tmp_val;
				perc_ptr = min_child;
			} else {
				is_valid_heap = true;
			}
		}
	}	
	return subtree_root;
}

template <typename K, typename V>
void MinHeapCollection<K,V>::insert(const K& key, const V& val) {
	
	Node* tmp = new Node;	// new node to be inserted
	tmp -> key = key;
	tmp -> value = val;
	tmp -> left = nullptr;
	tmp -> right = nullptr;
	tmp -> parent = nullptr;
	collection_size++;
	if (!root) {
		root = tmp;
		return;
	} else {
		int h = height();
		int* direction = new int[h];
		int size = collection_size;
		for (int i = h - 1; i >= 0; i--) {
			direction[i] = size % 2;
			size = std::floor(size/2);
		}
		Node* ptr = root;
		for (int i = 1; i < h - 1; i++) {
			if (direction[i] == 0) {
				ptr = ptr -> left;
			} else if (direction[i] == 1) {
				ptr = ptr -> right;
			}
		}
		if (!ptr -> left) {
			ptr -> left = tmp;
			tmp -> parent = ptr;
		} else if (ptr -> left and !ptr -> right) {
			ptr -> right = tmp;
			tmp -> parent = ptr;
		}
		
		// Trickle Up
		K tmp_key;
		V tmp_val;
		while (tmp -> parent and tmp -> parent -> key > tmp -> key) {
			tmp_key = tmp -> parent -> key;
			tmp_val = tmp -> parent -> value;
			tmp -> parent -> key = tmp -> key;
			tmp -> parent -> value = tmp -> value;
			tmp -> key = tmp_key;
			tmp -> value = tmp_val;
			tmp = tmp -> parent;
		}
	}		
}

template <typename K, typename V>
int MinHeapCollection<K,V>::size() const {
	return collection_size;
}

template <typename K, typename V>
void MinHeapCollection<K,V>::print() const {
	std::vector<K> ks;
	preorder(root, ks);
	for (int i = 0; i < size(); ++i) {
		std::cout << ks[i] << " ";
	}
	std::cout << std::endl;
}

template <typename K, typename V>
void MinHeapCollection<K,V>::print2Dutil(Node* subtree_root, int space) {
	if (!subtree_root)
		return;
	
	space += COUNT;
	
	print2Dutil(subtree_root -> right, space);
	
	std::cout << std::endl;
	for (int i = COUNT; i < space; ++i)
		std::cout << " ";
	
	std::cout << subtree_root -> key << "\n";
	
	print2Dutil(subtree_root -> left, space);
}


template <typename K, typename V>
void MinHeapCollection<K,V>::print2D() {
	print2Dutil(root, 0);
}

template <typename K, typename V>
int MinHeapCollection<K,V>::height() const {
	return std::ceil(std::log2(collection_size + 1));
}

template <typename K, typename V>
void MinHeapCollection<K,V>::remove_min() {
	K tmp_key;
	V tmp_val;
	bool is_found = find_min(tmp_key, tmp_val);
	root = remove(tmp_key, root);
	collection_size--;
}

template <typename K, typename V>
void MinHeapCollection<K,V>::sort(std::vector<K>& keys) const {
	MinHeapCollection<K,V> copy(*this);
	K tmp_key;
	V tmp_val;
	
	while (copy.size() > 0) {
		copy.find_min(tmp_key, tmp_val);
		keys.push_back(tmp_key);
		copy.remove_min();
	}
}
	
#endif


	