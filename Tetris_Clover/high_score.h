#ifndef HIGH_SCORE
#define HIGH_SCORE
#include <list>
using namespace std;

class HighScore{


public:
	list<int> m_high_score;
	bool LoadHighScore();
	void UpdateHighScore(int);
	void End(){ m_high_score.clear(); }



};







#endif