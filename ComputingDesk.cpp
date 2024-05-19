#include "ComputingDesk.h"


timehm timeSum(timehm a, timehm b){
	timehm rez;
	rez.minute = (a.minute + b.minute) % 60;
	rez.hour = a.hour + b.hour + (a.minute + b.minute) / 60;
	return rez;
}

timehm timeSub(timehm a, timehm b) {
	timehm rez;
	rez.hour = a.hour - b.hour;
	rez.minute = (a.minute - b.minute);
	if (rez.minute < 0) {
		rez.hour -= 1;
		rez.minute += 60;
	}
	if (rez.hour < 0)
		return(timeSum(b, a));
	return rez;
}

size_t saturationTime(timehm a) {
	size_t rez = a.hour;
	if (a.minute > 0)
		rez += 1;
	return rez;
}

ComputingDesk::ComputingDesk() {

}

ComputingDesk::ComputingDesk(size_t num) {
	number = num;
}

bool ComputingDesk::IsOpen() {
	return guest.empty();
}

std::string ComputingDesk::GetGuestName() {
	return guest;
}

size_t ComputingDesk::GetNumber() {
	return number;
}

void ComputingDesk::SatGuest(std::string name, timehm time) {
	guest = name;
	startClientTime = time;
}

void ComputingDesk::UpGuest(timehm time) {

	workingTime = timeSum(timeSub(time, startClientTime), workingTime);
	workingTimePayment += saturationTime(timeSub(time, startClientTime));
	guest.clear();
}
size_t ComputingDesk::GetMoney() {
	return workingTimePayment;
}

timehm ComputingDesk::GetWorkingTime() {
	return workingTime;
}