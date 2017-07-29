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
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
/**
 *
 * FOOD
 *
 */

Food::Food()
{
}

void Food::draw()
{
    //TODO
}

/**
 *
 * FOOD SPAWNER
 *
 */

FoodSpawner::FoodSpawner(PlayerCyclist& player) : player_(player), time_to_spawn_(0.)
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
    //const int track = glm::linearRand(0, 4);

}
