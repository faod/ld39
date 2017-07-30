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

#include "menu.hpp"

Menu::Menu() : menu_(al_create_bitmap(800, 600)),
               cursor_(0)
{
}

void Menu::update_impl(double delta_t)
{
    (void) delta_t;
}

void Menu::draw_impl() 
{
    ALLEGRO_BITMAP *restore = al_get_target_bitmap();
    al_set_target_bitmap(menu_.get());
    al_clear_to_color(al_map_rgb(0, 240, 0));

    al_draw_text(debug_font(), al_map_rgb(255, 0, 0), 400, 200, ALLEGRO_ALIGN_CENTRE, "PLAY");

    al_set_target_bitmap(restore);
    al_draw_bitmap(menu_.get(), 0, 0, 0);
}

void Menu::handle_impl(const ALLEGRO_EVENT& event)
{
    (void) event;
}
