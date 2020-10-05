#ifndef CHIP
#define CHIP
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Chip
{
public:
	Chip();
	~Chip();
	void cycle();
	void loadROM(std::string path);
	std::vector<std::vector<bool>> getScreen();

private:
	void draw(int x, int y, int n);
	void clearScreen();

	void memoryDump(int maxOffset);
	void memoryLoad(int maxOffset);

	void mathOperation(unsigned short opcode, int x, int y);
	void FOperation(unsigned short opcode, int x, int y);
	void EOperation(unsigned short opcode, int x, int y);


	void checkForInput();

	unsigned char delayTimer = 0;
	unsigned char soundTimer = 0;
	const unsigned short fontLocation = 0;
	unsigned short programCounter = 0x200;
	unsigned char V[16];
	unsigned short I = 0;
	unsigned char memory[4096];
	std::vector<std::vector<bool>> screen;
	std::vector<unsigned short> stack;
	std::vector<unsigned char> keyPressed;

};

#endif