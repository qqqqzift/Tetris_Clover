#include "block_line.h"


BlockLine::BlockLine()
{
	int i = 1;
	list[0] = WALL_X;
	for( i ;i < MAXROWBLOCK;i++)
	{
		list[i] = BLACK;
	}
	list[MAXROWBLOCK] = WALL_X;
}

BCOLORTYPE BlockLine::GetBlock(int rowid)
{
	if(rowid <= MAXROWBLOCK)
		return list[rowid];
	else
		return WALL_X;
}

void BlockLine::SetBlock(int rowid,BCOLORTYPE flag)
{
	list[rowid] = flag;
}