#include "Chip.h"
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

void Chip::cycle() {
    unsigned short opcode = (this->memory[this->programCounter] << 8) + this->memory[this->programCounter];
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;
    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00E0) {

            } else if (opcode == 0x00EE) {
                if (!this->stack.empty()) {
                    this->programCounter = this->stack[this->stack.size() - 1] + 2;
                    this->stack.pop_back();
                }
                else {
                    std::cerr << "[ERROR] Return from call subroutine but " <<std::endl;
                }
            }
            break;
        case 0x1000:
            this->programCounter = opcode & 0x0FFF;
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
            // TODO real random number
            int randomNumber = 42;
            this->V[x] = randomNumber & (opcode & 0x00FF);
            break;
        case 0xD000:
            break;
        case 0xE000:
            break;
        case 0xF000:
            break;
        default:
            std::cerr << "[ERROR] Unknown opcode " << opcode << std::endl;
            break;
    }
    this->programCounter += 2;
}

void Chip::memoryDump(int maxOffset) {
    for (size_t i; i <= maxOffset; i++) {
        this->memory[this->I + i] = this->V[i];
    }
}

void Chip::memoryLoad(int maxOffset) {
    for (size_t i; i <= maxOffset; i++) {
        this->V[i] = this->memory[this->I + i];
    }
}

void Chip::mathOperation(unsigned short opcode, int x, int y) {
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
        unsigned char vx = this->V[x];
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
        this->V[x] = this->V[y] - this->V[x];
        // TODO V F
        break;
    case 0xE:
        this->V[x] = this->V[x] << 1;\
        break;
    default:
        break;
    }
}