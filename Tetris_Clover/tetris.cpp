//#define NDEBUG
//#include <assert.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include "tetris.h"
#include "fps.h"
#include "keyboard.h"
#include "my_log.h"
#include "DxLib.h"
#include "tetris_game.h"

using namespace tetris_class;
using namespace std;
Tetris* Tetris::me;

MenuList_t g_stop_menu[kStopMenuLen] = {
	{ 80, 100, "ヘルプ" },
	{ 100, 150, "ゲームに戻る" },
	{ 100, 200, "最初からやり直す" },
	{ 100, 250, "タイトルにもとる" },
	{ 100, 300, "ゲーム終了" }
};

struct NextSquare{
	BCOLORTYPE m_color;
	SQUARETYPE m_stype;
};

typedef NextSquare NextSquare;
void Tetris::Init()
{
	
	m_fps_cnt = 0;

	//initialize all array
	status_board_vector.push_back(&status_board_up);
	status_board_vector.push_back(&status_board_down);
	status_board_vector.push_back(&status_board_left);
	status_board_vector.push_back(&status_board_right);
	for(int h = 0 ; h < TETRIS_NUM;h++){
		//Tetris square stop flag initialize
		m_stop_flag[TETRIS_TYPE(h)] = false;
		//square array initialize
		m_square[TETRIS_TYPE(h)] = make_shared<Square>(TETRIS_TYPE(h));
		//tetris status board initialize
		for(int i = 0;i < (MAXLINEBLOCK+BACKUPLINE);i++)
		{
			LineRef tprefup(new BlockLine());
			status_board_vector[TETRIS_UP]->push_back(tprefup);
			LineRef tprefdown(new BlockLine());
			status_board_vector[TETRIS_DOWN]->push_back(tprefdown);
			LineRef tprefleft(new BlockLine());
			status_board_vector[TETRIS_LEFT]->push_back(tprefleft);
			LineRef tprefright(new BlockLine());
			status_board_vector[TETRIS_RIGHT]->push_back(tprefright);
		}
		//initialize bottom boundary line of tetris status board
		for(int i = 0 ; i< MAXROWBLOCK+BACKUPROW;i++)
		{
			status_board_vector[TETRIS_UP]->at(MAXLINEBLOCK)->SetBlock(i,WALL_X);
			status_board_vector[TETRIS_DOWN]->at(MAXLINEBLOCK)->SetBlock(i,WALL_X);
			status_board_vector[TETRIS_LEFT]->at(MAXLINEBLOCK)->SetBlock(i,WALL_X);
			status_board_vector[TETRIS_RIGHT]->at(MAXLINEBLOCK)->SetBlock(i,WALL_X);
		}			
	}

	//set me pointer
	me = this;

	//
	m_basex[TETRIS_UP] = 300;
	m_basey[TETRIS_UP] = 330;

	m_basex[TETRIS_DOWN] = 300;
	m_basey[TETRIS_DOWN] = 0;

	m_basex[TETRIS_LEFT] = 100;
	m_basey[TETRIS_LEFT] = 200;

	m_basex[TETRIS_RIGHT] = 440;
	m_basey[TETRIS_RIGHT] = 200;
	

	//initialize block color
	//enum BCOLORTYPE{BLACK,RED,ORANGE,YELLOW,GREEN,SKY,BLUE,PURPLE,WALL_O,WALL_X};
	m_block_image[BLACK] = LoadGraph( "Picture/Block_BL.png" ); 
	m_block_image[RED] = LoadGraph( "Picture/Block_R.png" ); 
	m_block_image[ORANGE] = LoadGraph( "Picture/Block_O.png" ); 
	m_block_image[YELLOW] = LoadGraph( "Picture/Block_Y.png" ); 
	m_block_image[GREEN] = LoadGraph( "Picture/Block_G.png" ); 
	m_block_image[SKY] = LoadGraph( "Picture/Block_S.png" ); 
	m_block_image[BLUE] = LoadGraph( "Picture/Block_B.png" );
	m_block_image[PURPLE] = LoadGraph( "Picture/Block_P.png" );
	m_block_image[WALL_S] = LoadGraph( "Picture/Wall_S.png" );
	m_block_image[WALL_X] = LoadGraph( "Picture/Wall_X.png" );
	m_bg_image = LoadGraph( "Picture/cover.jpg");			


	//score initialize
	m_current_score = 0;
	m_speed = 2;
	m_flash_light_cnt = 0;
	m_ctrl_flg = TETRIS_UP;
	m_score_rate = 16;
	//flash light flag
	m_flash_off_flag = false;
}

