#include "my_log.h"

using namespace std;
LogWriter *LogWriter::me;
void LogWriter::LogMgrInitialize()
{
	me = this;

}
bool LogWriter::WriteLog(string message)
{
	ofstream out(LOGPATH,ios::app);
	string timestr;
	timestr = TimeGet();
	if(out.is_open())
	{
		out<<timestr<<": "<<message<<endl;
		return true;
	}
	else
		return false;
	
}



void LogWriter::LogMgrFinallize(){


}

string LogWriter::TimeGet()
{
	SYSTEMTIME start; //windows.h中  
    GetLocalTime(&start);//time.h的tm结构体一样的效箒E 
	ostringstream ostr;
	string timestr;
	ostr << start.wYear << "/" <<start.wMonth
		<<"/"<<start.wDay<<" "<<start.wHour<<":"<<start.wMinute
		<<":"<<start.wMilliseconds;
	timestr = ostr.str();
	return timestr;

}