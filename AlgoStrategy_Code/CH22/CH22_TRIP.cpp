#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
#include <time.h>
using namespace std;

#if 0

class TripNode;
typedef int KeyType;

class TripNode
{
public :
	TripNode(const KeyType &_key) : key(_key), priority(rand()), size(1), left(NULL), right(NULL)
	{}

public :

	void setLeft(TripNode *node)
	{
		left = node;
		updateNodeSize();
	}

	void setRight(TripNode *node)
	{
		right = node;
		updateNodeSize();
	}

	void updateNodeSize()
	{
		size = 1;
		if (left) size += left->getSize();
		if (right) size += right->getSize();
	}

	int getSize() { return size; }

private :

// 변수 선언
public :
	KeyType key;
	int priority;
	int size;
	TripNode *left, *right;
};

typedef pair<TripNode*, TripNode*> NodePair;
static NodePair split(TripNode *root, KeyType key)
{
	if (root == NULL)
		return NodePair(NULL, NULL);
	if (root->key < key)
	{
		NodePair rightSplitNodes = split(root->right, key);
		root->setRight(rightSplitNodes.first);
		return NodePair(root, rightSplitNodes.second);
	}

	NodePair leftSplitNodes = split(root->left, key);
	root->setLeft(leftSplitNodes.second);
	return NodePair(leftSplitNodes.first, root);
}

static TripNode *insertTripNode(TripNode *root, TripNode *newNode)
{
	if (root == NULL) return newNode;

	if (root->priority > newNode->priority)
	{
		if (root->key > newNode->key)
			root->setLeft(insertTripNode(root->left, newNode));
		else
			root->setRight(insertTripNode(root->right, newNode));

		return root;
	}

	NodePair ns = split(root, newNode->key);
	newNode->setLeft(ns.first);
	newNode->setRight(ns.second);

	return newNode;
}

TripNode *findKthNode(TripNode *root, int k)
{
	if (root == NULL || k > root->size)
		return NULL;

	int leftSize = 0;
	if (root->left) leftSize = root->left->size;

	if (k == leftSize + 1)
		return root;

	if (k <= leftSize)
		return findKthNode(root->left, k);

	return findKthNode(root->right, k - leftSize - 1);
}

TripNode *mergeTripNode(TripNode *leftNode, TripNode *rightNode)
{
	if (leftNode == NULL)	return rightNode;
	if (rightNode == NULL)	return leftNode;

	if (leftNode->priority > rightNode->priority)
	{
		leftNode->setRight(mergeTripNode(leftNode->right, rightNode));
		return leftNode;
	}

	rightNode->setLeft(mergeTripNode(leftNode, rightNode->left));
	return rightNode;
}

TripNode *eraseTripNode(TripNode *root, KeyType key)
{
	if (root == NULL) return root;

	if (root->key == key)
	{
		TripNode *newRoot = mergeTripNode(root->left, root->right);
		delete root;
		return newRoot;
	}

	if (key < root->key)
		root->setLeft(eraseTripNode(root->left, key));
	else
		root->setRight(eraseTripNode(root->right, key));

	return root;
}

int countLessThanKey(TripNode *root, KeyType key)
{
	if (root == NULL)
		return 0;

	int leftSize = 0;
	int rightSize = 0;

	if (root->left) leftSize = root->left->size;
	if (root->right) leftSize = root->right->size;

	if (root->key > key)
		return countLessThanKey(root->left, key);

	return leftSize + 1 + countLessThanKey(root->right, key);
}

void removeTripTree(TripNode *root)
{
	if (root == NULL) return;

	removeTripTree(root->left);
	removeTripTree(root->right);
	delete root;
}

#endif