void Tetris::Main()
{
	
	Fps fps_counter;
	
	int t_drop_cnt[TETRIS_NUM] ;
	bool t_game_over_message;
	

	NextSquare t_next_square[TETRIS_NUM];
	int t_next_position[TETRIS_NUM][2] = {
		{ 700, 0},
		{ 700, 50 },
		{ 700, 100 },
		{ 700, 150 }
	};
	char t_message[TETRIS_NUM][8] = {
		"下","上","左","右"
	};
	char t_status[2][16] = {
		"正常", "停止"
	};

	for (int i = 0; i < TETRIS_NUM; i++)
		t_drop_cnt[i] = 0;
	while( ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0 ){
		//draw back groud image
		DrawGraph(0, 0, m_bg_image, TRUE);

		//fps control and update
		fps_counter.Update();
		fps_counter.Draw();
		fps_counter.Wait();
		m_fps_cnt =fps_counter.GetCounter();
		
		
		if((m_fps_cnt%5)== 0)	//read input 
				SquareCtrl(TETRIS_TYPE(m_ctrl_flg),m_square[TETRIS_TYPE(m_ctrl_flg)]);
		for(int i = 0;i < TETRIS_NUM;i++){

			//draw Tetris board
			DrawTetrisBoard(TETRIS_TYPE(i));

			//check if set the square
			SquareSetCheck(TETRIS_TYPE(i),m_square[TETRIS_TYPE(i)]);		

			//square draw
			if(m_stop_flag[TETRIS_TYPE(i)] == false)
				m_square[TETRIS_TYPE(i)]->Draw(TETRIS_TYPE(i));

			//drop square every 2(120/60) second
			if ((((t_drop_cnt[i]++) % (120 / m_speed)) == 0) &&
				(m_stop_flag[TETRIS_TYPE(i)] == false))
				m_square[i]->Drop(TETRIS_TYPE(i));
		 
			if((m_fps_cnt%30)== 0)
				LineClearCheck(TETRIS_TYPE(i));

			if(m_stop_flag[TETRIS_TYPE(i)] == false)
				GameoverCheck(TETRIS_TYPE(i));
			
			t_next_square[i].m_stype = m_square[TETRIS_TYPE(i)]->m_next_stype;
			t_next_square[i].m_color = m_square[TETRIS_TYPE(i)]->m_next_bctype;
		}	
		DrawFormatString(0, 30, GetColor(0,0,0), "スコア:%d",m_current_score);
		DrawFormatString(0, 60, GetColor(0, 0, 0), "倍率:%dX", m_score_rate);
		DrawFormatString(0, 90, GetColor(0, 0, 0), "速度:%dX", m_speed);
		for (int i = 0; i < TETRIS_NUM; i++){
			if (m_stop_flag[i] == false){
				DrawFormatString((t_next_position[i][0] - 180), t_next_position[i][1], GetColor(0, 0, 0), "ネクストテトリス(%s):", t_message[i]);
				ShowNextInfo(t_next_position[i][0], t_next_position[i][1], t_next_square[i].m_color, t_next_square[i].m_stype);			
			}
			
			DrawFormatString(0, t_next_position[i][1] + 420, GetColor(0, 0, 0), "テトリス状態(%s):%s", t_message[i], t_status[m_stop_flag[i]]);
		}

		t_game_over_message = true;
		for(int i = 0;i < TETRIS_NUM ;i++){
			if(m_stop_flag[TETRIS_TYPE(i)] == false){
				t_game_over_message = false;
				break;
			}
		}
		
		if (t_game_over_message){
			DrawFormatString(200, 300, GetColor(0,0,0), "すべて4つのテトリスが止まりました、ESCでオプションが呼べます");
		}
		KeyboardUpdate();
		if (KeyboardGet(KEY_INPUT_ESCAPE) == 1){
			StopMenu();
		}

		//if got event in queue break game return to main loop
		if ((TetrisGame::me->EventInQueue() == true)||
			(m_back_to_main_loop == true))
			break;
	 }
}

void Tetris::End()
{
	//deque release
	for(int i = 0;i < TETRIS_NUM;i++){
		
		//square release
		m_square[i].reset();
	}
	//stauts borad deque release
	for(int i = 0 ; i < TETRIS_NUM;i++)
	{
		status_board_vector[i]->clear();
	}
	vector<deque<LineRef>*>().swap(status_board_vector); 
}


//set the block to the status board
void Tetris::SetBlock(int lid,int rid,BCOLORTYPE flag,const TETRIS_TYPE t_type)
{
	status_board_vector[t_type]->at(lid)->SetBlock(rid,flag);
}

