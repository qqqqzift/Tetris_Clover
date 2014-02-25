#include "square.h"
#include "tetris.h"
#include "DxLib.h"
#include "my_log.h"
#include<ctime> 

//TYPE_O
const bool k_type_o[SQUARE_LEN][SQUARE_LEN]={
	{true,true,false,false},
	{true,true,false,false},
	{false,false,false,false},
	{false,false,false,false}
};
//TYPE_I
const bool k_type_i[SQUARE_LEN][SQUARE_LEN]={
	{false,false,false,false},
	{false,false,false,false},
	{true,true,true,true},
	{false,false,false,false},

};

//TYPE_S
const bool k_type_s[SQUARE_LEN][SQUARE_LEN]={
	{true,false,false,false},
	{true,true,false,false},
	{false,true,false,false},
	{false,false,false,false},
};


//TYPE_Z
const bool k_type_z[SQUARE_LEN][SQUARE_LEN]={
	{false,true,false,false},
	{true,true,false,false},
	{true,false,false,false},
	{false,false,false,false},
};

//TYPE_T
const bool k_type_t[SQUARE_LEN][SQUARE_LEN]={
	{true,true,true,false},
	{false,true,false,false},
	{false,false,false,false},
	{false,false,false,false},
};

//TYPE_L
const bool k_type_l[SQUARE_LEN][SQUARE_LEN]={
	{true,false,false,false},
	{true,false,false,false},
	{true,true,false,false},
	{false,false,false,false},
};


//TYPE_J
const bool k_type_j[SQUARE_LEN][SQUARE_LEN]={
	{false,true,false,false},
	{false,true,false,false},
	{true,true,false,false},
	{false,false,false,false},
};
Square::Square(const TETRIS_TYPE t_type)
{
	GetNextSquare();
	Reset(t_type);
}

//local function: turn array counter clock
void Square::ArrayCounterClock(bool dst[][SQUARE_LEN],const bool src[][SQUARE_LEN],int len)
{
	for(int i = 0;i<len;i++)
		for(int j = 0;j<len;j++)
			dst[len-j-1][i] = src[i][j];	
}

//local function:copy src array to dst
void Square::ArrayCopy(bool dst[][SQUARE_LEN],const bool src[][SQUARE_LEN],int len,const TETRIS_TYPE t_type)
{
	memset(dst,false,len*len*(sizeof(bool)));
	for(int i = 0;i<len;i++){
		for(int j = 0;j<len;j++){
			dst[i][j] = src[i][j];
		}
	}
}

//NextConflictCheck:check if square is conflict to status board at given position 
//true:exist conflict block
//false:not exist conflict block
bool Square::NextConflictCheck(const bool tgt[][SQUARE_LEN],int len,int cur_line,int cur_row,const TETRIS_TYPE t_type)
{
	BCOLORTYPE t_ctype;
	//check if already exist block
	for(int i = 0;i<len;i++){
		for(int j = 0;j<len;j++){
			if(tgt[i][j]){
				//
				t_ctype = Tetris::me->status_board_vector[t_type]->at(cur_line+i)->GetBlock(cur_row+j);
				
				if(t_ctype != BLACK)
					return true;		
			}
		}
	}
	return false;
}

//ConflictCheck:check if square is confict to status board at recent position
bool Square::ConflictCheck(const TETRIS_TYPE t_type)
{
	bool c_flag;

	c_flag = NextConflictCheck(m_shape,SQUARE_LEN,m_current_line,m_current_row,t_type);

	return c_flag;
}


bool Square::TryMoveTypeI(int &t_row_move,const bool copy[SQUARE_LEN][SQUARE_LEN] ,const TETRIS_TYPE t_type)
{
	bool t_copy_flag = true;
	int movable[] = {1,2,-1,-2};

	for(int i = 0;i<(sizeof(movable)/sizeof(int));i++){
		//boundary check
		if(((m_current_row+movable[i])<0)&&
			((m_current_row+movable[i])>MAXROWBLOCK))
			continue;

		t_row_move = movable[i];
		t_copy_flag = NextConflictCheck(copy,SQUARE_LEN,m_current_line,m_current_row+movable[i],t_type);
		if(t_copy_flag == false)
			return t_copy_flag;
	}
	return t_copy_flag;
}

void Square::TurnArray(int len,const TETRIS_TYPE t_type)
{
	bool copy[SQUARE_LEN][SQUARE_LEN];
	//vector<vector<int> > copy(len, vector<int>(len));


	bool t_copy_flag = true;;
	int t_row_move = 0;
	//turn the array
	ArrayCounterClock(copy,m_shape,len);
	
	UpdateBlockLength(t_type);
	//check conflict
	t_copy_flag = NextConflictCheck(copy,len,m_current_line,m_current_row,t_type);

	//check TYPE_S,TYPE_Z,TYPE_T,TYPE_L,TYPE_J
	if((m_stype != TYPE_I)&&(t_copy_flag == true)){
		t_row_move = -1;
		t_copy_flag = NextConflictCheck(copy,len,m_current_line,m_current_row+t_row_move,t_type);
	}
	if((m_stype != TYPE_I)&&(t_copy_flag == true)){
		t_row_move = 1;
		t_copy_flag = NextConflictCheck(copy,len,m_current_line,m_current_row+t_row_move,t_type);
	}

	//check TYPE_I
	if((m_stype == TYPE_I)&&(t_copy_flag == true))
		t_copy_flag =TryMoveTypeI(t_row_move,copy,t_type);

	
	//if move failed set count to 0
	if(t_copy_flag)
		t_row_move = 0;


	//final movable check
	if(t_copy_flag){
		UpdateBlockLength(t_type);
		return ;
	}

	//copy turn result to sharp status
	ArrayCopy(m_shape,copy,len,t_type);

	//move left
	m_current_row = m_current_row+t_row_move;
}

