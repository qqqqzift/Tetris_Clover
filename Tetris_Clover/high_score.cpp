#include <stdlib.h>
#include <iostream>  
#include <fstream>  
#include "high_score.h"


bool HighScore::LoadHighScore()
{
	char buffer[256];
	int i = 0;
	ifstream in("highscore.txt");
	if (!in.is_open())
		return false;
	m_high_score.clear();
	while ((!in.eof())&& i++ <10)
	{
		in.getline(buffer, 100);
		m_high_score.push_front(atoi(buffer));
	}
	m_high_score.sort();
	return true;
}
void  HighScore::UpdateHighScore(int high)
{
	m_high_score.sort();
	if (high > m_high_score.front())
		m_high_score.front() = high;


	
	remove("highscore.txt");
	ofstream out("highscore.txt");
	char scorebuff[16];
	if (out.is_open())
	{
		for (auto it = m_high_score.begin(); it != m_high_score.end(); it++)
		{
			itoa((*it), scorebuff, 16);
			out << scorebuff;
		}
		
		
		out.close();
	}
}