//get the block status from status board
BCOLORTYPE Tetris::GetBlock(int lid,int rid,const TETRIS_TYPE t_type)
{
	return status_board_vector[t_type]->at(lid)->GetBlock(rid);
}

//BACKUPLINE MAXLINEBLOCK
void Tetris::DrawTetrisBlock(int lid, int rid, BCOLORTYPE color, const TETRIS_TYPE t_type)
{
	if(t_type == TETRIS_UP)
		DrawGraph( m_basex[t_type]+rid*BLOCK_WIDTH, m_basey[t_type]+lid*BLOCK_WIDTH, m_block_image[color], TRUE ); 
	else if(t_type == TETRIS_DOWN)
		DrawGraph( m_basex[t_type]+(MAXROWBLOCK  - rid)*BLOCK_WIDTH, m_basey[t_type]+(MAXLINEBLOCK - lid)*BLOCK_WIDTH, m_block_image[color], TRUE ); 
	else if(t_type == TETRIS_LEFT)
		DrawGraph( m_basex[t_type]+(MAXLINEBLOCK - lid)*BLOCK_WIDTH, m_basey[t_type]+(MAXROWBLOCK  - rid)*BLOCK_WIDTH, m_block_image[color], TRUE ); 
	else if(t_type == TETRIS_RIGHT)
		DrawGraph( m_basex[t_type]+lid*BLOCK_WIDTH, m_basey[t_type]+rid*BLOCK_WIDTH, m_block_image[color], TRUE ); 
		
}

void Tetris::DrawTetrisBoard(const TETRIS_TYPE t_type)
{
	bool flash_flag = false;

	BCOLORTYPE t_ctype ;
	for(int i = BACKUPLINE;i <= MAXLINEBLOCK;i++)
	{
		for(int j = 0; j <= MAXROWBLOCK; j++ )
		{
			t_ctype = status_board_vector[t_type]->at(i)->GetBlock(j);
			if(t_ctype!=BLACK)
				DrawTetrisBlock(i, j, t_ctype, t_type);
		}	
	}

	if(!m_flash_off_flag){
		if(m_flash_light_cnt < 4)
				m_flash_light_cnt = m_flash_light_cnt + BACKUPLINE;
		if((m_fps_cnt % 20) == 0){//MAXLINEBLOCK+BACKUPLINE
			m_flash_light_cnt = (m_flash_light_cnt++%(MAXLINEBLOCK));     //move flash light every 20 flame
			flash_flag = true;
		}
		if(m_ctrl_flg == t_type){
			DrawTetrisBlock(m_flash_light_cnt, 0, WALL_S, t_type);
			DrawTetrisBlock(m_flash_light_cnt, MAXROWBLOCK, WALL_S, t_type);
		}
	}
}


void Tetris::LineClearCheck(const TETRIS_TYPE t_type)
{
	//insert or erase will cause deque iterator lost
	int cleared_cnt = 0;
	int i = 0;
	
	for(auto it = status_board_vector[t_type]->begin();it!=status_board_vector[t_type]->end();)
	{
		int cnt = 0;
		i++;
		if(i > MAXLINEBLOCK)//skip check of the bottom line
			break;
		for(int j = 0; j < MAXROWBLOCK; j++ )//
		{			
			if((*it)->GetBlock(j))
				cnt++;
		}
		if(cnt == MAXROWBLOCK){
			(*it).reset();
			it = status_board_vector[t_type]->erase(it);
			cleared_cnt++;
		}else{
			it++;
		}
	}
	while(cleared_cnt!= 0){
		LineRef tpref(new BlockLine());
		status_board_vector[t_type]->push_front(tpref);
		m_current_score = m_current_score+(10*cleared_cnt)*m_score_rate*m_speed;
		cleared_cnt--;
	}

}



