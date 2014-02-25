#ifndef Tetris_H_
#define Tetris_H_


#include <deque>
#include<algorithm>
#include <memory>
#include <vector>
#include "square.h"
#include "block_line.h"
#include "Tetris_types.h"


using namespace std;
typedef shared_ptr<BlockLine> LineRef;


class Tetris
{
private:
	int m_basex[TETRIS_NUM],m_basey[TETRIS_NUM];						//postion array,save the basic x,y postion of Tetris
	shared_ptr<Square> m_square[TETRIS_NUM];							//
	int m_block_image[BLOCK_COLOR+WALL_COLOR];							//block image array
	int m_bg_image;														//tetris clover back ground image
	
	TETRIS_TYPE m_ctrl_flg;												//
	int m_current_score;
	int m_score_rate;
	bool m_stop_flag[TETRIS_NUM];
	int m_fps_cnt;
	int m_flash_light_cnt;
	int m_speed;
	bool m_flash_off_flag;

	bool m_back_to_main_loop;

	deque<LineRef> status_board_up;
	deque<LineRef> status_board_down;
	deque<LineRef> status_board_left;
	deque<LineRef> status_board_right;

	void DrawFreeBlock(int x, int y, BCOLORTYPE color, const bool[SQUARE_LEN][SQUARE_LEN]);
public:
	vector<deque<LineRef>*> status_board_vector;
	
	static Tetris *me;
	void Init();
	void Main();
	void End();
	BCOLORTYPE GetBlock(int,int,const TETRIS_TYPE);								//get the status of block from status board
	void SetBlock(int,int,BCOLORTYPE,const TETRIS_TYPE);						//set true to status 
	void DrawTetrisBlock(int,int,BCOLORTYPE,const TETRIS_TYPE);					//draw block of each tetris board
	void DrawTetrisBoard(const TETRIS_TYPE);									//draw Tetris board
	void LineClearCheck(const TETRIS_TYPE);										//check the status board and clear good line
	void ClearLine(const TETRIS_TYPE);											//clear
	void SquareCtrl(const TETRIS_TYPE,weak_ptr<Square>);						//read key to move square
	void SquareSetCheck(const TETRIS_TYPE,weak_ptr<Square>);					//check if set square
	bool GameoverCheck(const TETRIS_TYPE);										//check if game is over
	void CtrlBoardChangine(const TETRIS_TYPE);									//
	void StopMenu();															//show stop menu

	void ShowNextInfo(int, int, BCOLORTYPE, SQUARETYPE);
	void SpeedUp(){ if (m_speed <= 16){ m_speed = m_speed * 2; m_score_rate = m_score_rate * 2; } }
	void SpeedDown(){ if ((m_speed >= 2)&&(m_score_rate >= 2)){ m_speed = m_speed / 2; m_score_rate = m_score_rate / 2; } }
	void ShowHelp();
};



#endif