#ifndef YADRO_COMPUTINGCLASS_H
#define YADRO_COMPUTINGCLASS_H
#include <iostream>
#include <fstream>
#include "ComputingDesk.h"
#include <vector>
#include <sstream>
#include <regex>


struct ClubEvent {
	size_t id = 0;
	std::string name;
	size_t deskNumber = 0;
	timehm eventTime = {0,0};
};

struct Client {
	std::string name;
};

class ComputingClass
{
private:
	size_t timeCost = 0; // cost by hour
	size_t tableNumber = 0; // number of desks
	timehm startTime = {0,0}; // start club working time
	timehm endTime = {0,0}; // end club working time
	std::ifstream fin;
	std::vector<ComputingDesk> desks;
	std::vector<Client> queue;
public:
	ComputingClass(std::string);
	ComputingClass();
	~ComputingClass();
	void StartModeling();
	void CheckEvent(ClubEvent&);
	void Event1(ClubEvent&);
	void Event2(ClubEvent&);
	void Event3(ClubEvent&);
	void Event4(ClubEvent&);
	void Event11(ClubEvent&);
	void Event12(ClubEvent&, size_t);//size_t open desk
	void EndDay();
	void PrintDesks();
	void DesksSort();
	void QueueSort();
};





#endif