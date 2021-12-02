#include "BVHTree.h"

BVHTreeNode *insertToLeaf(BVHTreeNode *&node, AABB objectArea, std::string name);
BVHTreeNode *&findLeaf(BVHTreeNode *&node, AABB objectArea);
void correctParents(BVHTreeNode *node);
void getCollidingObjectsRec(AABB object, BVHTreeNode * node, std::vector<std::string> & list);

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level)
{
	if (root == nullptr)
		return;
	for (int i = 0; i < level; i++)
	{
		out << "  ";
	}
	if (!node->isLeaf)
	{
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else
	{
		out << "- ";
		if (node->parent)
		{
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}
std::ostream &operator<<(std::ostream &out, BVHTree &tree)
{
	tree.printNode(out, tree.root, 0);
	return out;
}

BVHTree::BVHTree()
{
	root = NULL;
}

BVHTree::~BVHTree()
{
}

void BVHTree::addBVHMember(AABB objectArea, std::string name)
{
	if (root == NULL) // if tree is empty
	{
		root = new BVHTreeNode(objectArea, name, true);
		map[name] = root;
	}
	else if (root && root->leftChild == nullptr && root->rightChild == nullptr) // if only one node in tree (might be buggy)
	{
		BVHTreeNode *new_node = insertToLeaf(root, objectArea, name);
		map[name] = new_node; // can there be collision?
	}
	else // there is more than one node in tree
	{
		BVHTreeNode *new_node = insertToLeaf(findLeaf(root, objectArea), objectArea, name);
		map[name] = new_node;

		correctParents(new_node);
	}
}

void BVHTree::moveBVHMember(std::string name, AABB newLocation)
{
	BVHTreeNode *node = map[name];
	BVHTreeNode* sibling = node->parent->leftChild == node ? node->parent->rightChild : node->parent->leftChild;

	AABB parent_area = node->parent->aabb;
	AABB new_area = sibling->aabb + newLocation;
	if (parent_area.maxX >= new_area.maxX && parent_area.minX <= new_area.minX
			&& parent_area.maxY >= new_area.maxY && parent_area.minY <= new_area.minY
		)
	{
		node->aabb = newLocation;
	}
	else
	{
		removeBVHMember(name);
		BVHTreeNode* new_node = insertToLeaf(findLeaf(root, newLocation), newLocation, name);
		correctParents(new_node);
	}
}

void BVHTree::removeBVHMember(std::string name)
{
	BVHTreeNode *nodeToBeDeleted = map[name];
	BVHTreeNode *parent = nodeToBeDeleted->parent;
	BVHTreeNode *sibling = parent->leftChild == nodeToBeDeleted ? parent->rightChild : parent->leftChild;
	BVHTreeNode *grandParent = parent->parent;

	if (grandParent->leftChild == parent)
	{
		grandParent->leftChild = sibling;
	}
	else
	{
		grandParent->rightChild = sibling;
	}

	sibling->parent = grandParent;

	map.erase(name);
	delete nodeToBeDeleted;
	delete parent;
}

std::vector<std::string> BVHTree::getCollidingObjects(AABB object)
{
	std::vector<std::string> list;
	getCollidingObjectsRec(object, root, list);

	return list;
}

void getCollidingObjectsRec(AABB object, BVHTreeNode * node, std::vector<std::string> & list)
{
	if (node)
	{
		if (node->isLeaf)
		{
			if (node->aabb.collide(object))
				list.push_back(node->name);
		}
		else
		{
			getCollidingObjectsRec(object,  node->leftChild, list);
			getCollidingObjectsRec(object,  node->rightChild, list);
		}
	}
}

BVHTreeNode *insertToLeaf(BVHTreeNode *&node, AABB objectArea, std::string name)
{
	BVHTreeNode *parent = node->parent;
	BVHTreeNode *old_node = node;
	BVHTreeNode *new_node = new BVHTreeNode(objectArea, name, true);

	node = new BVHTreeNode(old_node->aabb + new_node->aabb, "branch", false);
	node->parent = parent;

	node->leftChild = new_node;
	node->leftChild->parent = node;

	node->rightChild = old_node;
	node->rightChild->parent = node;

	return new_node;
}

BVHTreeNode *&findLeaf(BVHTreeNode *&node, AABB objectArea)
{
	if (node->isLeaf)
		return node;

	int increaseInRightTreeSize = AABB::unionArea(objectArea, node->rightChild->aabb) - node->rightChild->aabb.getArea();
	int increaseInLeftTreeSize = AABB::unionArea(objectArea, node->leftChild->aabb) - node->leftChild->aabb.getArea();

	if (increaseInLeftTreeSize <= increaseInRightTreeSize)
	{
		return findLeaf(node->leftChild, objectArea);
	}
	else
	{
		return findLeaf(node->rightChild, objectArea);
	}
}

void correctParents(BVHTreeNode *node)
{
	BVHTreeNode *ptr = node;

	while (ptr != NULL)
	{
		if (!ptr->isLeaf)
		{
			ptr->aabb = ptr->leftChild->aabb + ptr->rightChild->aabb;
		}
		ptr = ptr->parent;
	}
}