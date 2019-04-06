#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "clock.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    int window_width = 640;
    int window_height = 480;
    SDL_Window* window;
    SDL_Renderer* renderer;
    if (SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        printf("Couldn't create window and renderer: %s", SDL_GetError());
        return -1;
    }
    SDL_SetWindowTitle(window, "Clock arm test");

    struct IkRes ik_res = clock_ik(100, 100);
    printf("a1 %f, a2 %f\n", ik_res.angle1 / M_PI * 180, ik_res.angle2 / M_PI * 180);

    float time = 0.0f;
    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    //screen = SDL_GetWindowSurface(window);
                    window_width = e.window.data1;
                    window_height = e.window.data2;
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        int center_x = window_width / 2;
        int center_y = window_height / 2;

        // Draw clock face
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        for (int i = 0; i < 30; i++) {
            float i_angle = (float)i / 15.0f * M_PI;

            float x1 = CLOCK_RADIUS * cos(i_angle);
            float y1 = CLOCK_RADIUS * sin(i_angle);
            float x2 = CLOCK_RADIUS * cos(i_angle + M_PI / 15.0f);
            float y2 = CLOCK_RADIUS * sin(i_angle + M_PI / 15.0f);
            SDL_RenderDrawLine(
                renderer,
                center_x + x1, center_y + y1,
                center_x + x2, center_y + y2
            );
        }

        // Draw hand path
        SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 0xFF);
        for (int i = 0; i < 12; i++) {
            struct vec2 pos1 = number_position(i);
            struct vec2 pos2 = number_position(i + 1);
            SDL_RenderDrawLine(
                renderer,
                center_x + pos1.x, center_y + pos1.y,
                center_x + pos2.x, center_y + pos2.y
            );
        }

        struct vec2 target = hand_target(time);
        struct IkRes ik_res = clock_ik(target.x, target.y);

        int big_end_x = center_x + JOINT_1_LENGTH * cos(ik_res.angle1);
        int big_end_y = center_y + JOINT_1_LENGTH * -sin(ik_res.angle1);
        int small_end_x = big_end_x + JOINT_2_LENGTH * cos(ik_res.angle1 + ik_res.angle2);
        int small_end_y = big_end_y + JOINT_2_LENGTH * -sin(ik_res.angle1 + ik_res.angle2);
        // Draw clock hand
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderDrawLine(renderer, center_x, center_y, big_end_x, big_end_y);
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawLine(renderer, big_end_x, big_end_y, small_end_x, small_end_y);

        SDL_RenderPresent(renderer);
        SDL_Delay(33);
        time += 0.033f;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
