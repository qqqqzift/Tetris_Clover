#ifndef LOGMGR_H_
#define LOGMGR_H_
#include <iostream>
#include <fstream>
#include <windows.h> 
#include <string>
#include <sstream>

using namespace std;

#define LOGPATH  "log2014.txt"
#define TIMESTR 64
class LogWriter
{
public:
static LogWriter* me;
void LogMgrInitialize();

string TimeGet();

bool WriteLog(string message);

void LogMgrFinallize();


};


//get today and save to string



#endif
