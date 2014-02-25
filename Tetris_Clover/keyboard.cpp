#include "DxLib.h"
#include "keyboard.h"

int key[256];

int KeyboardUpdate(){
        char tmp_key[256]; //save recent input key status
		int keycnt = 0;
        GetHitKeyStateAll( tmp_key ); // get the status of all keys
        for( int i=0; i<256; i++ ){ 
                if( tmp_key[i] != 0 ){ // if Number "i" key is pressed
                        key[i]++;     // plus to the flame array
						keycnt;
						
                } else {              // if no pressed 
                        key[i] = 0;   // clear to zero
                }
        }
        return keycnt;
}

int KeyboardGet(int kcode)
{
	return key[kcode];
}

void KeyboardReSet(int kcode)
{
	if (kcode < 256)
		key[kcode] = 0;
}