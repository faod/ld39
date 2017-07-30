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

Game::Game() : want_menu_(false), want_reload_(false)
{
    bg_colour = al_map_rgb(0, 0, 40);
    add(menu_);
    
    menu_.add_entry("PLAY", make_map_selection_menu(this, &menu_));
    menu_.add_entry("QUIT", [&]() { throw 1;});
    gameover_ = std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter>(reinterpret_cast<ALLEGRO_BITMAP*>(al_img_loader("data/gameover.png")));
}

void Game::load_game(std::string map_name)
{
    auto it = std::begin(objects_);
    while (it != std::end(objects_))
    {
        remove((**it));
        it = objects_.erase(it);
    }

    auto drawmap_fct = [&]() {  
        auto pos = level_->tracks[player_->get_track()].get_position(level_->tracks[2], player_->get_pos());
        al_draw_scaled_rotated_bitmap(level_->bitmap.get(),
            pos.x, // center x
            pos.y, // center y
            400,   // destination x
            450,   // destination y
            6.,    // xscale
            6.,    // yscale
            pos.p, // angle in rad
            0      // flags (flip)
            );
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
    };
    auto drawmap_object = std::make_unique<Drawable>(drawmap_fct);

    level_ = std::make_unique<map>(map_name.c_str());
    
    if (player_)
    {
        remove(*player_);
    }
    player_ = std::make_unique<PlayerCyclist>(level_->tracks[2].get_16px_percentage());
    add(*player_);
    add(*drawmap_object);
    objects_.emplace_back(std::move(drawmap_object));

    foodspawner_ = std::make_unique<FoodSpawner>(*player_, *level_);
    foodspawner_->set_game(this); 
    
    if (!layer_)
        layer_.reset(al_create_bitmap(level_->width, level_->height));

    bg_colour = level_->bg_color;

    foods_.clear();
    auto spawner_fct = std::bind(&FoodSpawner::update, foodspawner_.get(), std::placeholders::_1);
    auto spawner_object = std::make_unique<Updatable>(spawner_fct);
    add(*spawner_object);
    objects_.emplace_back(std::move(spawner_object));

    auto pickup_fct = std::bind(&Game::update_food_pickup, this, std::placeholders::_1);
    auto pickup_object = std::make_unique<Updatable>(pickup_fct);
    add(*pickup_object);
    objects_.emplace_back(std::move(pickup_object));

    auto fooddraw_fct = std::bind(&Game::draw_food, this);
    auto fooddraw_object = std::make_unique<Drawable>(fooddraw_fct);
    add(*fooddraw_object);
    objects_.emplace_back(std::move(fooddraw_object));

}

void Game::update_impl(double delta_t)
{
    if (delta_t >= 1.) {
        cerr << "lag!" << endl;
    }
    Object_aggregator::update_impl(delta_t);

    if (want_menu_)
    {
        want_menu_ = false;
        menu_.activate(true);
    }

    if (want_reload_)
    {
        want_reload_ = false;
        load_game(level_->location_);
    }

    if (player_ && player_->activated() && !player_->alive())
    {
        game_over();
    }

    if (player_ && player_->activated() && player_->finished())
    {
        game_won();
    }

    sum_t += delta_t;
    if (sum_t >= 1.)
    {
        sum_t = 0;
        mk_fps_string(delta_t);
    }
}

void Game::draw_impl()
{
	al_clear_to_color(bg_colour);

   Object_aggregator::draw_impl();
 
    al_draw_text(debug_font(), al_map_rgb_f(1,1,1), 792, 8, ALLEGRO_ALIGN_RIGHT, fps_string.c_str());
    al_flip_display();
}

void Game::handle_impl(const ALLEGRO_EVENT& event)
{
    switch (event.type) {
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        cerr << "Bye!" << endl;
        throw 1;

    }
    Object_aggregator::handle_impl(event);
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
	auto pos = level_->tracks[player_->get_track()].get_position(level_->tracks[2], player_->get_pos());
    for (auto& f : foods_)
    {
        f->draw(-pos.p);
    }
    al_set_target_bitmap(restore);
}

void Game::update_food_pickup(double delta_t)
{
    (void) delta_t;
    auto i = std::begin(foods_);
    auto pt = player_->get_track();
    auto pp = player_->get_pos();
    auto p16p = level_->tracks[pt].get_16px_percentage();
    while (i != std::end(foods_))
    {
        auto& ptr = *i;
        if (ptr->get_track() == pt
            && std::abs(ptr->get_fpos() - pp) < p16p)
        {
            player_->add_power(ptr->get_power());
            i = foods_.erase(i);
            continue;
        }
        ++i;
    }
}
void Game::disable_all()
{
    for (auto& o: objects_)
    {
        o->activate(false);
        player_->activate(false);
        menu_.activate(false);
    }
}

void Game::game_over()
{
    disable_all();
    auto drawmap_fct = [&]() {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(gameover_.get(),
            400 - 77,   // destination x
            600 - 203,   // destination y
            0      // flags (flip)
            );
        al_draw_text(debug_font(), al_map_rgb(255, 0, 0), 400, 300, ALLEGRO_ALIGN_CENTRE, "WASTED");
        al_draw_text(debug_font(), al_map_rgb(255, 255, 0), 400, 400, ALLEGRO_ALIGN_CENTRE, "[R]etry - Space to Main Menu");
        };
    auto drawmap_object = std::make_unique<Drawable>(drawmap_fct);
    add(*drawmap_object);
    objects_.emplace_back(std::move(drawmap_object));

    auto gameover_fct = [&](const ALLEGRO_EVENT& event){
        switch(event.type)
        {
            case ALLEGRO_EVENT_KEY_DOWN:
                switch(event.keyboard.keycode)
                {
                    case ALLEGRO_KEY_R:
                        disable_all();
                        want_reload_ = true;
                    break;
                    case ALLEGRO_KEY_SPACE:
                        disable_all();
                        want_menu_ = true;;
                    break;
                }
            break;
        }
    };
    auto gameover_object = std::make_unique<Listener>(gameover_fct);
    add(*gameover_object);
    objects_.emplace_back(std::move(gameover_object));
}

void Game::game_won()
{
    disable_all();
    auto drawmap_fct = [&]() {
        al_clear_to_color(al_map_rgb(0, 255, 255));
        /*al_draw_bitmap(gameover_.get(),
            400 - 77,   // destination x
            600 - 203,   // destination y
            0      // flags (flip)
            );
            */
        al_draw_text(debug_font(), al_map_rgb(255, 0, 0), 400, 300, ALLEGRO_ALIGN_CENTRE, "VICTORY");
        al_draw_text(debug_font(), al_map_rgb(255, 255, 0), 400, 400, ALLEGRO_ALIGN_CENTRE, "[R]un Again - Space to Main Menu");
        };
    auto drawmap_object = std::make_unique<Drawable>(drawmap_fct);
    add(*drawmap_object);
    objects_.emplace_back(std::move(drawmap_object));

    auto gamewon_fct = [&](const ALLEGRO_EVENT& event){
        switch(event.type)
        {
            case ALLEGRO_EVENT_KEY_DOWN:
                switch(event.keyboard.keycode)
                {
                    case ALLEGRO_KEY_R:
                        disable_all();
                        want_reload_ = true;
                    break;
                    case ALLEGRO_KEY_SPACE:
                        disable_all();
                        want_menu_ = true;;
                    break;
                }
            break;
        }
    };
    auto gamewon_object = std::make_unique<Listener>(gamewon_fct);
    add(*gamewon_object);
    objects_.emplace_back(std::move(gamewon_object));
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
