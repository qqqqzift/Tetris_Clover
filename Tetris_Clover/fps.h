#ifndef FPS_H_
#define FPS_H_


class Fps{
private:
	int mStartTime;         //����J�n����
	int mCount;             //�J�E���^
	float mFps;             //fps
	static const int FPS = 60;	//�ݒ肵��FPS
public:
	Fps():mStartTime(0),mCount(0),mFps(0){}
	void Update();
	void Draw();
	void Wait();

	int GetCounter(){return mCount;}
};



#endif