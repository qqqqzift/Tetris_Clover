#include <math.h>
#include "fps.h"
#include "DxLib.h"

void Fps::Update()
{
	    if( mCount == 0 ){ //1フレーム目なら時刻を記憶
			mStartTime = GetNowCount();
		}
		if( mCount == FPS ){ //60フレーム目なら平均を計算する
			int t = GetNowCount();
			mFps = 1000.f/((t-mStartTime)/(float)FPS);
			mCount = 0;
			mStartTime = t;
		}
		mCount++;
}



void Fps::Wait(){
	int tookTime = GetNowCount() - mStartTime;	//かかった時間
	int waitTime = mCount*1000/FPS - tookTime;	//待つべき時間
	if( waitTime > 0 ){
		Sleep(waitTime);	//待機
	}
}


void Fps::Draw()
{
	DrawFormatString(0, 0, GetColor(0,0,0), "%.1f", mFps);
}
