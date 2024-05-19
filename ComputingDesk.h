#ifndef YADRO_COMPUTINGDESK_H
#define YADRO_COMPUTINGDESK_H
#include <string>


struct timehm {
	int hour = 0;
	int minute = 0;
};

class ComputingDesk//sometime u can see table in comments but i use desk becouse table often == bd.table
{
private:
	size_t workingTimePayment = 0;
	timehm workingTime = {0,0};
	timehm startClientTime;//time client sit
	std::string guest;//name of client on desk
	size_t number;// number of table(because we can sort them)
public:
	ComputingDesk();
	ComputingDesk(size_t);
	bool IsOpen();
	std::string GetGuestName();
	void SatGuest(std::string name, timehm time);
	void UpGuest(timehm time);
	size_t GetMoney();
	timehm GetWorkingTime();
	size_t GetNumber();
};


#endif