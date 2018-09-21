#include "BevTree.h"

using namespace DST;

namespace BevTree
{
	//------------------------------------------------------------------
	//BevNodePrioritySelector
	//------------------------------------------------------------------
	bool BevNodePrioritySelector::_doEvaluate(const BevNodeInputParam& input)
	{
		mCurSelectIndex = MAX_CHILD_NODE_COUNT;
		for (uint32_t i = 0; i < mChildNodeCount; ++i)
		{
			BevNode* node = mChildNodeList[i];
			if (node->evaluate(input))
			{
				mCurSelectIndex = i;
				return true;
			}
		}

		return false;
	}

	void BevNodePrioritySelector::_doTransition(const BevNodeInputParam& input)
	{
		if (checkIndex(mLastSelectIndex))
		{
			BevNode* node = mChildNodeList[mLastSelectIndex];
			node->transition(input);
		}

		mLastSelectIndex = MAX_CHILD_NODE_COUNT;
	}

	BevRunningStatus BevNodePrioritySelector::_doTick(const BevNodeInputParam& input, BevNodeOutputParam& output)
	{
		BevRunningStatus isFinish = BRS_Finish;
		if (checkIndex(mCurSelectIndex))
		{
			if (mLastSelectIndex != mCurSelectIndex)
			{
				if (checkIndex(mLastSelectIndex))
				{
					BevNode* node = mChildNodeList[mLastSelectIndex];
					node->transition(input);
				}
				
				mLastSelectIndex = mCurSelectIndex;
			}
		}

		if (checkIndex(mLastSelectIndex))
		{
			BevNode* node = mChildNodeList[mLastSelectIndex];
			isFinish = node->tick(input, output);
			if (isFinish)
			{
				mLastSelectIndex = MAX_CHILD_NODE_COUNT;
			}
		}

		return isFinish;
	}

	//------------------------------------------------------------------
	//BevNodeNonePrioritySelector
	//------------------------------------------------------------------
	bool BevNodeNonePrioritySelector::_doEvaluate(const BevNodeInputParam& input)
	{
		if (checkIndex(mCurSelectIndex))
		{
			BevNode* node = mChildNodeList[mCurSelectIndex];
			if (node->evaluate(input))
			{
				return true;
			}
		}

		return BevNodeNonePrioritySelector::_doEvaluate(input);
	}

	//------------------------------------------------------------------
	//BevNodeSequence
	//------------------------------------------------------------------
	bool BevNodeSequence::_doEvaluate(const BevNodeInputParam& input)
	{
		uint32_t testNode;
		testNode = (mCurNodeIndex == MAX_CHILD_NODE_COUNT) ? 0 : mCurNodeIndex;
		if (checkIndex(testNode))
		{
			BevNode* node = mChildNodeList[testNode];
			node->evaluate(input);
			return true;
		}

		return false;
	}

	void BevNodeSequence::_doTransition(const BevNodeInputParam& input)
	{
		if (checkIndex(mCurNodeIndex))
		{
			BevNode* node = mChildNodeList[mCurNodeIndex];
			node->transition(input);
		}

		mCurNodeIndex = INVALID_CHILD_NODE_INDEX;
	}

	BevRunningStatus BevNodeSequence::_doTick(const BevNodeInputParam& input, BevNodeOutputParam& output)
	{
		BevRunningStatus isFinish = BRS_Finish;
		if (mCurNodeIndex == INVALID_CHILD_NODE_INDEX)
		{
			mCurNodeIndex = 0;
		}

		BevNode* node = mChildNodeList[mCurNodeIndex];
		isFinish = node->tick(input, output);
		if (BRS_Finish == isFinish)
		{
			++mCurNodeIndex;
			if (mChildNodeCount == mCurNodeIndex)
			{
				mCurNodeIndex = INVALID_CHILD_NODE_INDEX;
			}
			else
			{
				isFinish = BRS_Executing;
			}
		}

		if (isFinish < 0)
		{
			mCurNodeIndex = INVALID_CHILD_NODE_INDEX;
		}

		return isFinish;
	}

	//------------------------------------------------------------------
	//BevNodeTerminal
	//------------------------------------------------------------------
	void BevNodeTerminal::_doTransition(const BevNodeInputParam& input)
	{
		if (mIsNeedExit)
		{
			_doExit(input, BRS_ERROR_Transition);
		}
		setActiveNode(NULL);
		mStatus = TNS_Ready;
		mIsNeedExit = false;
	}

