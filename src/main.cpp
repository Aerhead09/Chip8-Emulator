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

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        for (int i = 0; i < CYCLES_PER_FRAME; i++) {
            chip8.cycle();
        }

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