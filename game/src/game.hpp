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

class Game: public vivace::Object_full_aggregator
{
public:
	Game();
	virtual void update(double delta_t) override;
	virtual void draw() override;
	virtual void handle(const ALLEGRO_EVENT& event) override;

    void spawn_food(std::unique_ptr<Food>&& food);
private:
    double sum_t = 0.;
    std::string fps_string;
    ALLEGRO_COLOR bg_colour;

    PlayerCyclist player_;
    map level;
    FoodSpawner foodspawner_;

    std::vector<std::unique_ptr<Food>> foods_;

    void mk_fps_string(double delta_t);
    void draw_food();
};

#endif
