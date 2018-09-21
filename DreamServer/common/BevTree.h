
#pragma once

#ifndef _BEV_TREE_H__
#define _BEV_TREE_H__

#include <Base.h>

namespace BevTree
{
#define MAX_CHILD_NODE_COUNT 16
#define INVALID_CHILD_NODE_INDEX MAX_CHILD_NODE_COUNT

	enum BevRunningStatus
	{
		BRS_Executing			= 0,
		BRS_Finish				= 1,
		BRS_ERROR_Transition	= -1
	};

	enum TerminalNodeStatus
	{
		TNS_Ready	= 1,
		TNS_Running	= 2,
		TNS_Finish	= 3
	};

	enum ParallelFinishCondition
	{
		PFC_OR	= 1,
		PFC_AND	= 2
	};



	typedef DST::AnyData BevNodeInputParam;
	typedef DST::AnyData BevNodeOutputParam;

	class BevNodePrecondition : public Object
	{
	public:
		virtual bool externalCondition(const BevNodeInputParam& input) const = 0;
	};

	class BevNodePreconditonTRUE : public BevNodePrecondition
	{
	public:
		virtual bool externalCondition(const BevNodeInputParam& input) const override
		{
			return true;
		}
	};

	class BevNodePreconditionFALSE : public BevNodePrecondition
	{
	public:
		virtual bool externalCondition(const BevNodeInputParam& input) const override
		{
			return false;
		}
	};

	class BevNodePreconditionNOT : public BevNodePrecondition
	{
	public:
		BevNodePreconditionNOT(BevNodePrecondition* lhs)
			:m_lhs(lhs)
		{}

		~BevNodePreconditionNOT()
		{
			SAFE_DELETE(m_lhs);
		}

		virtual bool externalCondition(const BevNodeInputParam& input) const override
		{
			return !m_lhs->externalCondition(input);
		}
	private:
		BevNodePrecondition* m_lhs;
	};

	class BevNodePreconditionAND : public BevNodePrecondition
	{
	public:
		BevNodePreconditionAND(BevNodePrecondition* lhs, BevNodePrecondition* rhs)
			:m_lhs(lhs)
			, m_rhs(rhs)
		{}

		~BevNodePreconditionAND()
		{
			SAFE_DELETE(m_lhs);
			SAFE_DELETE(m_rhs);
		}

		virtual bool externalCondition(const BevNodeInputParam& input) const override
		{
			return m_lhs->externalCondition(input) && m_rhs->externalCondition(input);
		}
	private:
		BevNodePrecondition* m_lhs;
		BevNodePrecondition* m_rhs;
	};

	class BevNodePreconditionOR : public BevNodePrecondition
	{
	public:
		BevNodePreconditionOR(BevNodePrecondition* lhs, BevNodePrecondition* rhs)
			:m_lhs(lhs)
			, m_rhs(rhs)
		{}

		~BevNodePreconditionOR()
		{
			SAFE_DELETE(m_lhs);
			SAFE_DELETE(m_rhs);
		}

		virtual bool externalCondition(const BevNodeInputParam& input) const override
		{
			return m_lhs->externalCondition(input) || m_rhs->externalCondition(input);
		}
	private:
		BevNodePrecondition* m_lhs;
		BevNodePrecondition* m_rhs;
	};

	class BevNodePreconditionXOR : public BevNodePrecondition
	{
	public:
		BevNodePreconditionXOR(BevNodePrecondition* lhs, BevNodePrecondition* rhs)
			:m_lhs(lhs)
			, m_rhs(rhs)
		{}

		~BevNodePreconditionXOR()
		{
			SAFE_DELETE(m_lhs);
			SAFE_DELETE(m_rhs);
		}

		virtual bool externalCondition(const BevNodeInputParam& input) const override
		{
			return m_lhs->externalCondition(input) ^ m_rhs->externalCondition(input);
		}
	private:
		BevNodePrecondition* m_lhs;
		BevNodePrecondition* m_rhs;
	};

	//multi: over two preconditons 
	class BevNodeMultiPreconditionAND : public BevNodePrecondition
	{
	public:
		BevNodeMultiPreconditionAND(std::vector<BevNodePrecondition*> &conVec)
			:mPreConVec(std::move(conVec))
		{}

		~BevNodeMultiPreconditionAND()
		{
			for (auto c:mPreConVec)
			{
				SAFE_DELETE(c);
			}
		}

		virtual bool externalCondition(const BevNodeInputParam& input) const override
		{
			for (auto c:mPreConVec)
			{
				if (!c->externalCondition(input))
				{
					return false;
				}
			}

			return true;
		}
	private:
		std::vector<BevNodePrecondition*> mPreConVec;
	};

	class BevNodeMultiPreconditionOR : public BevNodePrecondition
	{
	public:
		BevNodeMultiPreconditionOR(std::vector<BevNodePrecondition*> &conVec)
			:mPreConVec(std::move(conVec))
		{}

		~BevNodeMultiPreconditionOR()
		{
			for (auto c:mPreConVec)
			{
				SAFE_DELETE(c);
			}
		}

