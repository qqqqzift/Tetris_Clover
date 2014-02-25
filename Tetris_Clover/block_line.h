#ifndef BLOCK_LINE_H_
#define BLOCK_LINE_H_
#include "tetris_types.h"

using namespace block_line_class;
using namespace square_class;

class BlockLine
{


private:
	BCOLORTYPE list[MAXROWBLOCK+BACKUPROW];

public:
	BlockLine();
	BCOLORTYPE GetBlock(int);
	void SetBlock(int ,BCOLORTYPE);
};



#endif