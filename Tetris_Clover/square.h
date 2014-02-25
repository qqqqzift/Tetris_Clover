#ifndef SQUARE_H_
#define SQUARE_H_
#include "tetris_types.h"

using namespace square_class;
using namespace tetris_class;
class Square
{
private:
	int m_current_line;									//current block line position
	int m_current_row;									//current block row position
	int m_line_block_num;								//current max block number in line
	int m_row_block_num;								//current max block number in row
	bool m_shape[SQUARE_LEN][SQUARE_LEN];				//square shape array
	SQUARETYPE m_stype;									//current square type 
	BCOLORTYPE m_bctype;								//current square color


	void ArrayCounterClock(bool [][SQUARE_LEN],const bool [][SQUARE_LEN],int);
	
	//child process of TURN function,main process to turn square
	void TurnArray(int,const TETRIS_TYPE);

	//check if square is conflict to status board at given position
	bool NextConflictCheck(const bool [][SQUARE_LEN],int,int,int,const TETRIS_TYPE);									
	
	//copy array to 
	void ArrayCopy(bool [][SQUARE_LEN],const bool [][SQUARE_LEN],int,const TETRIS_TYPE);

	//swap the square length while turn the square
	void UpdateBlockLength(const TETRIS_TYPE t_type);

	//if conflict try move I square one/two block left or one/two block right
	bool TryMoveTypeI(int &,const bool [][SQUARE_LEN],const TETRIS_TYPE);

	//get color and shape of next square
	void GetNextSquare();
public:
	Square(const TETRIS_TYPE);
	SQUARETYPE m_next_stype;							//next square type 
	BCOLORTYPE m_next_bctype;							//next square color

	//check if square is confict to status board at recent position
	bool ConflictCheck(const TETRIS_TYPE);														
	
	//turn the square 90Åãcounter-clock wise
	void Turn(const TETRIS_TYPE);																
	
	//move to left or right or down,if conflict move back
	void Move(DIRECTION,const TETRIS_TYPE);															
	
	//get current square block color
	BCOLORTYPE GetBCOLOR(){return m_bctype;}
	
	//set square to status board
	void SetSquare(const TETRIS_TYPE);															
	
	//move Å´ square
	void Drop(const TETRIS_TYPE);																
	
	//reset the moving square
	void Reset(const TETRIS_TYPE);															

	//draw the moving square
	void Draw(const TETRIS_TYPE);

	~Square(){}
};



#endif