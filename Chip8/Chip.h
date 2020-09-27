#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Chip
{
public:
	void countDown();
	Chip();
	~Chip();
	void cycle();
	void loadROM(std::string path);

private:
	void memoryDump(int maxOffset);
	void memoryLoad(int maxOffset);

	void mathOperation(unsigned short opcode, int x, int y);


	unsigned char delayTimer = 60;
	unsigned char soundTimer = 60;
	unsigned int opcode;

	unsigned short programCounter = 0x200;
	unsigned char V[16];
	unsigned short I = 0;
	unsigned char memory[4096];
	std::vector<unsigned short> stack;
};

