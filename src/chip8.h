#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <array>
#include <string>

class Chip8 {
public:
    // Spesifikasi CHIP-8:
    // - 4KB RAM (0x1000 byte)
    // - 16 register 8-bit (V0-VF)
    // - Register I 16-bit (untuk alamat memori)
    // - PC (program counter) 16-bit
    // - Stack 16 level untuk subroutine
    // - SP (stack pointer) 8-bit
    // - Delay timer 8-bit (60 Hz)
    // - Sound timer 8-bit (60 Hz)
    // - Layar 64x32 pixel monokrom
    // - Keypad 16 tombol (0-F)

    static constexpr int MEMORY_SIZE = 4096;
    static constexpr int NUM_REGISTERS = 16;
    static constexpr int STACK_SIZE = 16;
    static constexpr int VIDEO_WIDTH = 64;
    static constexpr int VIDEO_HEIGHT = 32;
    static constexpr int NUM_KEYS = 16;
    static constexpr int FONTSET_SIZE = 80;
    static constexpr int START_ADDRESS = 0x200;

    std::array<uint8_t, MEMORY_SIZE> memory{};
    std::array<uint8_t, NUM_REGISTERS> V{};
    uint16_t I = 0;
    uint16_t pc = START_ADDRESS;
    std::array<uint16_t, STACK_SIZE> stack{};
    uint8_t sp = 0;
    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;
    std::array<uint8_t, VIDEO_WIDTH * VIDEO_HEIGHT> video{};
    std::array<uint8_t, NUM_KEYS> keypad{};
    uint16_t opcode = 0;

    Chip8();

    void loadROM(const std::string& filename);
    void cycle();

private:
    static const uint8_t fontset[FONTSET_SIZE];
    void loadFontset();
};

#endif