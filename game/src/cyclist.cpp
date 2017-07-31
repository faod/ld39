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
#define is_zero(x) (x < 1e-6 && x > -1e-6)

#include "cyclist.hpp"
#include "map.hpp"
#include <sstream>

/**
 *
 * PLAYER CYCLIST
 *
 */
Cyclist::Cyclist(float forwardper16px):
	pos_(0.), speed_(forwardper16px * 3.), sprinting_(false), sprinting_ratio_(1.), track_(2),
	sprite_(reinterpret_cast<ALLEGRO_BITMAP*>(al_img_loader("data/maillot_jaune.png"))),
    power_(1000.),
    track_change_time_(0.),
    paused_(false),
    timer_(0.)
{
    using vivace::Drawable;
    auto db_fct =  [&]() {
            al_draw_rectangle(772, 150, 790, 500, al_map_rgb(0, 255, 0), 1.);
            
            const int size = glm::clamp(static_cast<int>(power_), 0, maxpower) * 350 / maxpower;
            al_draw_filled_rectangle(772, 500 - size, 790, 500, al_map_rgb(0, 240, 0));
            std::ostringstream oss;
            oss << power_ << " kcal";
            al_draw_text(debug_font(), al_map_rgb(0, 255, 0), 790, 130, ALLEGRO_ALIGN_RIGHT, oss.str().c_str());
            std::ostringstream oss2;
            oss2.precision(2);
            oss2 << std::fixed << timer_ << "s";
            al_draw_text(debug_font(), al_map_rgb(0, 255, 0), 790, 110, ALLEGRO_ALIGN_RIGHT, oss2.str().c_str());

            };
    auto db_object = std::make_unique<Drawable>(db_fct);
    add(*db_object);
    objects_.emplace_back(std::move(db_object));

#ifndef NDEBUG
    #define POS
    #define SPT_RATIO
    #define TRACK
#ifdef POS
    auto pos_fct = [&]() {
            std::ostringstream oss;
            oss.precision(5);
            oss << "pos: " << pos_ * 100 << "%";
            al_draw_text(debug_font(), al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, oss.str().c_str());
            };
    auto pos_object = std::make_unique<Drawable>(pos_fct);
    add(*pos_object);
    objects_.emplace_back(std::move(pos_object));
#endif
#ifdef SPT_RATIO
    auto spt_fct = [&]() {
            std::ostringstream oss;
            oss.precision(3);
            oss << "SPT ratio: " << sprinting_ratio_;
            al_draw_text(debug_font(), al_map_rgb(255, 255, 255), 10, 20, ALLEGRO_ALIGN_LEFT, oss.str().c_str());
            };
    auto spt_object = std::make_unique<Drawable>(spt_fct);
    add(*spt_object);
    objects_.emplace_back(std::move(spt_object));
#endif
#ifdef TRACK
    auto track_fct = [&]() {
            std::ostringstream oss;
            oss << "Track: " << track_;
            al_draw_text(debug_font(), al_map_rgb(255, 255, 255), 10, 30, ALLEGRO_ALIGN_LEFT, oss.str().c_str());
            };
    auto track_object = std::make_unique<Drawable>(track_fct);
    add(*track_object);
    objects_.emplace_back(std::move(track_object));
#endif
#endif
}

Cyclist::~Cyclist()
{
}

int Cyclist::get_track()
{
    return track_;
}

float Cyclist::get_pos()
{
    return pos_;
}

bool Cyclist::paused() const
{
    return paused_;
}
void Cyclist::set_pause(bool pause)
{
    paused_ = pause;
}
void Cyclist::update_impl(double delta_t)
{
    if (pos_ >= 1. || power_ <= 0)
    {
       return; 
    }
    timer_ += delta_t;

    //update cyclist movement
    if (sprinting_)
        sprinting_ratio_ = glm::clamp(sprinting_ratio_ + delta_t, 1., 2.);
    else
        sprinting_ratio_ = glm::clamp(sprinting_ratio_ - delta_t, 1., 2.);
    pos_ += speed_ * sprinting_ratio_ * delta_t;

    update_track_change(delta_t);
    power_ -= 100. * sprinting_ratio_ * delta_t;

    if (power_ < 0) //LOST
    {
        power_ = 0;
    }
}
void Cyclist::draw_impl()
{
    al_draw_scaled_bitmap(sprite_.get(),
                          0,
                          0,
                          11,
                          29,
                          400 - 33, 
                          450 - 87,
                          66, 
                          174,
                          0);
    Object_aggregator::draw_impl();
}
void Cyclist::handle_impl(const ALLEGRO_EVENT& event) 
{
    Object_aggregator::handle_impl(event);
    switch(event.type)
    {
        case ALLEGRO_EVENT_KEY_DOWN:
            switch(event.keyboard.keycode)
            {
                case ALLEGRO_KEY_LCTRL:
                    sprinting_ = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    if (track_ > 0 && is_zero(track_change_time_))
                    {
                        track_change_time_ = .4;
                        new_track_ = track_ - 1;
                    }
                    break;
                case ALLEGRO_KEY_RIGHT:
                    if (track_ < 4 && is_zero(track_change_time_))
                    {
                        track_change_time_ = .4;
                        new_track_ = track_ + 1;
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    paused_ = true;
                    break;
            }
            break;
        case ALLEGRO_EVENT_KEY_UP:
            switch(event.keyboard.keycode)
            {
                case ALLEGRO_KEY_LCTRL:
                    sprinting_ = false;
                    break;
            }
            break;
    }
}

bool Cyclist::alive() const
{
    return power_ > 0;
}

void Cyclist::update_track_change(float delta_t)
{
    track_change_time_ -= delta_t;
    if (track_change_time_ > 0. && track_change_time_ < .2)
        track_ = new_track_;
    if (track_change_time_ < 0.)
        track_change_time_ = 0;
}

bool Cyclist::finished() const
{
    return pos_ >= 1.; 
}

float Cyclist::elapsed() const
{
    return timer_;
}

void Cyclist::add_power(int amount)
{
    assert(amount > 0);
    power_ += amount;
}
