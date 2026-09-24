#include "chip8.h"
#include <fstream>
#include <iostream>

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
    std::cout << "PC: 0x" << std::hex << pc
          << "  Opcode: 0x" << opcode
          << std::dec
          << "  VA: " << (int)V[0xA]
          << "  VB: " << (int)V[0xB]
          << std::endl;
    if (pc + 1 >= MEMORY_SIZE) {
        std::cerr << "PC keluar batas: 0x" << std::hex << pc << std::dec << "\n";
        return;
    }

    opcode = (memory[pc] << 8) | memory[pc + 1];
    
    pc += 2;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0:
                video.fill(0);
                break;
            }
            break;
        case 0x1000:
            pc = opcode & 0x0FFF;
            break;
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
        case 0xA000:
            I = opcode & 0x0FFF;
            break;
    }
}