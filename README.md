# CHIP-8 Emulator

Emulator CHIP-8 yang dibangun dari nol dengan C++17 dan SDL3.

## Status

**Working.** Semua opcode utama sudah diimplementasi. Bisa main game klasik
CHIP-8 seperti Pong, Space Invaders, dan Tetris.

## Fitur

- Semua 35 opcode standar CHIP-8
- Load ROM dari file `.ch8`
- Rendering 64×32 pixel monokrom dengan SDL3
- Input keyboard (16 tombol)
- Delay dan sound timer (60 Hz)
- Layar di-scale 10× (640×320)

## Prasyarat

- **g++** (MinGW-w64 untuk Windows, atau GCC untuk Linux/Mac)
- **SDL3** — download dari [github.com/libsdl-org/SDL/releases](https://github.com/libsdl-org/SDL/releases)

### Setup SDL3

1. Download `SDL3-devel-x.x.x-mingw.tar.gz` (pilih yang MinGW, bukan MSVC)
2. Extract ke folder sementara
3. Copy isinya ke folder `libs/SDL3/`:
   ```
   libs/SDL3/
   ├── include/
   │   └── SDL3/
   │       └── (semua file header)
   └── lib/
       ├── libSDL3.dll.a
       └── (file .a lainnya)
   ```
4. Copy `SDL3.dll` dari folder `bin/` SDL3 ke folder `bin/` project ini:
   ```
   bin/
   └── SDL3.dll
   ```

## Cara Build

### Windows

```bat
build.bat
```

Binary hasil build akan ada di `bin/chip8.exe`.

### Linux / Mac

```sh
g++ -std=c++17 -I src -I libs/SDL3/include \
    src/main.cpp src/chip8.cpp \
    -L libs/SDL3/lib \
    -lSDL3 \
    -o bin/chip8
```

## Cara Pakai

```sh
bin/chip8.exe "games/Pong.ch8"
```

**Catatan:** kalau nama file ada tanda kurung atau spasi, bungkus dengan
tanda kutip dua `"..."`.

Contoh:

```bat
bin\chip8.exe "games\Pong (1 player).ch8"
```

## Cara Dapatkan ROM

ROM game (Pong, Tetris, Space Invaders, dll) **tidak di-commit** ke
repository karena berhak cipta dan bukan buatan proyek ini.

Untuk mengunduh:

1. Download dari [github.com/kripod/chip8-roms](https://github.com/kripod/chip8-roms)
2. Extract ke folder `games/` (untuk game) atau `demos/` (untuk demo)
3. Jalankan

ROM test buatan sendiri sudah ada di folder `roms/`:
- `first.ch8` — test opcode dasar
- `draw.ch8` — gambar angka 5 ke layar
- `arith.ch8` — test aritmatika (8XYN)
- `skip.ch8` — test skip conditional (3XNN, 4XNN, 5XY0, 9XY0)

## Kontrol Keyboard

Mapping tombol PC ke keypad CHIP-8:

```
PC Keyboard             CHIP-8 Keypad
1 2 3 4                 1 2 3 C
Q W E R         →       4 5 6 D
A S D F                 7 8 9 E
Z X C V                 A 0 B F
```

### Pong

- **Paddle kiri (kamu):** `Q` (naik), `A` (turun)
- **Paddle kanan (lawan):** `W` (naik), `S` (turun)

## Struktur Kode

- `src/chip8.h` — deklarasi class Chip8 dengan semua state
- `src/chip8.cpp` — implementasi: fontset, load ROM, fetch-decode-execute
- `src/main.cpp` — setup SDL3, game loop, rendering, input keyboard

## Spesifikasi CHIP-8

- 4 KB memory (0x1000 byte)
- 16 register 8-bit (V0-VF, VF dipakai sebagai flag)
- Register I 16-bit (alamat memory)
- Program counter 16-bit (mulai dari 0x200)
- Stack 16 level untuk subroutine
- Delay dan sound timer (60 Hz)
- Layar 64×32 pixel monokrom
- Keypad 16 tombol (0-F)

## Opcode yang Diimplementasi

| Range | Jenis | Status |
|---|---|---|
| `0NNN` | SYS (jarang dipakai) | ✅ Skip |
| `00E0` | Clear screen | ✅ |
| `00EE` | Return from subroutine | ✅ |
| `1NNN` | Jump | ✅ |
| `2NNN` | Call subroutine | ✅ |
| `3XNN` | Skip if VX == NN | ✅ |
| `4XNN` | Skip if VX != NN | ✅ |
| `5XY0` | Skip if VX == VY | ✅ |
| `6XNN` | Set VX = NN | ✅ |
| `7XNN` | VX += NN | ✅ |
| `8XYN` | Aritmatika & logika | ✅ |
| `9XY0` | Skip if VX != VY | ✅ |
| `ANNN` | Set I = NNN | ✅ |
| `BNNN` | Jump to NNN + V0 | ✅ |
| `CXNN` | VX = random & NN | ✅ |
| `DXYN` | Draw sprite | ✅ |
| `EX9E` | Skip if key VX pressed | ✅ |
| `EXA1` | Skip if key VX not pressed | ✅ |
| `FX07` | VX = delay_timer | ✅ |
| `FX0A` | Wait for key press | ✅ |
| `FX15` | delay_timer = VX | ✅ |
| `FX18` | sound_timer = VX | ✅ |
| `FX1E` | I += VX | ✅ |
| `FX29` | I = font address for VX | ✅ |
| `FX33` | Store BCD of VX | ✅ |
| `FX55` | Save V0-VX to memory | ✅ |
| `FX65` | Load V0-VX from memory | ✅ |

## Referensi

- [Wikipedia: CHIP-8](https://en.wikipedia.org/wiki/CHIP-8)
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Tobias V. Langhoff: Guide to making a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)

## Lisensi

MIT