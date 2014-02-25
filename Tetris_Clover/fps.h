#ifndef FPS_H_
#define FPS_H_


class Fps{
private:
	int mStartTime;         //測定開始時刻
	int mCount;             //カウンタ
	float mFps;             //fps
	static const int FPS = 60;	//設定したFPS
public:
	Fps():mStartTime(0),mCount(0),mFps(0){}
	void Update();
	void Draw();
	void Wait();

	int GetCounter(){return mCount;}
};



#endif