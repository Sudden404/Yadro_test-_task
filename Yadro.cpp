#include <iostream>
#include "ComputingClass.h"


int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "incorrect number of arguments" << std::endl;
		std::cout << *argv << std::endl;
		exit(1);
	}
	ComputingClass a(argv[1]);
	a.StartModeling();
	a.PrintDesks();
}
