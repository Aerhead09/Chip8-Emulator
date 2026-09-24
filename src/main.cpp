#include <SDL3/SDL.h>
#include <iostream>
#include "chip8.h"

constexpr int SCALE = 10;
constexpr int WINDOW_WIDTH = Chip8::VIDEO_WIDTH * SCALE;
constexpr int WINDOW_HEIGHT = Chip8::VIDEO_HEIGHT * SCALE;
constexpr int CYCLES_PER_FRAME = 10;
constexpr int FRAME_DELAY_MS = 16;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Cara pakai: chip8.exe <rom_file>\n";
        return 1;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init gagal: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0
    );

    if (window == nullptr) {
        std::cerr << "CreateWindow gagal: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == nullptr) {
        std::cerr << "CreateRenderer gagal: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Chip8 chip8;
    chip8.loadROM(argv[1]);

    bool running = true;
    SDL_Event event;
    Uint64 lastTimerUpdate = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                bool pressed = (event.type == SDL_EVENT_KEY_DOWN);
                int key = -1;
                switch (event.key.key) {
                    case SDLK_1: key = 0x1; break;
                    case SDLK_2: key = 0x2; break;
                    case SDLK_3: key = 0x3; break;
                    case SDLK_4: key = 0xC; break;
                    case SDLK_Q: key = 0x4; break;
                    case SDLK_W: key = 0x5; break;
                    case SDLK_E: key = 0x6; break;
                    case SDLK_R: key = 0xD; break;
                    case SDLK_A: key = 0x7; break;
                    case SDLK_S: key = 0x8; break;
                    case SDLK_D: key = 0x9; break;
                    case SDLK_F: key = 0xE; break;
                    case SDLK_Z: key = 0xA; break;
                    case SDLK_X: key = 0x0; break;
                    case SDLK_C: key = 0xB; break;
                    case SDLK_V: key = 0xF; break;
                }
                if (key >= 0) {
                    chip8.keypad[key] = pressed ? 1 : 0;
                }
            }
        }

        // Eksekusi instruksi
        for (int i = 0; i < CYCLES_PER_FRAME; i++) {
            chip8.cycle();
        }

        // Update timer 60 Hz (setiap 16ms)
        Uint64 now = SDL_GetTicks();
        if (now - lastTimerUpdate >= 16) {
            if (chip8.delay_timer > 0) chip8.delay_timer--;
            if (chip8.sound_timer > 0) chip8.sound_timer--;
            lastTimerUpdate = now;
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int y = 0; y < Chip8::VIDEO_HEIGHT; y++) {
            for (int x = 0; x < Chip8::VIDEO_WIDTH; x++) {
                if (chip8.video[y * Chip8::VIDEO_WIDTH + x]) {
                    SDL_FRect rect = {
                        (float)(x * SCALE),
                        (float)(y * SCALE),
                        (float)SCALE,
                        (float)SCALE
                    };
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(FRAME_DELAY_MS);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}