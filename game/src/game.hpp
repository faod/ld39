/*
    Copyright 2017 Jonathan Bayle, Thomas Medioni

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

#ifndef V_GAME_HPP
#define V_GAME_HPP
#pragma once

#include <vivace/object.hpp>
#include "cyclist.hpp"
#include "food.hpp"
#include "map.hpp"
#include "menu.hpp"

class Game: public vivace::Object_aggregator
{
public:
	Game();

    void spawn_food(std::unique_ptr<Food>&& food);

    protected:
	virtual void update_impl(double delta_t) override;
	virtual void draw_impl() override;
	virtual void handle_impl(const ALLEGRO_EVENT& event) override;
private:
    double sum_t = 0.;
    std::string fps_string;
    ALLEGRO_COLOR bg_colour;

    std::unique_ptr<map> level_;
    std::unique_ptr<PlayerCyclist> player_;
    std::unique_ptr<FoodSpawner> foodspawner_;

    std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter> layer_;
    
    std::vector<std::unique_ptr<Object>> objects_;
    std::vector<std::unique_ptr<Food>> foods_;

    Menu menu_;

    void mk_fps_string(double delta_t);
    void draw_food();
    void update_food_pickup(double delta_t);
    void load_game();
};

#endif
