#pragma once

struct TreeNode {
	int data;
	TreeNode* left;
	TreeNode* right; 
};

class AvlTreeId {
public:
	void insert(int);
	void erase(int);
	TreeNode* find(int);
	std::vector<int> get_nodes() const;
	~AvlTreeId();
private:
    TreeNode* root = nullptr;
    int height(TreeNode*);
	int diff(TreeNode*);
	TreeNode* rr_rotation(TreeNode*);
	TreeNode* ll_rotation(TreeNode*);
	TreeNode* lr_rotation(TreeNode*);
	TreeNode* rl_rotation(TreeNode*);
	TreeNode* balance(TreeNode *);
	TreeNode* insert(TreeNode *, int);
	TreeNode* remove(TreeNode* t, int);
	TreeNode* find(TreeNode*, int);
	TreeNode* find_min(TreeNode*);
	void get_nodes(TreeNode*,std::vector<int>&) const;
	void delete_node(TreeNode*);
};