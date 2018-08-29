#include <iostream>
#include <stack>
#include <ctime>
#include <algorithm> //max function

using namespace std;

template <class T>
class BST;

const int IMBALANCE = 1;

//BST Node Class (templated)
template <class T>
class BSTNode {
public:
	BSTNode(T newdata = T(), BSTNode<T>* newparent = nullptr, BSTNode<T>* newleft = nullptr, BSTNode<T>* newright = nullptr, int newheight = 0) {
		data = newdata; parent = newparent; left = newleft; right = newright; height = newheight;
	};

	friend class BST<T>;

private:
	BSTNode<T>* left;
	BSTNode<T>* right;
	BSTNode<T>* parent;
	T data;
	int height;
};


template <class T>
class BST {
private:
	BSTNode<T>* root;
	int countOfNodes;
	BSTNode<T>* recursiveCopy(const BSTNode<T>* rhs);
	void printInOrder(BSTNode<T>* ptr);
public:
	BST() : root(nullptr) {}; //default constructor (empty BST)
	
	BST(const BST<T>& rhs) : root(nullptr) { *this = rhs; }; //copy constructor
	~BST() { clear(); }; //destructor
	bool isEmpty() { return root == nullptr; };
	void remove(const T& val) { remove(find(val)); };
	bool findInTree(const T& val) { return find(val) != nullptr; }
	void clear() { while (!isEmpty()) remove(root); }
	int size() { return countOfNodes; }


	BST<T>& operator=(const BST<T>& rhs); //=operator overload
	void insert(const T& val);
	void remove(BSTNode<T>* ptr);
	BSTNode<T>* find(const T&);
	void printInOrder() { printInOrder(root); }
	
	int getHeight(BSTNode<T>* ptr);
	void balance(BSTNode<T>* ptr);
	void rotateWithLeftChild(BSTNode<T>* ptr);
	void doubleWithLeftChild(BSTNode<T>* ptr);
	void rotateWithRightChild(BSTNode<T>* ptr);
	void doubleWithRightChild(BSTNode<T>* ptr);

	void printLevelOrder();
	void printThisLevel(BSTNode<T>* ptr, int level);
};

template <class T>
void BST<T>::printLevelOrder() {
	int x = getHeight(root);
	for (int i = 1; i <= x+1; i++) {
		printThisLevel(root, i);
		cout << endl;
	}

}


template <class T>
void BST<T>::printThisLevel(BSTNode<T>* ptr, int level) {
	if (ptr == nullptr)
		cout << "x "; //can choose to print "x " or just space if nullptr
	else if (level == 1)
		cout << ptr->data << " ";
	else if (level > 1) { //recursive call to left and right children nodes
		printThisLevel(ptr->left, level - 1);
		printThisLevel(ptr->right, level - 1);
	}
}

template <class T>
void BST<T>::balance(BSTNode<T>* ptr) { //start ptr and go up BST to update heights and balance each node
	
	BSTNode<T>* temp = ptr;
	while (temp != nullptr) {

		if (getHeight(temp->left) - getHeight(temp->right) > IMBALANCE) { // left subtree is heavier
			if (getHeight(temp->left->left) >= getHeight(temp->left->right)) { //left-left case
				rotateWithLeftChild(temp);
			}
			else { //left-right case
				doubleWithLeftChild(temp);
			}
		}
		else if (getHeight(temp->right) - getHeight(temp->left) > IMBALANCE) { //right subtree is heavier
			if (getHeight(temp->right->right) >= getHeight(temp->right->left)) { //right-right case
				rotateWithRightChild(temp);
			}
			else { //right-left case
				doubleWithRightChild(temp);
			}
		}
		else {
			temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1; //update height of temp node
		}
		temp = temp->parent;
	} //after while loop terminates temp is nullptr
	
}

