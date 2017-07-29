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

class Food 
{
public:
    Food();
    void draw();
};

class FoodSpawner
{
public:
    FoodSpawner(PlayerCyclist& player);
    void update(double delta_t);
private:
    std::reference_wrapper<PlayerCyclist> player_;
    float time_to_spawn_;

    void spawn();
};
#endif
