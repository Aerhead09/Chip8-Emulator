# CHIP-8 Emulator

Emulator CHIP-8 yang dibangun dari nol dengan C++17 dan SDL3.

## Status

Dalam pengembangan. Saat ini sudah bisa:
- Load ROM
- Fetch-decode-execute loop dasar
- Beberapa opcode dasar (00E0, 1NNN, 6XNN, 7XNN, ANNN)

Belum bisa main game — masih perlu implementasi semua opcode dan input keyboard.

## Cara Build

### Prasyarat

1. **g++** (MinGW-w64 untuk Windows)
2. **SDL3** — download dari [github.com/libsdl-org/SDL/releases](https://github.com/libsdl-org/SDL/releases)
   - Pilih `SDL3-devel-x.x.x-mingw.tar.gz`
   - Extract
   - Copy folder `include/` ke `libs/SDL3/include/`
   - Copy file `.a` dari `lib/` ke `libs/SDL3/lib/`
   - Copy `SDL3.dll` dari `bin/` ke `bin/SDL3.dll`

### Build

```
build.bat
```

Binary hasil build ada di `bin/chip8.exe`.

## Cara Pakai

```
bin\chip8.exe roms\your_rom.ch8
```

## Struktur Kode

- `src/chip8.h` — deklarasi class Chip8 dengan semua state (memory, register, stack, timer, video, keypad)
- `src/chip8.cpp` — implementasi: load fontset, load ROM, fetch-decode-execute
- `src/main.cpp` — setup SDL3, game loop, rendering framebuffer

## Spesifikasi CHIP-8

- 4 KB memory (0x1000 byte)
- 16 register 8-bit (V0-VF)
- Register I 16-bit (alamat memory)
- Program counter 16-bit (mulai dari 0x200)
- Stack 16 level
- Delay & sound timer (60 Hz)
- Layar 64x32 pixel monokrom
- Keypad 16 tombol (0-F)

## Referensi

- [Wikipedia: CHIP-8](https://en.wikipedia.org/wiki/CHIP-8)
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Tobias V. Langhoff: Guide to making a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)

## Lisensi

MIT