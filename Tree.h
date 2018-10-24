#pragma once
#include <list>
#include <iostream> // For debugging
#include <vector>

template <typename T>
class Tree
{
	struct TreeNode
	{
		T mData;
		TreeNode *mLeft;
		TreeNode *mRight;
		int mHeight;

		TreeNode()
		{
			mLeft = nullptr;
			mRight = nullptr;
			mHeight = 0;
		}
		TreeNode(T tData) : TreeNode()
		{
			mData = tData;
			mLeft = nullptr;
			mRight = nullptr;
			mHeight = 0;
		}
	};
	TreeNode *mHead;

	void CopyTree(TreeNode *tNode)  // Aren't these recursive functions beautiful?  A perfect example of the three strategies.  Copy, Delete, and Dump
	{
		if (tNode != nullptr)
		{
			Add(tNode->mData);
			CopyTree(tNode->mLeft);
			CopyTree(tNode->mRight);
		}

	}
	void DeleteTree(TreeNode *tNode)
	{
		if (tNode != nullptr)
		{
			DeleteTree(tNode->mLeft);
			DeleteTree(tNode->mRight);
			delete tNode;
		}
	}
	void DumpTree(std::list<T> *tLeftToRight, TreeNode *tNode)
	{
		if (tNode != nullptr)
		{
			if (tNode->mLeft)
			{
				std::cout << "L "; // Going left
				DumpTree(tLeftToRight, tNode->mLeft);
			}

			// To help debug avl balancing, I'm going to make this thing output.  This is generally a naughty thing to do outside of main because it 
			// makes a dependency between your template code and the console.
			std::cout << tNode->mData << " ";
			//			tLeftToRight->push_back(tNode->mData);

			if (tNode->mRight)
			{
				std::cout << "R "; // Going right
				DumpTree(tLeftToRight, tNode->mRight);
			}
			std::cout << "^ "; // Going back up
		}
	}
	void DeleteFunc(TreeNode *tNodeToDelete, TreeNode *tWalker)	//Called in Erase(), just so I don't have to type it twice for mLeft and mRight
	{
		// Get the number of children of the node we're deleting (bool is same as 1 or 0 so I can just add them)
		int tDeleteChildCount = (tNodeToDelete->mLeft != nullptr) + (tNodeToDelete->mRight != nullptr);
		switch (tDeleteChildCount)
		{
		case 0:
			if (tWalker->mLeft == tNodeToDelete)
				tWalker->mLeft = nullptr;
			else
				tWalker->mRight = nullptr;
			delete tNodeToDelete;
			break;
		case 1:
			if (tWalker->mLeft == tNodeToDelete)
			{
				// Does the left node exist?					   set it                no? Then it must be the right.
				tWalker->mLeft = (tNodeToDelete->mLeft != nullptr) ? tNodeToDelete->mLeft : tNodeToDelete->mRight;
			}
			else
				tWalker->mRight = (tNodeToDelete->mLeft != nullptr) ? tNodeToDelete->mLeft : tNodeToDelete->mRight;

			delete tNodeToDelete;
			break;
		case 2:
			TreeNode *tLeftMostNode = tNodeToDelete->mRight;
			TreeNode *tLeftMostPrevious = tLeftMostNode;
			while (tLeftMostNode->mLeft != nullptr)
			{
				tLeftMostPrevious = tLeftMostNode;
				tLeftMostNode = tLeftMostNode->mLeft;
			}
			tNodeToDelete->mData = tLeftMostNode->mData;
			int tLeftMostChildCount = (tLeftMostNode->mLeft != nullptr) + (tLeftMostNode->mRight != nullptr);
			if (tLeftMostChildCount == 0)
			{
				delete tLeftMostNode;
				tLeftMostPrevious->mLeft = nullptr;
			}
			else
			{
				// Does the left node exist?									set it                no? Then it must be the right.
				tLeftMostPrevious->mLeft = (tLeftMostNode->mLeft != nullptr) ? tLeftMostNode->mLeft : tLeftMostNode->mRight;
				delete tLeftMostNode;
			}
			break;
		}
	}

public:
	Tree()
	{
		mHead = nullptr;
	}
	Tree(const Tree &other) : Tree()
	{
		CopyTree(other.mHead);
	}
	Tree & operator= (const Tree & other)
	{
		DeleteTree(mHead);
		mHead = nullptr;

		if (this != &other)
			CopyTree(other.mHead);

		return *this;
	}
	~Tree()
	{
		DeleteTree(mHead);
	}

	void Add(T tWhat)
	{
		if (mHead == nullptr)
		{
			mHead = new TreeNode(tWhat);
			return;
		}

		std::vector<TreeNode> *tPathNodes = new std::vector<TreeNode>;
		TreeNode *tWalker = mHead;
		while (true)
		{
			if (tWalker->mData == tWhat) // This data is already here. No dupes.
			{
				break;
			}
			else if (tWalker->mData > tWhat) // I want to go left
			{
				if (tWalker->mLeft == nullptr) // But I'm at the end so I'll just put this there.
				{
					tWalker->mLeft = new TreeNode(tWhat);
					break;
				}
				else
				{
					tWalker = tWalker->mLeft;
				}
				tPathNodes->push_back(*tWalker);
			}
			else
			{
				if (tWalker->mRight == nullptr)
				{
					tWalker->mRight = new TreeNode(tWhat);
					break;
				}
				else
				{
					tWalker = tWalker->mRight;
				}
				tPathNodes->push_back(*tWalker);
			}
		}
		for (TreeNode *tNode = tPathNodes->begin(); tNode < tPathNodes->end(); tNode = tPathNodes->)
		{
			tNode->mHeight++;
		}
		delete tPathNodes;

	}

	void Remove(T tWhat)
	{
		if (mHead == nullptr)
			return;
		if (mHead->mData == tWhat)
		{
			// mHead.mIsDead = true;
		}

		TreeNode *tWalker = mHead;
		while (true)
		{
			if (tWalker->mData > tWhat)// I want to go left
			{
				if (tWalker->mLeft == nullptr)// But I can't
					break;
				else if (tWalker->mLeft->mData == tWhat)// ...or I don't need to because it's right there
				{
					TreeNode *tNodeToDelete = tWalker->mLeft;
					DeleteFunc(tNodeToDelete, tWalker);
					break;
				}
				else
					tWalker = tWalker->mLeft;// I'll go left and keep looking
			}
			else
			{
				if (tWalker->mRight == nullptr)
					break;
				else if (tWalker->mRight->mData == tWhat)
				{
					TreeNode *tNodeToDelete = tWalker->mRight;
					DeleteFunc(tNodeToDelete, tWalker);
					break;
				}
				else
					tWalker = tWalker->mRight;
			}
		}
	}


	bool Contains(T tWhat)// Nobody likes copied code.  Is there a way to consolidate these?  Maybe Add and Contains both call a PrivateFind or something?
	{
		if (mHead == nullptr)
			return false;
		TreeNode *tWalker = mHead;
		while (true)
		{
			if (tWalker->mData == tWhat)
			{
				return true;// return tWalker->mData->mIsDead
			}
			else if (tWalker->mData > tWhat)
			{
				if (tWalker->mLeft == nullptr)
					return false;
				else
					tWalker = tWalker->mLeft;
			}
			else
			{
				if (tWalker->mRight == nullptr)
					return false;
				else
					tWalker = tWalker->mRight;
			}

		}
		return false;
	}
	void Dump(std::list<T> *tLeftToRight)
	{
		DumpTree(tLeftToRight, mHead);
	}
};