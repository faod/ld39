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

#include "food.hpp"
#include "game.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>

/**
 *
 * FOOD
 *
 */
Food::Food(glm::dvec3 pos, int track) : pos_(pos), track_(track)
{
}

void Food::draw() const
{
    al_draw_filled_rectangle(pos_.x - 5, pos_.y - 5, pos_.x + 5, pos_.y + 5, al_map_rgb(0, 0, 255));
}

int Food::get_track() const
{
    return track_;
}

glm::dvec3 Food::get_pos() const
{
    return pos_;
}

/**
 *
 * FOOD SPAWNER
 *
 */

FoodSpawner::FoodSpawner(PlayerCyclist& player, map& level) :
                player_(player),
                level_(level),
                game_(nullptr),
                time_to_spawn_(0.)
{
}

void FoodSpawner::update(double delta_t)
{
    time_to_spawn_ -= delta_t;

    if (time_to_spawn_ < 0.)
        spawn();
}

void FoodSpawner::spawn()
{
    time_to_spawn_ = glm::linearRand(5., 20.);
    
    // spawn one food
    const int track = glm::linearRand(0, 4);
    const double tsize = level_.get().tracks[track].get16pxPercentage();
    auto& p = player_.get();
    const float pos = glm::linearRand(p.get_pos() + tsize, p.get_pos() + tsize * 8);
    assert(game_);
    game_->spawn_food(std::make_unique<Food>(level_.get().tracks[track].getPosition(pos), track));
}

void FoodSpawner::set_game(Game* g)
{
    game_ = g;
}