	BevRunningStatus BevNodeTerminal::_doTick(const BevNodeInputParam& input, BevNodeOutputParam& output)
	{
		BevRunningStatus isFinish = BRS_Finish;

		if (TNS_Ready == mStatus)
		{
			_doEnter(input);
			setActiveNode(this);
			mStatus = TNS_Running;
			mIsNeedExit = true;
		}

		if (TNS_Running == mStatus)
		{
			isFinish = _doExecute(input, output);
			setActiveNode(this);
			if (isFinish == BRS_Finish || isFinish < 0)
			{
				mStatus = TNS_Finish;
			}
		}

		if (TNS_Finish == mStatus)
		{
			if (mIsNeedExit)
			{
				_doExit(input, isFinish);
			}

			setActiveNode(NULL);
			mStatus = TNS_Ready;
			mIsNeedExit = false;
		}

		return isFinish;
	}

	//------------------------------------------------------------------
	//BevNodeParallel
	//------------------------------------------------------------------
	bool BevNodeParallel::_doEvaluate(const BevNodeInputParam& input)
	{
		for (uint32_t i = 0; i < mChildNodeCount;++i)
		{
			BevNode* node = mChildNodeList[i];
			if (BRS_Executing == mChildNodeStatus[i])
			{
				if (!node->evaluate(input))
				{
					return false;
				}
			}
		}

		return true;
	}

	void BevNodeParallel::_doTransition(const BevNodeInputParam& input)
	{
		for (uint32_t i = 0; i < MAX_CHILD_NODE_COUNT; ++i)
		{
			mChildNodeStatus[i] = BRS_Executing;
		}

		for (uint32_t i = 0; i < mChildNodeCount; ++i)
		{
			BevNode* node = mChildNodeList[i];
			node->transition(input);
		}
	}

	BevNodeParallel& BevNodeParallel::setFinishCondition(ParallelFinishCondition condition)
	{
		mFinishCondition = condition;
		return *this;
	}

	BevRunningStatus BevNodeParallel::_doTick(const BevNodeInputParam& input, BevNodeOutputParam& output)
	{
		uint32_t nFinishChildCount = 0;
		for (uint32_t i = 0; i < mChildNodeCount; ++i)
		{
			BevNode* node = mChildNodeList[i];
			if (PFC_OR == mFinishCondition)
			{
				if (BRS_Executing == mChildNodeStatus[i])
				{
					mChildNodeStatus[i] = node->tick(input, output);
				}
				if (BRS_Executing != mChildNodeStatus[i])
				{
					for (uint32_t j = 0; j < MAX_CHILD_NODE_COUNT; ++j)
					{
						mChildNodeStatus[j] = BRS_Executing;
					}

					return BRS_Finish;
				}
			}
			else if (PFC_AND == mFinishCondition)
			{
				if (BRS_Executing == mChildNodeStatus[i])
				{
					mChildNodeStatus[i] = node->tick(input, output);
				}

				if (BRS_Executing != mChildNodeStatus[i])
				{
					++nFinishChildCount;
				}
			}
		}

		if (mChildNodeCount == nFinishChildCount)
		{
			for (uint32_t i = 0; i < mChildNodeCount; ++i)
			{
				mChildNodeStatus[i] = BRS_Executing;
			}
			return BRS_Finish;
		}

		return BRS_Executing;
	}

	//------------------------------------------------------------------
	//BevNodeLoop
	//------------------------------------------------------------------
	bool BevNodeLoop::_doEvaluate(const BevNodeInputParam& input)
	{
		bool isCheckLoopCount = (mLoopCount == InfiniteLoop) || mCurCount < mLoopCount;
		if (!isCheckLoopCount)
		{
			return false;
		}

		if (checkIndex(0))
		{
			BevNode* node = mChildNodeList[0];
			if (node->evaluate(input))
			{
				return true;
			}
		}

		return false;
	}

	void BevNodeLoop::_doTransition(const BevNodeInputParam& input)
	{
		if (checkIndex(0))
		{
			BevNode* node = mChildNodeList[0];
			node->transition(input);
		}

		mCurCount = 0;
	}

	BevRunningStatus BevNodeLoop::_doTick(const BevNodeInputParam& input, BevNodeOutputParam& output)
	{
		BevRunningStatus isFinish = BRS_Finish;
		if (checkIndex(0))
		{
			BevNode* node = mChildNodeList[0];
			isFinish = node->tick(input, output);
			if (BRS_Finish == isFinish)
			{
				if (InfiniteLoop != mLoopCount)
				{
					++mCurCount;
					if (mCurCount < mLoopCount)
					{
						isFinish = BRS_Executing;
					}
				}
				else
				{
					isFinish = BRS_Executing;
				}
			}
		}

		if (isFinish)
		{
			mCurCount = 0;
		}

		return isFinish;
	}


}// namespace BevTree