//read input key to move square
void Tetris::SquareCtrl(const TETRIS_TYPE t_type,weak_ptr<Square> t_wsquare)
{
	if(!t_wsquare.expired()){
		shared_ptr<Square> t_ssquare = t_wsquare.lock();
		KeyboardUpdate();
		//down button is pressed
		if(((KeyboardGet(KEY_INPUT_DOWN) > 0)&&(t_type == TETRIS_UP))|| 
			((KeyboardGet(KEY_INPUT_UP) > 0)&&(t_type == TETRIS_DOWN))||
			((KeyboardGet(KEY_INPUT_RIGHT) > 0)&&(t_type == TETRIS_RIGHT))||
			((KeyboardGet(KEY_INPUT_LEFT) > 0)&&(t_type == TETRIS_LEFT))
			){
			t_ssquare->Move(kGODOWN,t_type);
		}
		//left button is pressed
		else if(((KeyboardGet(KEY_INPUT_LEFT) > 0)&&(t_type == TETRIS_UP))|| 
			((KeyboardGet(KEY_INPUT_RIGHT) > 0)&&(t_type == TETRIS_DOWN))||
			((KeyboardGet(KEY_INPUT_DOWN) > 0)&&(t_type == TETRIS_LEFT))||
			((KeyboardGet(KEY_INPUT_UP) > 0)&&(t_type == TETRIS_RIGHT))
			){
			t_ssquare->Move(kGOLEFT,t_type);
		}
		//right button is pressed
		else if(((KeyboardGet(KEY_INPUT_RIGHT) > 0)&&(t_type == TETRIS_UP))|| 
			((KeyboardGet(KEY_INPUT_LEFT) > 0)&&(t_type == TETRIS_DOWN))||
			((KeyboardGet(KEY_INPUT_UP) > 0)&&(t_type == TETRIS_LEFT))||
			((KeyboardGet(KEY_INPUT_DOWN) > 0)&&(t_type == TETRIS_RIGHT))
			){	
			t_ssquare->Move(kGORIGHT,t_type);
		}
		else if(((KeyboardGet(KEY_INPUT_UP) > 0)&&(t_type == TETRIS_UP))|| 
			((KeyboardGet(KEY_INPUT_DOWN) > 0)&&(t_type == TETRIS_DOWN))||
			((KeyboardGet(KEY_INPUT_RIGHT) > 0)&&(t_type == TETRIS_LEFT))||
			((KeyboardGet(KEY_INPUT_LEFT) > 0)&&(t_type == TETRIS_RIGHT))
			){
			t_ssquare->Turn(t_type);
		}else if(KeyboardGet(KEY_INPUT_SPACE ) > 0){
			CtrlBoardChangine(t_type);
		}
		else if (KeyboardGet(KEY_INPUT_Z) == 1){
			SpeedUp();
		}
		else if (KeyboardGet(KEY_INPUT_X) == 1){
			SpeedDown();
		}

	}
}
void Tetris::CtrlBoardChangine(const TETRIS_TYPE  t_type)
{
	for(int i = 0,temp = t_type;i < TETRIS_NUM;i++)
	{
		m_flash_off_flag = true;
		m_flash_off_flag = m_stop_flag[(++temp)%TETRIS_NUM];
		if(m_flash_off_flag == false){
			m_ctrl_flg = TETRIS_TYPE(temp%TETRIS_NUM);
			m_flash_light_cnt = 0;
			break;
		}
	}

}

//check if it's the right place to set square if so ,set it
void Tetris::SquareSetCheck(const TETRIS_TYPE t_type,weak_ptr<Square> t_wsquare)
{
	if(!t_wsquare.expired()){
		shared_ptr<Square> t_ssquare = t_wsquare.lock();
		if(t_ssquare->ConflictCheck(t_type) == true)
		{
			t_ssquare->SetSquare(t_type);
		}
	}else{
		LogWriter::me->WriteLog("square expired in SquareSetCheck");
	}
}

//check if game is over
bool Tetris::GameoverCheck(const TETRIS_TYPE t_type)
{
	
	for(int i = 0;i < BACKUPLINE;i++)
	{
		for(int j = 1; j < MAXROWBLOCK-1; j++ )
		{
			if((status_board_vector[t_type]->at(i)->GetBlock(j)!=BLACK)&&
				(status_board_vector[t_type]->at(i)->GetBlock(j)!=WALL_S)&&
				(status_board_vector[t_type]->at(i)->GetBlock(j)!=WALL_X)){
				//stop moving and drawing the moving square
				m_stop_flag[t_type] = true;

				//drop the score rate
				if(m_score_rate>2)
					m_score_rate = m_score_rate/2;

				//change control to another
				CtrlBoardChangine(t_type);
				return true;
			}
				
		}
	}
	return false;

}




