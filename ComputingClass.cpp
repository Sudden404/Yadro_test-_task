#define _CRT_SECURE_NO_WARNINGS
#include "ComputingClass.h"


void sToTime(std::string& str, timehm& t){
	char tmp[2];
	tmp[0] = str[0];
	tmp[1] = str[1];
	t.hour = atoi(tmp);
	tmp[0] = str[3];
	tmp[1] = str[4];
	t.minute = atoi(tmp);
}

void PrintTime(timehm time) {
	std::cout << time.hour / 10 << time.hour % 10 << ":" << time.minute / 10 << time.minute % 10 << std::endl;
}

void PrintError(timehm time, const char* message) {
	std::cout << time.hour / 10 << time.hour % 10 << ":" << time.minute / 10 << time.minute % 10;
	std::cout << " 13 " << message << std::endl;
}

void PrintEvent(ClubEvent& ev) {
	std::cout << ev.eventTime.hour / 10 << ev.eventTime.hour % 10 << ":" << ev.eventTime.minute / 10 << ev.eventTime.minute % 10;
	std::cout << " " << ev.id << " " << ev.name;
	if (ev.id == 2 || ev.id == 12)
		std::cout << " " << ev.deskNumber+1;
	std::cout << std::endl;
}

void ComputingClass::PrintDesks(){
	for (int i = 0; i < desks.size(); i++) {
		int j;
		for (j = 0; j < desks.size(); j++)
			if (i == desks[j].GetNumber()-1)
				break;
		std::cout << desks[j].GetNumber() << " " << desks[j].GetMoney() * timeCost << " ";
		timehm time = desks[j].GetWorkingTime();
		std::cout << time.hour / 10 << time.hour % 10 << ":" << time.minute / 10 << time.minute % 10 << std::endl;
	}
}

int value_sort(ComputingDesk& pa, ComputingDesk& pb){
	std::string s1 = pa.GetGuestName();
	std::string s2 = pb.GetGuestName();
	return s1.compare(s2);
}

int value_sort(Client& pa, Client& pb) {
	std::string s1 = pa.name;
	std::string s2 = pb.name;
	return s1.compare(s2);
}

void ComputingClass::DesksSort() {
	for (int i = 0; i < desks.size(); i++) {
		for (int j = i + 1; j < desks.size(); j++) {
			if (value_sort(desks[i], desks[j]) == -1)
				std::swap(desks[i], desks[j]);
		}
	}
}

void ComputingClass::QueueSort() {
	for (int i = 0; i < queue.size(); i++) {
		for (int j = i + 1; j < queue.size(); j++) {
			if (value_sort(queue[i], queue[j]) == -1)
				std::swap(queue[i], queue[j]);
		}
	}
}

void ComputingClass::EndDay() {
	DesksSort();
	QueueSort();
	int i, j;
	for (i = desks.size()-1, j = queue.size()-1; i > 0 && j > 0; i, j) {
		if (desks[i].IsOpen()) {
			i--;
			continue;
		}
		if (desks[i].GetGuestName().compare(queue[j].name) == 1) {//name on desk client > name queue client
			ClubEvent ev{11, queue[j].name, 0, endTime};
			Event11(ev);
			j--;
		}
		else {
			ClubEvent ev{ 11, desks[i].GetGuestName(), (size_t)i + 1, endTime };
			Event11(ev);
			i--;
		}
	}
	
	if (j <= 0) {
		while (i >= 0) {
			if (desks[i].IsOpen()) {
				i--;
				continue;
			}
			ClubEvent ev{ 11, desks[i].GetGuestName(), (size_t)i + 1, endTime };
			Event11(ev);
			i--;
		}
	}else if (i <= 0)
		while (j >= 0) {
			ClubEvent ev{ 11, queue[j].name, 0, endTime };
			Event11(ev);
			queue.erase(queue.begin() + j--);
		}
}

void ComputingClass::Event11(ClubEvent& ev){
	PrintEvent(ev);
	bool onDesk = false;
	size_t num = 0;
	{//»щем клиента
		for (int i = 0; i < desks.size(); i++) {//Find client in system
			if (desks[i].GetGuestName() == ev.name) {
				onDesk = true;
				num = i + 1;
				break;
			}
		}
		if (!onDesk)
			for (int i = 0; i < queue.size(); i++) {//Find user in queue
				if (queue[i].name == ev.name) {
					num = i + 1;
					break;
				}
			}
	}
	{
		if (onDesk) {
			desks[num - 1].UpGuest(ev.eventTime);
		}
		else {
			queue.erase(num - 1 + queue.begin());
		}
	}
}

