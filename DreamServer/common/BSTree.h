
//this is binary sort tree

#pragma once

#ifndef _BINARY_SORT_TREE_H__
#define _BINARY_SORT_TREE_H__

#include <DataStruct.h>

namespace BSTree
{
	template<typename ExtraType>
	struct BSTNode
	{
		DST::uint32_t lower_bound = 0;
		DST::uint32_t upper_bound = 0;
		DST::uint32_t count = 0;

		DST::ExtraArray<ExtraType> mExtraArray;

		struct BSTNode* l_child = nullptr;
		struct BSTNode* r_child = nullptr;
		struct BSTNode* prev = nullptr;
		struct BSTNode* next = nullptr;
	};

	template<typename ExtraType>
	struct BSTNodeData
	{
		DST::uint32_t ranking = 0;
		DST::uint32_t score = 0;
		DST::ExtraData<ExtraType> extra;
	};

	template<typename ExtraType>
	class BSTree
	{
	public:
		typedef BSTNodeData<ExtraType> BSTNodeDataType;
		typedef DST::ExtraData<ExtraType> ExtraDataType;
		typedef BSTNode<ExtraType> BSTreeNode;

		BSTree(DST::uint32_t lb, DST::uint32_t rb)
		{
			init(lb, rb);
		}

		~BSTree()
		{
			destroy();
		}

	public:
		void init(DST::uint32_t lb, DST::uint32_t rb)
		{
			root = setupNode(root, lb, rb);
		}

		void destroy()
		{
			eraseNode(root);
		}

		void insert(DST::uint32_t score, ExtraType l, ExtraType r)
		{
			ExtraDataType extra;
			extra.left = l;
			extra.right = r;

			insert(score, extra);
		}

		void insert(DST::uint32_t score, ExtraDataType extra)
		{
			insertNode(root, score, extra);
		}

		void remove(DST::uint32_t score, ExtraType l, ExtraType r)
		{
			ExtraDataType extra;
			extra.left = l;
			extra.right = r;

			remove(score, extra);
		}

		void remove(DST::uint32_t score, ExtraDataType extra)
		{
			removeNode(root, score, extra);
		}

		void change(DST::uint32_t oldScore, DST::uint32_t newScore, ExtraType l, ExtraType r)
		{
			ExtraDataType extra;
			extra.left = l;
			extra.right = r;

			change(oldScore, newScore, extra);
		}

		void change(DST::uint32_t oldScore, DST::uint32_t newScore, ExtraDataType extra)
		{
			remove(oldScore, extra);
			insert(newScore, extra);
		}

		DST::uint32_t getRank(DST::uint32_t score, ExtraType l, ExtraType r)
		{
			ExtraDataType extra;
			extra.left = l;
			extra.right = r;

			return getRank(score, extra) + 1;
		}

		DST::uint32_t getRank(DST::uint32_t score, ExtraDataType extra)
		{
			return getRankOfNode(root, score, extra);
		}

		bool getTopN(std::vector<BSTNodeDataType>& dataVec, const DST::uint32_t n)
		{
			DST::uint32_t count = 0;
			BSTreeNode* cursor = tail;
			dataVec.clear();
			while (nullptr != cursor)
			{
				for (ExtraDataType itExtra : cursor->mExtraArray.getDataList())
				{
					BSTNodeDataType data;
					data.ranking = ++count;
					data.score = cursor->lower_bound;
					data.extra = itExtra;
					dataVec.push_back(data);

					if (count >= n)
					{
						return true;
					}
				}

				cursor = cursor->prev;
			}

			return true;
		}
	private:
		BSTreeNode* setupNode(BSTreeNode* node, DST::uint32_t lb, DST::uint32_t rb)
		{
			if (lb > rb)
			{
				return nullptr;
			}

			node = new BSTreeNode();
			node->lower_bound = lb;
			node->upper_bound = rb;

			if (isLeafNode(node))
			{
				if (nullptr == head)
				{
					head = node;
				}

				if (nullptr != tail)
				{
					tail->next = node;
					node->prev = tail;
				}

				tail = node;
				return node;
			}

			if (rb > lb)
			{
				DST::uint32_t middle_bound = Utils::middle_of_two_numbers(lb, rb);
				node->l_child = setupNode(node->l_child, lb, middle_bound);
				node->r_child = setupNode(node->r_child, middle_bound + 1, rb);

			}

			return node;
		}

		void eraseNode(BSTreeNode* node)
		{
			if (nullptr == node)
			{
				return;
			}

			if (isLeafNode(node))
			{
				SAFE_DELETE(node);
				return;
			}

			if (nullptr == node->l_child && nullptr == node->r_child)
			{
				SAFE_DELETE(node);
				return;
			}

			eraseNode(node->l_child);
			eraseNode(node->r_child);
		}

		void insertNode(BSTreeNode* node, DST::uint32_t score, ExtraDataType extraData)
		{
			if (nullptr == node)
			{
				return;
			}

			if (!isInsideNode(node, score))
			{
				return;
			}

			++node->count;

			if (isLeafNode(node))
			{
				node->mExtraArray.insert(extraData);
				node->mExtraArray.sort();
				return;
			}

			DST::uint32_t middle_bound = Utils::middle_of_two_numbers(node->lower_bound, node->upper_bound);
			if (score <= middle_bound)
			{
				insertNode(node->l_child, score, extraData);
			}
			else
			{
				insertNode(node->r_child, score, extraData);
			}
		}

		void removeNode(BSTreeNode* node, DST::uint32_t score, ExtraDataType extraData)
		{
			if (nullptr == node)
			{
				return;
			}

			if (!isInsideNode(node, score))
			{
				return;
			}

			--node->count;

			if (isLeafNode(node))
			{
				node->mExtraArray.erase(extraData);
				node->mExtraArray.sort();
				return;
			}

			DST::uint32_t middle_bound = Utils::middle_of_two_numbers(node->lower_bound, node->upper_bound);
			if (score <= middle_bound)
			{
				removeNode(node->l_child, score, extraData);
			}
			else
			{
				removeNode(node->r_child, score, extraData);
			}

		}

		DST::uint32_t getRankOfNode(BSTreeNode* node, DST::uint32_t score, ExtraDataType extraData)
		{
			DST::uint32_t ranking = 0;

			if (nullptr == node)
			{
				return ranking;
			}

			if (score < node->lower_bound)
			{
				ranking += node->count;
				return ranking;
			}

			if (score > node->upper_bound)
			{
				ranking += 0;
				return ranking;
			}

			if (isLeafNode(node))
			{
				ranking += node->mExtraArray.find(extraData);
				return ranking;
			}

			DST::uint32_t middle_bound = Utils::middle_of_two_numbers(node->lower_bound, node->upper_bound);
			if (score <= middle_bound)
			{
				ranking += node->r_child != nullptr ? node->r_child->count : 0;
				ranking += getRankOfNode(node->l_child, score, extraData);
			}
			else
			{
				ranking += getRankOfNode(node->r_child, score, extraData);
			}

			return ranking;
		}



		bool isLeafNode(BSTreeNode* node)
		{
			return node->lower_bound == node->upper_bound;
		}

		bool isInsideNode(BSTreeNode* node, DST::uint32_t score)
		{
			return score >= node->lower_bound && score <= node->upper_bound;
		}


	private:
		BSTreeNode* root = nullptr;
		BSTreeNode* head = nullptr;
		BSTreeNode* tail = nullptr;
	};

}//// namespace BSTree







#endif