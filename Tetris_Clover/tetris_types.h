#ifndef TERIS_TYPES_H_
#define TERIS_TYPES_H_



//for square class
namespace square_class{
#define SQUARE_LEN 4

	enum DIRECTION{ kGOLEFT, kGORIGHT, kGODOWN };
	enum SQUARETYPE{ TYPE_O, TYPE_I, TYPE_S, TYPE_Z, TYPE_T, TYPE_L, TYPE_J };
	enum BCOLORTYPE{ BLACK, RED, ORANGE, YELLOW, GREEN, SKY, BLUE, PURPLE, WALL_S, WALL_X };

};



//for block_line class
namespace block_line_class{
#define MAXLINEBLOCK 16
#define MAXROWBLOCK 12
#define BACKUPLINE  4
#define BACKUPROW   1
#define kStopMenuLen 5
};



//for window
#define WINWIDTH 800
#define WINHEIGHT 600

//for Tetris
namespace tetris_class{
#define BLOCK_COLOR 8
#define WALL_COLOR 2
#define BLOCK_WIDTH 16

#define TETRIS_NUM 4
	enum TETRIS_TYPE{ TETRIS_UP, TETRIS_DOWN, TETRIS_LEFT, TETRIS_RIGHT };


};




//for TetrisGame
namespace tetris_menu{
	enum STARTMENU_EVENT_TYPE{
		ShowStartMenuEVENT, 
		StartNewTetrisEVENT,
		ShowHelpEVENT,
		ShowHighScoreEVENT,
		GAMEOVEREVENT
	};
#define kSTartMenuLen  4
	typedef struct{
		int x, y;       // position
		char name[128]; // property
	} MenuList_t;





};
extern const bool k_type_o[SQUARE_LEN][SQUARE_LEN];
extern const bool k_type_i[SQUARE_LEN][SQUARE_LEN];
extern const bool k_type_l[SQUARE_LEN][SQUARE_LEN];
extern const bool k_type_j[SQUARE_LEN][SQUARE_LEN];
extern const bool k_type_s[SQUARE_LEN][SQUARE_LEN];
extern const bool k_type_z[SQUARE_LEN][SQUARE_LEN];
extern const bool k_type_t[SQUARE_LEN][SQUARE_LEN];


#define kTitleItem 4
#define kHighScoreNum 10
#define PI 3.141592653


enum PAGE_ITEM_TYPE{kClover,kHighscore,kMenu,kHelp};


#endif