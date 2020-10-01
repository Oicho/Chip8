#include "Chip.h"
#include "Graphics.h"
#include <random>


unsigned char chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

Chip::Chip() : screen(64, std::vector<bool>(32)) {
    for (size_t i = 0; i < 80; i++) {
        this->memory[fontLocation + i] = chip8_fontset[i];
    }
    //this->screen[0][0] = true;
    //this->screen[31][15] = true;
    //this->screen[63][31] = true;


}   

Chip::~Chip() {

}

std::vector<std::vector<bool>> Chip::getScreen() {
    return this->screen;
}

void Chip::clearScreen() {
    for (size_t i = 0; i < this->screen.size(); i++) {
        for (size_t j = 0; j < this->screen[1].size(); j++) {
            this->screen[i][j] = false;
        }
    }
}

void Chip::cycle() {
    unsigned short opcode = (this->memory[this->programCounter] << 8) + this->memory[this->programCounter + 1];
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;
    // TODO MOve this to construct
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distChar(0, 255);

    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00E0) {
                this->clearScreen();
            } else if (opcode == 0x00EE) {
                if (!this->stack.empty()) {
                    this->programCounter = this->stack[this->stack.size() - 1];
                    this->stack.pop_back();
                }
                else {
                    std::cerr << "[ERROR] Return from call subroutine but " <<std::endl;
                }
            }
            break;
        case 0x1000:
            this->programCounter = opcode & 0x0FFF;
            this->programCounter -= 2;
            break;
        case 0x2000:
            this->stack.push_back(this->programCounter);
            this->programCounter = opcode & 0x0FFF;
            // Removing because of the default PC + 2 at the end
            this->programCounter -= 2;
            break;
        case 0x3000:
            if (this->V[x] == (opcode & 0x00FF)) {
                this->programCounter += 2;
            }
            break;
        case 0x4000:
            if (this->V[x] != (opcode & 0x00FF)) {
                this->programCounter += 2;
            }
            break;
        case 0x5000:
            if (this->V[x] == this->V[y]) {
                this->programCounter += 2;
            }
            break;
        case 0x6000:
            this->V[x] = opcode & 0x00FF;
            break;
        case 0x7000:
            this->V[x] = this->V[x] + (opcode & 0x00FF);
            break;
        case 0x8000:
            this->mathOperation(opcode, x, y);
            break;
        case 0x9000:
            if (this->V[x] != this->V[y]) {
                this->programCounter += 2;
            }
            break;
        case 0xA000:
            this->I = opcode & 0x0FFF;
            break;
        case 0xB000:
            this->programCounter = (opcode & 0x0FFF) + this->V[0];
            break;
        case 0xC000:
            this->V[x] = distChar(rng) & (opcode & 0x00FF);
            break;
        case 0xD000:
            this->draw(this->V[x], this->V[y], opcode & 0x000F);
            break;
        case 0xE000:
            // TOFO
            break;
        case 0xF000:
            this->FOperation(opcode, x, y);
            break;
        default:
            std::cerr << "[ERROR] Unknown opcode " << opcode << std::endl;
            break;
    }
    this->programCounter += 2;
}

void Chip::memoryDump(int maxOffset) {
    for (size_t i = 0; i <= maxOffset; i++) {
        this->memory[this->I + i] = this->V[i];
    }
}

void Chip::memoryLoad(int maxOffset) {
    for (size_t i = 0; i <= maxOffset; i++) {
        this->V[i] = this->memory[this->I + i];
    }
}

void Chip::mathOperation(unsigned short opcode, int x, int y) {
    unsigned char vx = this->V[x];

    switch (opcode & 0x000F)
    {
    case 0x0:
        this->V[x] = this->V[y];
        break;
    case 0x1:
        this->V[x] = this->V[x] | this->V[y];
        break;
    case 0x2:
        this->V[x] = this->V[x] & this->V[y];
        break;
    case 0x3:
        this->V[x] = this->V[x] ^ this->V[y];
        break;
    case 0x4:
        this->V[x] += this->V[y];
        if (this->V[x] < this->V[y])
            // carry
            this->V[0xF] = 1;
        else
            this->V[0xF] = 0;
        break;
    case 0x5:
        this->V[x] -= this->V[y];
        if (this->V[x] > vx)
            // borrow
            this->V[0xF] = 0;
        else
            this->V[0xF] = 1;
        break;
    case 0x6:
        this->V[0xF] = this->V[x] & 1;
        this->V[x] = this->V[x] >> 1;
        break;
    case 0x7:
        if (this->V[y] > this->V[x])
            this->V[0xF] = 1;
        else
            this->V[0xF] = 0;

        this->V[x] = this->V[y] - this->V[x];
        break;  
    case 0xE:
        this->V[0xF] = this->V[x] & 0x80;
        this->V[x] = this->V[x] << 1;

        break;
    default:
        break;
    }
}

void Chip::FOperation(unsigned short opcode, int x, int y) {
    switch (opcode & 0x00FF) {
    case 0x07:
        this->V[x] = this->delayTimer;
        break;
    case 0x0A:
        this->V[x] = waitForInput();
        break;
    case 0x15:
        this->delayTimer = this->V[x];
        break;
    case 0x18:
        this->soundTimer = this->V[x];
        break;
    case 0x1E:
        this->I += this->V[x];
        break;
    case 0x29:
        this->I = this->fontLocation + this->V[x] * 5;
        break;
    case 0x33:
        this->memory[this->I + 2] = this->V[x] % 10;
        this->memory[this->I + 1] = (this->V[x] / 10) % 10;
        this->memory[this->I] = this->V[x] / 100;
        break;
    case 0x55:
        this->memoryDump(x);
        break;
    case 0x65:
        this->memoryLoad(x);
        break;
    }
}

void Chip::loadROM(std::string path) {
    char* pmemory = (char*) &memory[0x200];
    std::streampos size;
    std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(pmemory, size);
        file.close();

        std::cout << "the entire file content is in memory";
    }
    else {
        std::cerr << "Unable to open file at " << path << std::endl;
    }
}

void Chip::draw(int x, int y, int n) {
    unsigned char value;
    bool isOverlap = false;
    for (size_t i = 0; i < n && i + y < 32; i++)
    {
        value = this->memory[this->I + i];
        for (size_t j = 0; j < 8 && j + x < 64; j++) {
            isOverlap = isOverlap || (this->screen[j + x][i + y] && (value & 0b10000000));
            this->screen[j + x][i + y] = this->screen[j + x][i + y] ^ (value & 0b10000000);
            value = value << 1;
        }
    }
    if (isOverlap)
        this->V[0xF] = 1;

}