void ComputingClass::Event12(ClubEvent& ev, size_t num) {
	ClubEvent ev12 = ev;
	ev12.id = 12;
	ev12.deskNumber = num;
	if (!queue.empty()) {
		desks[num].SatGuest(queue[0].name, ev.eventTime);
		ev12.name = queue[0].name;
		queue.erase(queue.begin());
		PrintEvent(ev12);
	}
}

void ComputingClass::Event1(ClubEvent& ev) {
	PrintEvent(ev);
	{//Club is open
		if (ev.eventTime.hour < startTime.hour) {
			PrintError(ev.eventTime, "NotOpenYet");
			return;
		}
		else if (ev.eventTime.hour == startTime.hour && ev.eventTime.minute <= startTime.minute) {
			PrintError(ev.eventTime, "NotOpenYet");
			return;
		}
		
		if (ev.eventTime.hour > endTime.hour) {
			PrintError(ev.eventTime, "NotOpenYet");
			return;
		}
		else if (ev.eventTime.hour == endTime.hour && ev.eventTime.minute > endTime.minute) {
			PrintError(ev.eventTime, "NotOpenYet");
			return;
		}
	}
	{//Find client in club
		for (auto i : desks) {//Find client on desk
			if (i.GetGuestName() == ev.name) {
				PrintError(ev.eventTime, "YouShallNotPass");
				return;
			}
		}
		for (auto i : queue) {//Find client in queue
			if (i.name == ev.name) {
				PrintError(ev.eventTime, "YouShallNotPass");
				return;
			}
		}
	}
	queue.push_back(Client{ ev.name});//Add client in queue
}

void ComputingClass::Event2(ClubEvent& ev){
	PrintEvent(ev);
	bool onDesk = false;
	size_t num = 0;
	{//Find client in club
		for (int i = 0; i < desks.size(); i++) {//Find client on desk
				if (desks[i].GetGuestName() == ev.name) {
					onDesk = true;
					num = i + 1;
					break;
				}
			}
		if (!onDesk)
			for (int i = 0; i < queue.size(); i++) {// Find client in queue
				if (queue[i].name == ev.name) {
					num = i + 1;
					break;
				}
				}
		if (num == 0) {
			PrintError(ev.eventTime, "ClientUnknown");
			return;
		}
	}
	{
		if (!desks[ev.deskNumber].IsOpen()) {//Desk close
			PrintError(ev.eventTime, "PlaceIsBusy");
			return;
		}
		else {//desk open
			if (!onDesk) {//Client in queue
				desks[ev.deskNumber].SatGuest(ev.name, ev.eventTime);//Sits on desk
				queue.erase(num - 1 + queue.begin());
			}
			else {//Client was on desk, and want new desk
				desks[num - 1].UpGuest(ev.eventTime);//Client up desk
				desks[ev.deskNumber].SatGuest(ev.name, ev.eventTime);//Sits on desk
				Event12(ev, num - 1);
			}
		}
	}
}
void ComputingClass::Event3(ClubEvent& ev){
	PrintEvent(ev);
	{//Find any open desk
		for (auto i : desks) {
			if (i.IsOpen()) {
				PrintError(ev.eventTime, "ICanWaitNoLonger!");
				break;
			}
		}
	}
	{
		if (queue.size() > desks.size())
			Event11(ev);
	}
}

void ComputingClass::Event4(ClubEvent& ev){
	PrintEvent(ev);
	bool onDesk = false;
	size_t num = 0;
	{//Find client in club
		for (int i = 0; i < desks.size(); i++) {//Find client on desk
			if (desks[i].GetGuestName() == ev.name) {
				onDesk = true;

				num = i + 1;
				break;
			}
		}
		if (!onDesk)
			for (int i = 0; i < queue.size(); i++) {//Find client in queue
				if (queue[i].name == ev.name) {
					num = i + 1;
					break;
				}
			}
		if (num == 0) {
			PrintError(ev.eventTime, "ClientUnknown");
			return;
		}
	}
	{
		if (onDesk) {//if client sat on desk
			desks[num - 1].UpGuest(ev.eventTime);//client up desk
			Event12(ev, num - 1);
		}
		else {//client was in queue and left
			queue.erase(num - 1 + queue.begin());
		}
	}
}

