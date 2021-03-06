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

#ifndef V_FOOD_HPP
#define V_FOOD_HPP
#pragma once

#include "cyclist.hpp"
#include "map.hpp"

class Food 
{
public:
    Food(glm::dvec3 pos, float fpos, int track, int power);
   
    void draw(double angle) const;

    glm::dvec3 get_pos() const;
    float get_fpos() const;
    int get_track() const;
    int get_power() const;
private:
    glm::dvec3 pos_;
    float fpos_;
    int track_;
    int power_;
    std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter> sprite_;
};

class Game;
class FoodSpawner
{
public:
    FoodSpawner(Cyclist& player, map& level);
    
    void set_game(Game* g);
    void update(double delta_t);
private:
    std::reference_wrapper<Cyclist> player_;
    std::reference_wrapper<map> level_;
    Game* game_;

    float time_to_spawn_;

    void spawn();
};
#endif
