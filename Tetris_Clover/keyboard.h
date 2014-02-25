#ifndef KEYBOARD_H_
#define KEYBOARD_H_


/*
	Keyboard_Update:save input key status
	Keyboard_Get:return the result if kcode key is pressed

*/
extern int key[256];
int KeyboardUpdate();

int KeyboardGet(int kcode);

void KeyboardReSet(int kcode);


#endif