#include <thread>
#include "tetris_game.h"

#include "DxLib.h"




int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	TetrisGame t_teris_game;

	t_teris_game.Init();

	t_teris_game.Main();
	t_teris_game.EndGame();


	return 0;
}