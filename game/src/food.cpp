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
#include "map.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>

/**
 *
 * FOOD
 *
 */
Food::Food(glm::dvec3 pos, float fpos, int track, int power) : pos_(pos), fpos_(fpos), track_(track), power_(power)
{
    void* sp = nullptr;
    if (power_ < 700)
        sp = al_img_loader("data/banana.png");
    else if (power_ < 1100)
        sp = al_img_loader("data/powerade.png");
    else
        sp = al_img_loader("data/epo.png");

    sprite_ = std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter>(reinterpret_cast<ALLEGRO_BITMAP*>(sp));
}

void Food::draw(double angle) const
{
#ifndef NDEBUG
	al_draw_rectangle(pos_.x-8, pos_.y-8, pos_.x+8, pos_.y+8, al_map_rgb(255, 0, 0), 1.);
#endif
	al_draw_rotated_bitmap(sprite_.get(),
			8,      8,
			pos_.x, pos_.y,
			angle, 0);
}

int Food::get_track() const
{
    return track_;
}

glm::dvec3 Food::get_pos() const
{
    return pos_;
}

float Food::get_fpos() const
{
    return fpos_;
}

int Food::get_power() const
{
    return power_;
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
    time_to_spawn_ = glm::linearRand(3., 8.);
    
    // spawn one food
    const int track = glm::linearRand(0, 4);
    const double tsize = level_.get().tracks[track].get_16px_percentage();
    auto& p = player_.get();
    const float pos = glm::linearRand(p.get_pos() + tsize * 3, p.get_pos() + tsize * 8);
    const int power = glm::linearRand(300, 1500);
    assert(game_);
	glm::dvec3 pos2d = level_.get().tracks[track].get_position(level_.get().tracks[2], pos);
	std::cout << "Spawn food at (" << pos2d.x << ", " << pos2d.y << ')' << std::endl;
    game_->spawn_food(std::make_unique<Food>(pos2d, pos, track, power));
}

void FoodSpawner::set_game(Game* g)
{
    game_ = g;
}
