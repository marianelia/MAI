#include<iostream>
#include <vector>
#include <algorithm>
#include "avl.h"

using namespace std;

AvlTreeId::~AvlTreeId(){
	delete_node(root);
}

void AvlTreeId::insert(int id){
	root = insert(root, id);
}

void AvlTreeId::erase(int id){
	root = remove(root, id);
}

TreeNode* AvlTreeId::find(int id){
	return find(root, id);
}

void AvlTreeId::delete_node(TreeNode* node) {
    if(node == nullptr) { return; }
    delete_node(node->right);
    delete_node(node->left);
    delete node;
}

vector<int> AvlTreeId::get_nodes() const {
    vector<int> result;
    get_nodes(root, result);
    return result;
}

void AvlTreeId::get_nodes(TreeNode* node, std::vector<int>& v) const {
    if(node == nullptr) { return; }
    get_nodes(node->left,v);
    v.push_back(node->data);
    get_nodes(node->right, v);
}


TreeNode* AvlTreeId::find(TreeNode* r, int id){
	if(r == nullptr || r->data == id) {
		 return r; 
	}
	if(id < r->data) {
		 return find(r->left, id); 
	}
	if(id > r->data) {
		 return find(r->right, id);
	}
}

int AvlTreeId::height(TreeNode* node){
	int h = 0;
	if(node != nullptr){
		int l_height = height(node->left);
		int r_height = height(node->right);
		int max_height = std::max(l_height, r_height);
		h = max_height + 1;
	}
	return h;
}

int AvlTreeId::diff(TreeNode* node){
	int l_height = height(node->left);
	int r_height = height(node->right);
	int b_factor = l_height - r_height;
	return b_factor;
}

TreeNode* AvlTreeId::rr_rotation(TreeNode* parent){
	TreeNode* node;
	node = parent->right;
	parent->right = node->left;
	node->left = parent;
	return node;
}

TreeNode* AvlTreeId::ll_rotation(TreeNode* parent){
	TreeNode* node;
	node = parent->left;
	parent->left = node->right;
	node->right = parent;
	return node;
}

TreeNode* AvlTreeId::lr_rotation(TreeNode* parent){
	TreeNode* node;
	node = parent->left;
	parent->left = rr_rotation(node);
	return ll_rotation(parent);
}

TreeNode* AvlTreeId::rl_rotation(TreeNode* parent){
	TreeNode* node;
	node = parent->right;
	parent->right = ll_rotation(node);
	return rr_rotation(parent);
}

TreeNode* AvlTreeId::balance(TreeNode* node){
	int bal_factor = diff(node);
	if(bal_factor > 1){
		if(diff(node->left) > 0) {
			 node = ll_rotation(node); 
		}
		else {
			 node = lr_rotation(node); 
		}
	}
	else if(bal_factor < -1){
		if(diff(node->right) > 0) {
			 node = rl_rotation(node); 
		}
		else {
			 node = rr_rotation(node); 
		}
	}
	return node;
}

TreeNode* AvlTreeId::find_min(TreeNode* node){
	if(node == nullptr) { return nullptr; }
	else if(node->left == nullptr) { return node; }
	else { return find_min(node->left); }
}

TreeNode* AvlTreeId::insert(TreeNode* root, int val){
	if(root == nullptr){
		root = new TreeNode;
		root->data = val;
		root->left = nullptr;
		root->right = nullptr;
		return root;
	}
	else if(val < root->data){
		root->left = insert(root->left, val);
		root = balance(root);
	}
	else if(val >= root->data){
		root->right = insert(root->right, val);
		root = balance(root);
	}
	return root;
}

TreeNode* AvlTreeId::remove(TreeNode* root_node, int val){
	TreeNode* node;
	if(root_node == nullptr) {
		 return nullptr;
	}
	else if(val < root_node->data) {
		 root_node->left = remove(root_node->left, val); 
	}
	else if(val >root_node->data) {
		 root_node->right = remove(root_node->right, val); 
	}
	else if(root_node->left && root_node->right) {
		node = find_min(root_node->right);
		root_node->data = node->data;
		root_node->right = remove(root_node->right, root_node->data);
	}
	else {
		node = root_node;
		if(root_node->left == nullptr) {
			 root_node = root_node->right; 
		}
		else if(root_node->right == nullptr) {
			 root_node = root_node->left; 
		}
		delete node;
	}
	if(root_node == nullptr) {
		 return root_node; 
	}
	root_node = balance(root_node);
}
