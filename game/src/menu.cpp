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
               cursor_(0),
               choices_({"PLAY", "QUIT"})
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

    for (unsigned i = 0; i < choices_.size(); ++i)
    {
        auto&s = choices_[i];
        auto color = cursor_ == i ? al_map_rgb(0, 0, 255) : al_map_rgb(255, 0, 0);
        al_draw_text(debug_font(), color, 400, 200 + 20 * i, ALLEGRO_ALIGN_CENTRE, s.c_str());
    }

    al_set_target_bitmap(restore);
    al_draw_bitmap(menu_.get(), 0, 0, 0);
}

void Menu::handle_impl(const ALLEGRO_EVENT& event)
{
    switch (event.type)
    {
        case ALLEGRO_EVENT_KEY_DOWN:
            switch (event.keyboard.keycode)
            {
                case ALLEGRO_KEY_DOWN:
                    cursor_ = cursor_ < choices_.size() - 1 ? cursor_ + 1 : 0;
                    break;
                case ALLEGRO_KEY_UP:
                    cursor_ = cursor_ > 0 ? cursor_ - 1 : choices_.size() - 1;
                    break;
                case ALLEGRO_KEY_SPACE:
                    select();
                    break;
            }
            break;
    }
}

void Menu::select()
{
    switch(cursor_)
    {
        case 0: //PLAY
            break;
        case 1: //QUIT
            throw 1;
            break;
    }
}
