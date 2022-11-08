#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <thread>
#include <chrono>

#include "vec2.h"
#include "ray.h"
#include "map.h"
#include "player.h"
#include "misc.h"
#include "renderer.h"
#include "updater.h"

using namespace std::chrono_literals;

class game_engine {
public:
    game_engine(): world_map(nullptr), plyr(nullptr), r(nullptr), upd(nullptr) {
        render_flag = new bool(true);
    }

    ~game_engine() {
        delete world_map;
        delete plyr;
        delete r;
        delete upd;
        delete render_flag;
    }

    void create_map(std::string map_file) {
        world_map = new map(map_file);
    }

    void create_player(vec2 spawn_location, vec2 looking_dir, double FOV) {
        plyr = new player(spawn_location, looking_dir, FOV, world_map);
    }

    void create_updater() {
        assert(world_map != nullptr && plyr != nullptr);
        upd = new updater(world_map, plyr);
    }

    void create_renderer(int width, int height) {
        assert(world_map != nullptr && plyr != nullptr);
        r = new renderer(width, height, world_map, plyr);
        plyr->calculate_ray_angles(r->screen_width);
    }

    bool check_quit() {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return true;
            } else
                return false;
        }
        return false;
    }

    void game_loop() {
        if (!*render_flag)
            std::this_thread::sleep_for(10ms);

        upd->update_player(render_flag);

        if (*render_flag) {
            r->render_to_backbuffer();
            r->swap_buffers();
            *render_flag = upd->key_down() ? true : false;
        }
    }

private:
    map* world_map;
    player* plyr;
    renderer* r;
    updater* upd;
    bool* render_flag;
};