		virtual bool externalCondition(const BevNodeInputParam& input) const override
		{
			for (auto c:mPreConVec)
			{
				if (c->externalCondition(input))
				{
					return true;
				}
			}

			return false;
		}
	private:
		std::vector<BevNodePrecondition*> mPreConVec;
	};


	/////////BevNode///////////////
	class BevNode : public Object
	{
	public:
		BevNode(BevNode* parentNode, BevNodePrecondition* precondition = nullptr)
			: mChildNodeCount(0)
			, mActiveNode(nullptr)
			, mLastActiveNode(nullptr)
			, mNodeName("UNNAMED")
			, mNodePrecondition(nullptr)
		{
			for (DST::uint32_t i = 0; i < MAX_CHILD_NODE_COUNT;++i)
			{
				mChildNodeList[i] = nullptr;
			}

			setParentNode(parentNode);
		}

		virtual ~BevNode()
		{
			for (DST::uint32_t i = 0; i < mChildNodeCount;++i)
			{
				SAFE_DELETE(mChildNodeList[i]);
			}

			SAFE_DELETE(mNodePrecondition);
		}

		bool evaluate(const BevNodeInputParam& input)
		{
			return (mNodePrecondition == nullptr || mNodePrecondition->externalCondition(input)) && _doEvaluate(input);
		}

		void transition(const BevNodeInputParam& input)
		{
			_doTransition(input);
		}

		BevRunningStatus tick(const BevNodeInputParam& input, BevNodeOutputParam& output)
		{
			return _doTick(input, output);
		}

		BevNode& addChildNode(BevNode* node)
		{
			do 
			{
				if (nullptr == node)
				{
					//to do: log_error
					break;
				}

				if (MAX_CHILD_NODE_COUNT == mChildNodeCount)
				{
					//to do: log_error
					break;
				}

				mChildNodeList[mChildNodeCount++] = node;
			} while (0);

			return (*this);
		}

		BevNode& setPrecondition(BevNodePrecondition* con)
		{
			do 
			{
				if (nullptr == con)
				{
					//to do: log_error
					break;
				}

				mNodePrecondition = con;
			} while (0);
			return *this;
		}

		BevNode& setActiveNode(BevNode* node)
		{
			do 
			{
				if (nullptr == node)
				{
					//to do: log_error
					break;
				}

				mLastActiveNode = mActiveNode;
				mActiveNode = node;
			} while (0);

			return *this;
		}

		BevNode* getLastActiveNode() const { return mLastActiveNode; }

		BevNode& setNodeName(const char* nodename)
		{
			if (nullptr == nodename)
			{
				//to do : log_error
				return *this;
			}

			mNodeName = nodename;
			return *this;
		}

		const char* getNodeName() const { return mNodeName.c_str(); }

	protected:
		// virtual , like interface
		virtual bool _doEvaluate(const BevNodeInputParam& input)
		{
			return true;
		}

		virtual void _doTransition(const BevNodeInputParam& input)
		{

		}

		virtual BevRunningStatus _doTick(const BevNodeInputParam& input, BevNodeOutputParam& output)
		{
			return BRS_Finish;
		}

	protected:
		void setParentNode(BevNode* parentNode)
		{
			mParentNode = parentNode;
		}

		bool checkIndex(DST::uint32_t index)
		{
			return (index >= 0) && (index < mChildNodeCount);
		}

	protected:
		BevNode*				mChildNodeList[MAX_CHILD_NODE_COUNT];
		DST::uint32_t			mChildNodeCount;
		BevNode*				mParentNode;
		BevNode*				mActiveNode;
		BevNode*				mLastActiveNode;
		BevNodePrecondition*	mNodePrecondition;
		std::string				mNodeName;
	};

	class BevNodePrioritySelector : public BevNode
	{
	public:
		BevNodePrioritySelector(BevNode* parentNode, BevNodePrecondition* precondition = nullptr)
			:BevNode(parentNode, precondition)
			,mCurSelectIndex(MAX_CHILD_NODE_COUNT)
			,mLastSelectIndex(MAX_CHILD_NODE_COUNT)
		{}

		virtual bool _doEvaluate(const BevNodeInputParam& input) override;
		virtual void _doTransition(const BevNodeInputParam& input) override;
		virtual BevRunningStatus _doTick(const BevNodeInputParam& input, BevNodeOutputParam& output) override;
	protected:
		DST::uint32_t mCurSelectIndex;
		DST::uint32_t mLastSelectIndex;
	};

	class BevNodeNonePrioritySelector : public BevNodePrioritySelector
	{
	public:
		BevNodeNonePrioritySelector(BevNode* parentNode, BevNodePrecondition* precondition = nullptr)
			:BevNodePrioritySelector(parentNode, precondition)
		{}
		virtual bool _doEvaluate(const BevNodeInputParam& input) override;
	};

	class BevNodeSequence : public BevNode
	{
	public:
		BevNodeSequence(BevNode* parentNode, BevNodePrecondition* precondition = nullptr)
			: BevNode(parentNode, precondition)
			, mCurNodeIndex(MAX_CHILD_NODE_COUNT)
		{}

