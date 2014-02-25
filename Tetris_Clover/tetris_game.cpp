#include "tetris_types.h"
#include "tetris_game.h"
#include "high_score.h"
#include "keyboard.h"
#include "tetris.h"
#include "DxLib.h"
#include "fps.h"

TetrisGame* TetrisGame::me;
//start menu
MenuList_t g_start_menu[kSTartMenuLen] = {
	{ 480, 300, "ゲームスタート" }, 
	{ 500, 350, "ハイスコア" },
	{ 500, 400, "ゲーム終了" },
	//{ 100, 250, "" }
};
void TetrisGame::Init(){
	//windows initialize
	SetMainWindowText("Tetris Clover");
	ChangeWindowMode(TRUE);
	SetGraphMode(WINWIDTH, WINHEIGHT, 32);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetBackgroundColor(255, 255, 255);
	me = this;
	m_game_over_flag = false;
	m_title_image = LoadGraph("Picture/title.png");
	m_item_image[kClover] = LoadGraph("Picture/t_clover.png");
	m_item_image[kHighscore] = LoadGraph("Picture/t_high_score.png");
	m_item_image[kMenu] = LoadGraph("Picture/t_menu.png");
	m_item_image[kHelp] = LoadGraph("Picture/t_help.png");
	SetEventFlag(ShowStartMenuEVENT);
	//
}

void TetrisGame::StartNewTetrisGame()
{
	Tetris t_teris;

	t_teris.Init();
	t_teris.Main();
	t_teris.End();
	

}

void TetrisGame::Main()
{
	//main loop
	while (m_game_over_flag == false){
		DoEvent();
	}
}
void TetrisGame::SetEventFlag(STARTMENU_EVENT_TYPE t_ctrl_flag){
	m_event_queue.push_back(t_ctrl_flag);
}

void TetrisGame::EndGame()
{
	DxLib_End();
}


void TetrisGame::StartMenu()
{
	
	int t_selected_item = 0;
	Fps fps_counter;
	int t_counter;
	int count = 0;
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0){
		fps_counter.Update();
		fps_counter.Wait();
		t_counter = fps_counter.GetCounter();
		DrawGraph(100, 50, m_title_image, FALSE);
		if ((t_counter % 20) == 0)
			count++;
		DrawRotaGraph(180, 500, 0.75, PI*(count++ % 16) / 16, m_item_image[kClover], TRUE); //draw picture and roll it
		KeyboardUpdate();

		if (KeyboardGet(KEY_INPUT_DOWN) == 1){ // action once  down is pressed

			t_selected_item = (t_selected_item + 1) % kSTartMenuLen; // select the lower item
		}

		if (KeyboardGet(KEY_INPUT_UP) == 1){ // action once up is pressed

			t_selected_item = (t_selected_item + (kSTartMenuLen - 1)) % kSTartMenuLen; // select the upper item
		}

		if (KeyboardGet(KEY_INPUT_DOWN) == 1 || KeyboardGet(KEY_INPUT_UP) == 1){ //once up or down is pressed
			for (int i = 0; i<kSTartMenuLen; i++){
				if (i == t_selected_item){
					g_start_menu[i].x = 480;					//selected item
				}
				else {
					g_start_menu[i].x = 500;
				}
			}
		}

		// draw menu
		for (int i = 0; i<kSTartMenuLen; i++){
			DrawFormatString(g_start_menu[i].x, g_start_menu[i].y, GetColor(0, 0, 0), g_start_menu[i].name);
		}
		if (KeyboardGet(KEY_INPUT_RETURN) == 1){
			switch(t_selected_item){
				case 0:
					SetEventFlag(StartNewTetrisEVENT);//Start new Game
					break;
				case 1:
					SetEventFlag(ShowHighScoreEVENT);//HighScore
					break;
				case 2:
					SetEventFlag(GAMEOVEREVENT);//GameOver
					break;
				default:
					break;
			}
		}
		if (EventInQueue() == true)
			break;
	}
}

//deal with EVENT in queue 
void TetrisGame::DoEvent()
{
	if (EventInQueue()){
		STARTMENU_EVENT_TYPE t_event = m_event_queue.front();
		m_event_queue.pop_front();
		switch (t_event){
		case ShowStartMenuEVENT://Show start menu
			StartMenu();
			break;
		case StartNewTetrisEVENT:
			StartNewTetrisGame();//Start new Game
			break; 
		case ShowHighScoreEVENT:
			ShowHighScore();//HighScore
			break;
		/*
		case ShowHelpEVENT:
			ShowHelp();//Help
			break;
		*/
		case GAMEOVEREVENT:
			m_game_over_flag = true;//GameOver
			break;
		default:
			break;
		}
	
	}
}



void TetrisGame::ShowHighScore()
{
	int count = 0,i;
	HighScore t_high_score;
	Fps fps_counter;
	t_high_score.LoadHighScore();
	vector<int> t_high_score_vector;
	int t_y_postion[kHighScoreNum];
	for (unsigned int j = 0; j < t_high_score.m_high_score.size(); j++)
	{
		t_y_postion[j] = 100 + j * 30;
	}
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0){
		fps_counter.Update();
		fps_counter.Wait();
		DrawGraph(0, 0, m_item_image[kHighscore], TRUE);
		
		i = 0;
		for (auto it = t_high_score.m_high_score.begin(); it != t_high_score.m_high_score.end(); it++){
				i++;
				t_high_score_vector.push_back(*it);	
				if (fps_counter.GetCounter() % 4 == 0)count++;
				DrawRotaGraph(200, t_y_postion[i], 0.1, PI*(count % 16) / 16, m_item_image[kClover], TRUE);
				DrawFormatString(230, t_y_postion[i], GetColor(0, 0, 0), "%d:%d",i,(*it));
		}
		DrawFormatString(550, 500, GetColor(0, 0, 0), "ESCキで戻る");
		KeyboardUpdate();
		if (KeyboardGet(KEY_INPUT_ESCAPE) == 1){
			SetEventFlag(ShowStartMenuEVENT);
			break;
		}
		
		
	}
	t_high_score.End();
}



bool TetrisGame::EventInQueue()
{
	if (m_event_queue.empty() == true)
		return false; 
	else 
		return true; 
}