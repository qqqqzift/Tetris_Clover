#include <math.h>
#include "fps.h"
#include "DxLib.h"

void Fps::Update()
{
	    if( mCount == 0 ){ //1�t���[���ڂȂ玞�����L��
			mStartTime = GetNowCount();
		}
		if( mCount == FPS ){ //60�t���[���ڂȂ畽�ς��v�Z����
			int t = GetNowCount();
			mFps = 1000.f/((t-mStartTime)/(float)FPS);
			mCount = 0;
			mStartTime = t;
		}
		mCount++;
}



void Fps::Wait(){
	int tookTime = GetNowCount() - mStartTime;	//������������
	int waitTime = mCount*1000/FPS - tookTime;	//�҂ׂ�����
	if( waitTime > 0 ){
		Sleep(waitTime);	//�ҋ@
	}
}


void Fps::Draw()
{
	DrawFormatString(0, 0, GetColor(0,0,0), "%.1f", mFps);
}
