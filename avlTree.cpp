#include "student.cpp"

class Node {
public:
	Student* data;
	Node* left;
	Node* right;
	
	Node() {}
}

class AVL {
public:
	Node* root;
	AVL() {}
}