template <class T>
void BST<T>::rotateWithLeftChild(BSTNode<T>* ptr) {
	BSTNode<T>* ptr2 = ptr->left;

	ptr->left = ptr2->right;
	if (ptr->left != nullptr){
		ptr->left->parent = ptr;
	}

	ptr2->right = ptr;
	
	ptr2->parent = ptr->parent;
	if (ptr != root) {
		if (ptr->parent->left == ptr)
			ptr->parent->left = ptr2;
		else
			ptr->parent->right = ptr2;
	} else {
		root = ptr2;
	}
	ptr->parent = ptr2;

	ptr->height = max(getHeight(ptr->left), getHeight(ptr->right)) + 1;
	ptr2->height = max(getHeight(ptr2->left), getHeight(ptr2->right)) + 1;
	//ptr = ptr2; //update input ptr to point to promoted node
}

template <class T>
void BST<T>::doubleWithLeftChild(BSTNode<T>* ptr) {
	rotateWithRightChild(ptr->left);
	rotateWithLeftChild(ptr);
}

template <class T>
void BST<T>::rotateWithRightChild(BSTNode<T>* ptr) {
	BSTNode<T>* ptr2 = ptr->right;

	ptr->right = ptr2->left;
	if (ptr->right != nullptr) {
		ptr->right->parent = ptr;
	}

	ptr2->left = ptr;

	ptr2->parent = ptr->parent;
	if (ptr != root) {
		if (ptr->parent->right == ptr)
			ptr->parent->right = ptr2;
		else
			ptr->parent->left = ptr2;
	} else {
		root = ptr2;
	}
	ptr->parent = ptr2;

	ptr->height = max(getHeight(ptr->left), getHeight(ptr->right)) + 1;
	ptr2->height = max(getHeight(ptr2->left), getHeight(ptr2->right)) + 1;
	//ptr = ptr2; //update input ptr to point to promoted node to continue in balance() function
}

template <class T>
void BST<T>::doubleWithRightChild(BSTNode<T>* ptr) {
	rotateWithLeftChild(ptr->right);
	rotateWithRightChild(ptr);
}

template <class T>
int BST<T>::getHeight(BSTNode<T>* ptr) {
	if (ptr == nullptr)
		return -1;
	else
		return max(getHeight(ptr->right), getHeight(ptr->left))+1;
}

template <class T>
void BST<T>::printInOrder(BSTNode<T>* ptr) {
	if (root != nullptr) {
		if (ptr->left != nullptr)
			printInOrder(ptr->left);
		cout << ptr->data << endl;
		if (ptr->right != nullptr)
			printInOrder(ptr->right);
	}
}


template <class T>
BSTNode<T>* BST<T>::find(const T& val) {
	BSTNode<T>* temp = root;
	while (temp != nullptr && temp->data != val) {
		if (val < temp->data)
			temp = temp->left;
		else
			temp = temp->right;
	}
	return temp; //either return null or ptr to node where val exists

}


template <class T>
void BST<T>::remove(BSTNode<T>* ptr) {
	if (ptr == nullptr) //asking to remove something that doesnt exist (find() can return nullptr)
		return;
	if (countOfNodes == 1 && ptr == nullptr) { //ptr is last node on tree
		countOfNodes--;
		delete root;
		root = nullptr;
	}

	//ptr has no children (just update parent's child pointer to null and delete node)
	else if (ptr->left == nullptr && ptr->right == nullptr) {
		BSTNode<T>* parent = ptr->parent;
		if (parent != nullptr) {
			if (parent->left == ptr)
				parent->left = nullptr;
			else
				parent->right = nullptr;
		}
		delete ptr;
		ptr = nullptr;
		countOfNodes--;
		balance(parent);
	} 


	//ptr has one child (delete node and promote child)
	else if (ptr->left == nullptr) {//node has right child only
		BSTNode<T>* temp = ptr->right;
		BSTNode<T>* parent = ptr->parent;
		if (parent != nullptr) {
			if (ptr == parent->left)
				parent->left = temp;
			else
				parent->right = temp;
		}
		else
			root = temp;
		temp->parent = parent;
		delete ptr;
		ptr = nullptr;
		countOfNodes--;
		balance(temp);
	}

	else if (ptr->right == nullptr) {//node has left child only
		BSTNode<T>* temp = ptr->left;
		BSTNode<T>* parent = ptr->parent;
		if (parent != nullptr) {
			if (ptr == parent->right)
				parent->right = temp;
			else
				parent->left = temp;
		}
		else
			root = temp;
		temp->parent = parent;
		delete ptr;
		ptr = nullptr;
		countOfNodes--;
		balance(temp);
	}

	//ptr has two children, not clear which is to promote, BUT we have a workaround
	else {
		BSTNode<T>* temp = ptr->right;
		while (temp->left != nullptr)
			temp = temp->left;
		ptr->data = temp->data;
		remove(temp); //recursion to remove further child that is going to get promoted to ptr
		//this last recursion guarantees termination because temp CANNOT have two children
	}
}

