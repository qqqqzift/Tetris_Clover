#ifndef TETRIS_GAME_H_
#define TETRIS_GAME_H_
#include <deque>
#include <algorithm>
#include "tetris_types.h"

using namespace tetris_menu;
using std::deque;

class TetrisGame{
private:
	deque<STARTMENU_EVENT_TYPE> m_event_queue;
	bool m_game_over_flag;
	int m_title_image;
	int m_item_image[kTitleItem];
public:
	static TetrisGame *me;
	
	void Init();
	void Main();
	void SetEventFlag(STARTMENU_EVENT_TYPE);
	bool EventInQueue();	//
	void DoEvent(); 

	void StartMenu();
	void StartNewTetrisGame();															//tetris page
	void ShowHighScore();																//high score page

	void EndGame();
	int GetItemImage(int i){ if (i < kTitleItem)return m_item_image[i]; else return 0; }
};


#endif