void ComputingClass::CheckEvent(ClubEvent& ev) {
	switch (ev.id) {
	case 1: Event1(ev); break;
	case 2: Event2(ev); break;
	case 3: Event3(ev); break;
	case 4: Event4(ev); break;
	default: std::cout << "CheckEventError"; exit(0);
	}
}
void checkFormatFile(std::ifstream& f) { // check format(exit if find the error)
	std::string str;
	std::regex r1("^[1-9]|([1-9][0-9]+)$");
	std::regex r2("^([01]?[0-9]|2[0-3]):[0-5][0-9] ([01]?[0-9]|2[0-3]):[0-5][0-9]$");
	std::regex r3("^([01]?[0-9]|2[0-3]):[0-5][0-9] (([134] [a-z0-9_-]+)|([2] [a-z0-9_-]+ [1-9]|([1-9][0-9]+)))$");
	std::regex r4(" ([1-9]|([1-9][0-9]+))$");
	getline(f, str);
	if (!std::regex_match(str, r1)){//check 1st string format
		std::cout << str;
		exit(0);
	}
	size_t desk = std::stoi(str);
	if (desk > 1000000) {//check on number of tables 
		std::cout << str;
		exit(0);
	}
	getline(f, str);
	if (!std::regex_match(str, r2)) {//check 2nd string format
		std::cout << str;
		exit(0);
	}
	getline(f, str);
	if (!std::regex_match(str, r1)) {//check 3rd string format
		std::cout << str;
		exit(0);
	}
	if (std::stoi(str) > 1000000) {//check on price
		std::cout << str;
		exit(0);
	}
	while (!f.eof()) {//check else strings format
		getline(f, str);
		if (!std::regex_match(str, r3)) {
			std::cout << str;
			exit(0);
		}
		std::smatch m;
		if (std::regex_search(str, m, r4)) {//check on number of tables border
			std::string t = m[0];
			t.erase(0, 1);
			if (std::stoi(t) > desk) {
				std::cout << str;
				exit(0);
			}
		}
	}

}
void stringCut(const std::string buff, ClubEvent& ev) {
	std::stringstream tmpbuff(buff);
	std::string tmp;

	// read time
	tmpbuff >> tmp;
	sToTime(tmp, ev.eventTime);

	// read id
	tmpbuff >> tmp;
	ev.id = std::stoi(tmp);

	if (ev.id != 13) {
		//read name
		tmpbuff >> ev.name;
	}

	if (ev.id == 2 || ev.id == 12) {
		//read desk number
		tmpbuff >> tmp;
		ev.deskNumber = std::stoi(tmp) - 1;
	}
}

ComputingClass::ComputingClass(std::string path) {

	fin = std::ifstream(path);
	if (!fin.is_open()) {
		std::cout << "Fail in reading file";
		fin.close();
		exit(0);
	}

	checkFormatFile(fin);
	fin.seekg(0);

	char buff[12] = {0};

	fin.getline(buff, 12);
	tableNumber = atoi(buff);
	for (int i = 0; i < tableNumber; i++) {
		desks.push_back(ComputingDesk(i+1));
		
	}

	fin.getline(buff, 12);
	if (sscanf(buff, "%02d:%02d %02d:%02d", &startTime.hour, &startTime.minute, &endTime.hour, &endTime.minute) == EOF) {
		std::cout << buff;
		exit(0);
	}

	fin.getline(buff, 12);
	timeCost = atoi(buff);
}

ComputingClass::ComputingClass() {
	std::cout << "You cant use defolt constructor";// its not safty
	exit(0);
}

ComputingClass::~ComputingClass() {
	if(fin.is_open())
		fin.close();
}

void ComputingClass::StartModeling() {
	std::string buff;
	PrintTime(startTime);
	while (!fin.eof()) {

		std::getline(fin, buff);//read string
		ClubEvent even;
		stringCut(buff, even);//get event from string
		CheckEvent(even);//processing event
	}
	EndDay();
	PrintTime(endTime);

}