template <class T>
void BST<T>::insert(const T& val) { //note that insertion for BST can only occur at one place
	countOfNodes++;
	if (root == nullptr) {
		root = new BSTNode<T>(val);
		root->height = 0;
		return;
	} else {
		BSTNode<T>* prev = root;
		BSTNode<T>* temp = root;
		while (temp != nullptr) {
			prev = temp;
			if (val < temp->data)
				temp = temp->left;
			else
				temp = temp->right;
		} //at end of this while loop, temp = nullptr, prev points to to-be-parent of new node
		//prev is a pointer to the node on which we will insert

		if (val < prev->data) {
			prev->left = new BSTNode<T>(val, prev);
			prev->left->height = 0;
			balance(prev->left); //start from new node and balance tree as you go up the tree to root
		} else {
			prev->right = new BSTNode<T>(val, prev);
			prev->right->height = 0;
			balance(prev->right);
		}
	}
}

template <class T>
BSTNode<T>* BST<T>::recursiveCopy(const BSTNode<T>* rhs) {
	if (rhs == nullptr)
		return nullptr;
	BSTNode<T>* temp = new BSTNode<T>(rhs->data);
	temp->left = recursiveCopy(rhs->left);
	temp->right = recursiveCopy(rhs->right);
	if (temp->left != nullptr)
		temp->left->parent = temp;
	if (temp->right != nullptr)
		temp->right->parent = temp;
	
	return temp;

}

template <class T>
BST<T>& BST<T>::operator=(const BST<T>& rhs) {
	if (this == &rhs)
		return *this;
	clear(); //clear out 'this' tree before copying data from rhs
	root = recursiveCopy(rhs.root);
	countOfNodes = rhs.countOfNodes;
	return *this;

}

int main() {

	BST<int> t1;
	srand(/*time(0)*/15);
	/*
	for (int i = 0; i < 7; i++) {
		t1.insert(rand() % 1000);
	}
	*/

	//example from textbook to check AVL program
	t1.insert(3);
	t1.insert(2);
	t1.insert(1);
	t1.insert(4);
	t1.insert(5);
	t1.insert(6);
	t1.insert(7);
	t1.insert(16);
	t1.insert(15);
	t1.insert(14);
	t1.insert(13);
	t1.insert(12);
	t1.insert(11);
	t1.insert(10);
	t1.insert(8);
	t1.insert(9);


	t1.printLevelOrder();

	t1.remove(9);
	t1.remove(8);
	t1.remove(10);
	t1.remove(11);
	t1.remove(12);
	t1.remove(13);
	t1.remove(14);
	t1.remove(15);

	t1.printLevelOrder();

	cout << "Done printing AVL Tree in level order" << endl;

	/*
	BST<int>t2 = t1;
	t2.printLevelOrder();

	for (int i = 0; i < 10; i++)
		t2.remove(rand() % 1000);
	*/

	return 0;
}