void Square::UpdateBlockLength(const TETRIS_TYPE t_type)
{
	int temp;
	temp = m_line_block_num;
	m_line_block_num = m_row_block_num;
	m_row_block_num = temp;
}


void Square::Turn(const TETRIS_TYPE t_type)
{
	int t_flag = false;
	
	if(m_stype == TYPE_O)
		return ;
	
	if(m_stype == TYPE_I){
		TurnArray(4,t_type);
	}	
	else{//TYPE_S,TYPE_Z,TYPE_T,TYPE_L,TYPE_J
		TurnArray(3,t_type);
	}
	
}


//read input key and move the square
void Square::Move(DIRECTION d,const TETRIS_TYPE t_type)
{
	bool t_conflict_flag = false;
	switch(d){
	case kGOLEFT:
		t_conflict_flag = NextConflictCheck(m_shape,SQUARE_LEN,m_current_line,m_current_row-1,t_type);
		if(!t_conflict_flag)m_current_row = m_current_row-1;
		break;
	case kGORIGHT:
		t_conflict_flag = NextConflictCheck(m_shape,SQUARE_LEN,m_current_line,m_current_row+1,t_type);
		if(!t_conflict_flag)m_current_row = m_current_row+1;
		break;
	case kGODOWN:
		Drop(t_type);
		break;
	default:
		break;
	}
}




//reset moving square
void Square::Reset(const TETRIS_TYPE t_type)
{
	
	m_current_line = 0;
	m_current_row = GetRand((unsigned) time(NULL))%6+1;

	m_stype = m_next_stype;
	//m_stype = TYPE_I;

	m_bctype = m_next_bctype;	
	
	GetNextSquare();
	switch(m_stype){//TYPE_O,TYPE_I,TYPE_S,TYPE_Z,TYPE_T,TYPE_L,TYPE_J
	case TYPE_O:
		ArrayCopy(m_shape,k_type_o,SQUARE_LEN,t_type);
		m_line_block_num = 2;
		m_row_block_num = 2;
		break;
	case TYPE_I:
		ArrayCopy(m_shape,k_type_i,SQUARE_LEN,t_type);
		m_line_block_num = 1;
		m_row_block_num = 4;
		break;
	case TYPE_S:
		ArrayCopy(m_shape,k_type_s,SQUARE_LEN,t_type);
		m_line_block_num = 2;
		m_row_block_num = 3;
		break;
	case TYPE_Z:
		ArrayCopy(m_shape,k_type_z,SQUARE_LEN,t_type);
		m_line_block_num = 2;
		m_row_block_num = 3;
		break;
	case TYPE_T:
		ArrayCopy(m_shape,k_type_t,SQUARE_LEN,t_type);
		m_line_block_num = 3;
		m_row_block_num = 2;
		break;
	case TYPE_L:
		ArrayCopy(m_shape,k_type_l,SQUARE_LEN,t_type);
		m_line_block_num = 2;
		m_row_block_num = 3;
		break;
	case TYPE_J:
		ArrayCopy(m_shape,k_type_j,SQUARE_LEN,t_type);
		m_line_block_num = 2;
		m_row_block_num = 3;
		break;
	default:
		LogWriter::me->WriteLog("Square Reset recieve unhandle square type");
		break;
	}


}

//draw square
void Square::Draw(const TETRIS_TYPE t_type)
{
	for(int i = 0;i < SQUARE_LEN;i++)
	{
		for(int j = 0;j <SQUARE_LEN;j++)
		{
			if(m_shape[i][j]){
				Tetris::me->DrawTetrisBlock(m_current_line + i, m_current_row + j, m_bctype, t_type);
			}
		}
	}
}

//set square to status board
void Square::SetSquare(const TETRIS_TYPE t_type)
{
	for(int i = 0;i < SQUARE_LEN;i++)
	{
		for(int j = 0;j <SQUARE_LEN;j++)
		{
			if(m_shape[i][j]){
				Tetris::me->SetBlock(m_current_line+i,m_current_row+j,m_bctype,t_type);
			}
		}
	}	
	Reset(t_type);
}

//move Å´ square
void Square::Drop(const TETRIS_TYPE t_type)
{
	bool t_conflict_flag = NextConflictCheck(m_shape,SQUARE_LEN,m_current_line+1,m_current_row,t_type);
	if(!t_conflict_flag)
		m_current_line = m_current_line+1;
	else
		SetSquare(t_type);
}


void Square::GetNextSquare()
{
	m_next_stype = SQUARETYPE(GetRand((unsigned)time(NULL)) % 7);;							//next square type 
	m_next_bctype = BCOLORTYPE(GetRand((unsigned)time(NULL)) % 6 + 1);;						//next square color
}