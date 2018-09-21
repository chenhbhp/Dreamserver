
#pragma once

#ifndef _A_STAR_H__
#define _A_STAR_H__

#include <Base.h>


enum NodeType
{
	NT_Normal	= 0,
	NT_Start	= 1,
	NT_End		= 2,
	NT_Barrier	= 3
};

#define NT_Null		NT_Normal
#define NODE_NUM	10
#define MAX_NUM		100

typedef struct _AStarNode
{
	DST::int32_t x = 0;
	DST::int32_t y = 0;

	DST::uint32_t gValue = 0;
	DST::uint32_t hValue = 0;
	DST::uint8_t nodeType	= NT_Null;
	bool isInOpen	= false;
	bool isInClose	= false;
	struct _AStarNode* parent = nullptr;
}AStarNode, *PAStarNode;

AStarNode map_maze[NODE_NUM][NODE_NUM];
PAStarNode open_table[MAX_NUM];
PAStarNode close_table[MAX_NUM];





#endif //_A_STAR_H__