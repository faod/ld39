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

#include "cyclist.hpp"
#include <sstream>
/**
 *
 * CYCLIST 
 *
 */
Cyclist::Cyclist() : speed_(1.)
{
    sprite_ = std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter>(al_create_bitmap(32, 68));
    al_set_target_bitmap(sprite_.get());
    al_clear_to_color(al_map_rgb(255, 0, 255));
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
}

Cyclist::~Cyclist()
{
}

void Cyclist::update(double delta_t)
{
    //update cyclist movement
}

void Cyclist::draw()
{
    al_draw_bitmap(sprite_.get(), 380 + pos_.x, 280 - pos_.y, 0);
}

void Cyclist::handle(const ALLEGRO_EVENT& event)
{
    //Handle events : void for bots
}

/**
 *
 * PLAYER CYCLIST
 *
 */
PlayerCyclist::PlayerCyclist() : Cyclist(), power_(1000)
{
    add_draw_back( [&]() {
            al_draw_rectangle(772, 150, 790, 500, al_map_rgb(0, 255, 0), 1.);
            
            const int size = glm::clamp(power_, 0, maxpower) * 350 / maxpower;
            al_draw_filled_rectangle(772, 500 - size, 790, 500, al_map_rgb(0, 240, 0));
            std::ostringstream oss;
            oss << power_ << " kcal";
            al_draw_text(debug_font(), al_map_rgb(0, 255, 0), 790, 130, ALLEGRO_ALIGN_RIGHT, oss.str().c_str());
            });

#ifndef NDEBUG
    #define POS
#ifdef POS
    add_draw_back( [&]() {
            std::ostringstream oss;
            oss << "x: " << pos_.x << " y: " << pos_.y;
            al_draw_text(debug_font(), al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, oss.str().c_str());
            });
#endif
#endif
}

PlayerCyclist::~PlayerCyclist()
{
}
void PlayerCyclist::update(double delta_t)
{
    Cyclist::update(delta_t);
}
void PlayerCyclist::draw()
{
    Cyclist::draw();
    Object_split_aggregator::draw();
}
void PlayerCyclist::handle(const ALLEGRO_EVENT& event) 
{
}
