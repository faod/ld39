/*
	Copyright 2016 Jonathan Bayle, Thomas Medioni

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "game.hpp"

#include <vivace/vivace.hpp>
#include "cyclist.hpp"
#include "food.hpp"
#include "map.hpp"
using namespace vivace;

#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

#include <glm/glm.hpp>

using namespace glm;

Game::Game():
		level("data/maps/01.tmx"),
        player_(level.tracks[2].get16pxPercentage()),
        foodspawner_(player_, level),
        layer_(al_create_bitmap(level.width, level.height))
{
    bg_colour = level.bg_color;
    //johnny = unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter>(al_load_bitmap("data/johnny_running.png"));
    for (int i=0; i<8; i++) {
        //ALLEGRO_BITMAP* bmp = al_create_sub_bitmap(johnny.get(), 35*i, 0, 35, 68);
        //character.add_frame(shared_ptr<ALLEGRO_BITMAP>(bmp, al_bitmap_deleter()), 0.1);
    }
    foodspawner_.set_game(this); 
    add(player_);
    add_update(std::bind(&FoodSpawner::update, foodspawner_, std::placeholders::_1));
    add_update(std::bind(&Game::update_food_pickup, this, std::placeholders::_1));
    add_draw_back(std::bind(&Game::draw_food, this));
}

void Game::update(double delta_t)
{
    if (delta_t >= 1.) {
        cerr << "lag!" << endl;
    }
    else {
        //pos.x += speed.x * 50. * delta_t;
        //pos.y += speed.y * 50. * delta_t;
    }
    Object_full_aggregator::update(delta_t);
    //character.update(delta_t);

    sum_t += delta_t;
    if (sum_t >= 1.)
    {
        sum_t = 0;
        mk_fps_string(delta_t);
    }
}

void Game::draw()
{
	al_clear_to_color(bg_colour);

    auto pos = level.tracks[player_.get_track()].getPosition(player_.get_pos());
    al_draw_scaled_rotated_bitmap(level.bitmap.get(),
            pos.x, // center x
            pos.y, // center y
            400,   // destination x
            450,   // destination y
            6.,    // xscale
            6.,    // yscale
            pos.p, // angle in rad
            0      // flags (flip)
            );
    //al_draw_filled_rectangle(pos.x - 5, pos.y - 5, pos.x + 5, pos.y + 5, al_map_rgb(255, 0, 128));
    //al_draw_bitmap(character.current().get(), 380+pos.x, 280-pos.y, 0);
    al_draw_scaled_rotated_bitmap(layer_.get(),
            pos.x, // center x
            pos.y, // center y
            400,   // destination x
            450,   // destination y
            6.,    // xscale
            6.,    // yscale
            pos.p, // angle in rad
            0      // flags (flip)
            );
    Object_full_aggregator::draw();
 
    al_draw_text(debug_font(), al_map_rgb_f(1,1,1), 792, 8, ALLEGRO_ALIGN_RIGHT, fps_string.c_str());
    al_flip_display();
}

void Game::handle(const ALLEGRO_EVENT& event)
{
    switch (event.type) {
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        cerr << "Bye!" << endl;
        throw 1;

    }
    Object_full_aggregator::handle(event);
}

void Game::mk_fps_string(double delta_t)
{
	ostringstream oss;
	oss << static_cast<int>(1/delta_t) << " fps";
	fps_string = oss.str();

}

void Game::spawn_food(std::unique_ptr<Food>&& food)
{
    foods_.emplace_back(std::move(food));
}

void Game::draw_food()
{
    ALLEGRO_BITMAP *restore = al_get_target_bitmap();
    al_set_target_bitmap(layer_.get());
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    for (auto& f : foods_)
    {
        f->draw();
    }
    al_set_target_bitmap(restore);
}

void Game::update_food_pickup(double delta_t)
{
    (void) delta_t;
    auto i = std::begin(foods_);
    auto pt = player_.get_track();
    auto pp = player_.get_pos();
    auto p16p = level.tracks[pt].get16pxPercentage();
    while (i != std::end(foods_))
    {
        auto& ptr = *i;
        if (ptr->get_track() == pt
            && std::abs(ptr->get_fpos() - pp) < p16p)
        {
            player_.add_power(ptr->get_power());
            i = foods_.erase(i);
            continue;
        }
        ++i;
    }
}

int main(void) {
	try {
        std::srand(std::time(nullptr));
		Vivace engine("GAME_NAME"s, ""s);
		map::initialize();
		unique_ptr<ALLEGRO_DISPLAY, al_display_deleter> dsp(al_create_display(800, 600));
		Game game;
		Basic_loop main_loop(1/30., game);
		main_loop.register_event_source(al_get_display_event_source(dsp.get()));
		main_loop.register_event_source(al_get_keyboard_event_source());
		main_loop.run();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	catch (int ex) {}
}
