#include "chip8.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

// Fontset: angka 0-F dalam bentuk sprite 4x5 pixel
// Setiap karakter butuh 5 byte, tiap byte 1 baris (4 bit kiri)
const uint8_t Chip8::fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() {
    loadFontset();
}

void Chip8::loadFontset() {
    for (int i = 0; i < FONTSET_SIZE; i++) {
        memory[i] = fontset[i];
    }
}

void Chip8::loadROM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Gagal buka ROM: " << filename << "\n";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // ROM dimuat mulai dari alamat 0x200 (512)
    // Alamat 0x000-0x1FF dipakai untuk fontset dan interpreter
    file.read(reinterpret_cast<char*>(&memory[START_ADDRESS]), size);

    std::cout << "Loaded ROM: " << filename << " (" << size << " bytes)\n";
}

void Chip8::cycle() {
    if (pc + 1 >= MEMORY_SIZE) {
        static bool warned = false;
        if (!warned) {
            std::cerr << "PC keluar batas: 0x" << std::hex << pc << std::dec << "\n";
            warned = true;
        }
        return;
    }

    opcode = (memory[pc] << 8) | memory[pc + 1];
    
    //std::cout << "PC: 0x" << std::hex << pc
    //      << "  Opcode: 0x" << opcode
    //      << std::dec
    //      << "  VA: " << (int)V[0xA]
    //      << "  VB: " << (int)V[0xB]
    //      << std::endl;

    pc += 2;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0:
                    video.fill(0);
                    break;
                case 0x00EE:  // RET: return from subroutine
                    sp--;
                    pc = stack[sp];
                    break;
            }
            break;
        case 0x1000:
            pc = opcode & 0x0FFF;
            break;
        case 0x2000:
            stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;
            break;
        case 0x3000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t NN = opcode & 0x00FF;
            //if (V[X] == NN) pc += 2;
            bool skip = (V[X] == NN);
        if (skip) pc += 2;
        // std::cout << "3XNN: V" << (int)X << "=" << (int)V[X]
        //         << " NN=" << (int)NN
        //         << " -> " << (skip ? "SKIP" : "no skip") << "\n";
            break;
        }
        case 0x4000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t NN = opcode & 0x00FF;
            //if (V[X] != NN) pc += 2;
            bool skip = (V[X] != NN);
            if (skip) pc += 2;
            // std::cout << "4XNN: V" << (int)X << "=" << (int)V[X]
            //         << " NN=" << (int)NN
            //         << " -> " << (skip ? "SKIP" : "no skip") << "\n";
            break;
        }
        case 0x5000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t Y = (opcode & 0x00F0) >> 4;
            //if (V[X] == V[Y]) pc += 2;
            bool skip = (V[X] == V[Y]);
            if (skip) pc += 2;
            // std::cout << "5XY0: V" << (int)X << "=" << (int)V[X]
            //         << " V" << (int)Y << "=" << (int)V[Y]
            //         << " -> " << (skip ? "SKIP" : "no skip") << "\n";
            break;
        }
        case 0x6000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t NN = opcode & 0x00FF;
            V[X] = NN;
            break;
        }
        case 0x7000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t NN = opcode & 0x00FF;
            V[X] += NN;
            break;
        }
        case 0x8000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t Y = (opcode & 0x00F0) >> 4;
            uint8_t N = opcode & 0x000F;
            // uint8_t beforeX = V[X];
            // uint8_t beforeY = V[Y];
            switch (N) {
                case 0x0:
                    V[X] = V[Y];
                    break;
                case 0x1:
                    V[X] |= V[Y];
                    break;
                case 0x2:
                    V[X] &= V[Y];
                    break;
                case 0x3:
                    V[X] ^= V[Y];
                    break;
                case 0x4: {
                    uint16_t sum = V[X] + V[Y];
                    V[0xF] = (sum > 255) ? 1 : 0;
                    V[X] = sum & 0xFF;
                    break; 
                }
                case 0x5: 
                    V[0xF] = (V[X] >= V[Y]) ? 1 : 0;
                    V[X] -= V[Y];
                    break;
                case 0x6:
                    V[0xF] = V[X] & 1;
                    V[X] >>= 1;
                    break;
                case 0x7:
                    V[0xF] = (V[Y] >= V[X]) ? 1 : 0;
                    V[X] = V[Y] - V[X];
                    break;
                case 0xE:
                    V[0xF] = (V[X] >> 7) & 1;
                    V[X] <<= 1;
                    break;
            }
            // std::cout << "8XY" << std::hex << (int)N
            //   << "  V" << (int)X << ": " << std::dec << (int)beforeX
            //   << " -> " << (int)V[X]
            //   << "  (V" << (int)Y << "=" << (int)beforeY << ")"
            //   << "  VF=" << (int)V[0xF]
            //   << "\n";

            break;
        }
        case 0x9000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t Y = (opcode & 0x00F0) >> 4;
            bool skip = (V[X] != V[Y]);
            if (skip) pc += 2;
            // std::cout << "9XY0: V" << (int)X << "=" << (int)V[X]
            //         << " V" << (int)Y << "=" << (int)V[Y]
            //         << " -> " << (skip ? "SKIP" : "no skip") << "\n";
            //if (V[X] != V[Y]) pc += 2;
            break;
        }
        case 0xA000:
            I = opcode & 0x0FFF;
            break;
        case 0xC000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t NN = opcode & 0x00FF;
            V[X] = (rand() % 256) & NN;
            break;
        }
        case 0xD000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t Y = (opcode & 0x00F0) >> 4;
            uint8_t N = opcode & 0x000F;

            uint8_t x = V[X] % VIDEO_WIDTH;
            uint8_t y = V[Y] % VIDEO_HEIGHT;

            V[0xF] = 0;

            for (int row = 0; row < N; row++) {
                uint8_t spriteByte = memory[I + row];
                for (int col = 0; col < 8; col++) {
                    if ((spriteByte & (0x80 >> col)) != 0) {
                        int px = (x + col) % VIDEO_WIDTH;
                        int py = (y + row) % VIDEO_HEIGHT;
                        int idx = py * VIDEO_WIDTH + px;

                        if (video[idx] == 1) {
                            V[0xF] = 1;
                        }
                        video[idx] ^= 1;
                    }
                }
            }
            break;
        }
        case 0xE000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                case 0x9E:  
                    if (keypad[V[X]] != 0) pc += 2;
                    break;
                case 0xA1:  
                    if (keypad[V[X]] == 0) pc += 2;
                    break;
            }
            break;
        }
        case 0xF000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                case 0x07:
                    V[X] = delay_timer;
                    break;
                case 0x0A: {
                    bool pressed = false;
                    for (int i = 0; i < 16; i++) {
                        if (keypad[i] != 0) {
                            V[X] = i;
                            pressed = true;
                            break;
                        }
                    }
                    if (!pressed) pc -= 2;
                    break;
                }
                case 0x15:  // FX15: delay_timer = VX
                    delay_timer = V[X];
                    break;
                case 0x18: 
                    sound_timer = V[X];
                    break;
                case 0x1E:
                    I += V[X];
                    break;
                case 0x29: 
                    I = V[X] * 5;
                    break;
                case 0x33: {
                    uint8_t value = V[X];
                    memory[I] = value / 100;
                    memory[I + 1] = (value / 10) % 10;
                    memory[I + 2] = value % 10;
                    break;
                }
                case 0x55:
                    for (int i = 0; i <= X; i++) {
                        memory[I + i] = V[i];
                    }
                    break;
                case 0x65:  // FX65: load V0-VX
                for (int i = 0; i <= X; i++) {
                    V[i] = memory[I + i];
                }
                break;
            }
            break;
        }
    }
}