//check if game is over
void Tetris::StopMenu()
{
	int t_selected_item = 0;
	bool t_back_to_game_flag = false;
	int t_menu_item = TetrisGame::me->GetItemImage(kMenu);
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0){
		DrawGraph(0, 0, t_menu_item, TRUE);
		DrawFormatString(550, 500, GetColor(0, 0, 0), "ESCキで戻る");
		KeyboardUpdate();

		if (KeyboardGet(KEY_INPUT_DOWN) == 1){ 

			t_selected_item = (t_selected_item + 1) % kStopMenuLen; 
		}

		if (KeyboardGet(KEY_INPUT_UP) == 1){ 

			t_selected_item = (t_selected_item + kStopMenuLen - 1) % kStopMenuLen; 
		}

		if (KeyboardGet(KEY_INPUT_DOWN) == 1 || KeyboardGet(KEY_INPUT_UP) == 1){ //once up or down is pressed
			for (int i = 0; i < kStopMenuLen; i++){
				if (i == t_selected_item){
					g_stop_menu[i].x = 80;					//selected item
				}
				else {
					g_stop_menu[i].x = 100;
				}
			}
		}
		if (KeyboardGet(KEY_INPUT_ESCAPE) == 1){
			break;
		}
		// draw menu
		for (int i = 0; i < kStopMenuLen; i++){
			DrawFormatString(g_stop_menu[i].x, g_stop_menu[i].y, GetColor(0, 0, 0), g_stop_menu[i].name);
		}
		if (KeyboardGet(KEY_INPUT_RETURN) == 1){
			switch (t_selected_item){
			case 0://help
				ShowHelp();
				break;
			case 1://return to game
				t_back_to_game_flag = true;
				break;
			case 2://restart
				t_back_to_game_flag = true;
				m_back_to_main_loop = true;
				TetrisGame::me->SetEventFlag(StartNewTetrisEVENT);
				break;
			case 3://back to title
				t_back_to_game_flag = true;
				m_back_to_main_loop = true;
				TetrisGame::me->SetEventFlag(ShowStartMenuEVENT);
				break;
			case 4://game over
				t_back_to_game_flag = true;
				m_back_to_main_loop = true;
				TetrisGame::me->SetEventFlag(GAMEOVEREVENT);
				break;
			default:
				break;
			}
		}
		
		if (t_back_to_game_flag == true)
			break;
	}
}
void Tetris::DrawFreeBlock(int x, int y, BCOLORTYPE color, const bool t_next_square[SQUARE_LEN][SQUARE_LEN])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (t_next_square[i][j] == true)
				DrawGraph(x + j*BLOCK_WIDTH, y + i*BLOCK_WIDTH, m_block_image[color], TRUE);
		}
	}
}

//TYPE_O, TYPE_I, TYPE_S, TYPE_Z, TYPE_T, TYPE_L, TYPE_J
void Tetris::ShowNextInfo(int x, int y, BCOLORTYPE t_color, SQUARETYPE t_stype)
{
	switch (t_stype){
	case TYPE_O:
		DrawFreeBlock(x, y, t_color, k_type_o);
		break;
	case TYPE_I:
		DrawFreeBlock(x, y, t_color, k_type_i);
		break;
	case TYPE_S:
		DrawFreeBlock(x, y, t_color, k_type_s);
		break;
	case TYPE_Z:
		DrawFreeBlock(x, y, t_color, k_type_z);
		break;
	case TYPE_T:
		DrawFreeBlock(x, y, t_color, k_type_t);
		break;
	case TYPE_L:
		DrawFreeBlock(x, y, t_color, k_type_l);
		break;
	case TYPE_J:
		DrawFreeBlock(x, y, t_color, k_type_j);
		break;
	default:
		break;
	}

}


//void ShowHelp()
void Tetris::ShowHelp()
{
	int count = 0, i;
	Fps fps_counter;
	int t_help_image = TetrisGame::me->GetItemImage(kHelp);
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0){
		fps_counter.Update();
		fps_counter.Wait();
		DrawGraph(0, 0, t_help_image, TRUE);

		DrawFormatString(220, 180, GetColor(0, 0, 0), "方向キ(テトリスによって違うので、Defaultを説明する)");
		DrawFormatString(220, 200, GetColor(0, 0, 0), "←キ:左にいく");
		DrawFormatString(220, 220, GetColor(0, 0, 0), "→キ:右にいく");
		DrawFormatString(220, 240, GetColor(0, 0, 0), "↓キ:下にいく");
		DrawFormatString(220, 260, GetColor(0, 0, 0), "↑キ:落下スクエアを回転する");

		DrawFormatString(220, 280, GetColor(0, 0, 0), "操作キ");
		DrawFormatString(220, 300, GetColor(0, 0, 0), "Spaceキ:正常状態の他のテトリスに移る");
		DrawFormatString(220, 320, GetColor(0, 0, 0), "Zキ:ゲーム加速");
		DrawFormatString(220, 340, GetColor(0, 0, 0), "Xキ:ゲーム減速");
		DrawFormatString(550, 360, GetColor(0, 0, 0), "ESCキで戻る");
		KeyboardUpdate();
		if (KeyboardGet(KEY_INPUT_ESCAPE) == 1){
			
			break;
		}


	}
}