		virtual bool _doEvaluate(const BevNodeInputParam& input) override;
		virtual void _doTransition(const BevNodeInputParam& input) override;
		virtual BevRunningStatus _doTick(const BevNodeInputParam& input, BevNodeOutputParam& output) override;

	private:
		DST::uint32_t mCurNodeIndex;
	};

	class BevNodeTerminal : public BevNode
	{
	public:
		BevNodeTerminal(BevNode* parentNode, BevNodePrecondition* precondition = nullptr)
			: BevNode(parentNode, precondition)
			, mStatus(TNS_Ready)
			, mIsNeedExit(false)
		{}
		virtual void _doTransition(const BevNodeInputParam& input) override;
		virtual BevRunningStatus _doTick(const BevNodeInputParam& input, BevNodeOutputParam& output) override;
	protected:
		virtual bool				_doEnter(const BevNodeInputParam& input)								{}
		virtual BevRunningStatus	_doExecute(const BevNodeInputParam& input, BevNodeOutputParam& output)	{ return BRS_Finish; }
		virtual void				_doExit(const BevNodeInputParam& input, BevRunningStatus exitID)		{}
	private:
		TerminalNodeStatus	mStatus;
		bool				mIsNeedExit;
	};

	class BevNodeParallel : public BevNode
	{
	public:
		BevNodeParallel(BevNode* parentNode, BevNodePrecondition* precondition = nullptr)
			: BevNode(parentNode, precondition)
			, mFinishCondition(PFC_OR)
		{
			for (DST::uint32_t i = 0; i < MAX_CHILD_NODE_COUNT;++i)
			{
				mChildNodeStatus[i] = BRS_Executing;
			}
		}
		virtual bool _doEvaluate(const BevNodeInputParam& input) override;
		virtual void _doTransition(const BevNodeInputParam& input) override;
		virtual BevRunningStatus _doTick(const BevNodeInputParam& input, BevNodeOutputParam& output) override;

		BevNodeParallel& setFinishCondition(ParallelFinishCondition condition);
	private:
		ParallelFinishCondition	mFinishCondition;
		BevRunningStatus	mChildNodeStatus[MAX_CHILD_NODE_COUNT];
	};

	class BevNodeLoop : public BevNode
	{
	public:
		static const DST::int32_t InfiniteLoop = -1;
	public:
		BevNodeLoop(BevNode* parentNode, BevNodePrecondition* precondition = nullptr, DST::int32_t loopCount = InfiniteLoop)
			: BevNode(parentNode, precondition)
			, mLoopCount(loopCount)
			, mCurCount(0)
		{}
		virtual bool _doEvaluate(const BevNodeInputParam& input) override;
		virtual void _doTransition(const BevNodeInputParam& input) override;
		virtual BevRunningStatus _doTick(const BevNodeInputParam& input, BevNodeOutputParam& output) override;
	private:
		DST::int32_t mLoopCount;
		DST::int32_t mCurCount;
	};

	class BevNodeFactory
	{
	public:
		static BevNode& oCreateParallelNode(BevNode* parentNode, ParallelFinishCondition condition, const char* nodeName)
		{
			BevNodeParallel* pReturn = new BevNodeParallel(parentNode);
			pReturn->setFinishCondition(condition);
			oCreateNodeCommon(pReturn, parentNode, nodeName);
			return *pReturn;
		}
		static BevNode& oCreatePrioritySelectorNode(BevNode* parentNode, const char* nodeName)
		{
			BevNodePrioritySelector* pReturn = new BevNodePrioritySelector(parentNode);
			oCreateNodeCommon(pReturn, parentNode, nodeName);
			return *pReturn;
		}
		static BevNode& oCreateNonePrioritySelectorNode(BevNode* parentNode, const char* nodeName)
		{
			BevNodeNonePrioritySelector* pReturn = new BevNodeNonePrioritySelector(parentNode);
			oCreateNodeCommon(pReturn, parentNode, nodeName);
			return *pReturn;
		}
		static BevNode& oCreateSequenceNode(BevNode* parentNode, const char* nodeName)
		{
			BevNodeSequence* pReturn = new BevNodeSequence(parentNode);
			oCreateNodeCommon(pReturn, parentNode, nodeName);
			return *pReturn;
		}
		static BevNode& oCreateLoopNode(BevNode* parentNode, const char* nodeName)
		{
			BevNodeLoop* pReturn = new BevNodeLoop(parentNode);
			oCreateNodeCommon(pReturn, parentNode, nodeName);
			return *pReturn;
		}

		template<typename T>
		static BevNode& oCreateTerminalNode(BevNode* parentNode, const char* nodeName)
		{
			BevNodeTerminal* pReturn = new T(parentNode);
			oCreateNodeCommon(pReturn, parentNode, nodeName);
			return *pReturn;
		}
	private:
		static void oCreateNodeCommon(BevNode* me, BevNode* parentNode, const char* nodeName)
		{
			if (nullptr != parentNode)
			{
				parentNode->addChildNode(me);
			}
			else
			{
				//to do: log_error
			}

			me->setNodeName(nodeName);
		}
	};

}// namespace BevTree






#endif