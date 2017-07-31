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

#ifndef V_CYCLIST_HPP
#define V_CYCLIST_HPP
#pragma once

#include <vivace/object.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "competitors.hpp"

class Cyclist: public vivace::Object_aggregator {
public:
    Cyclist(Competitors& competitors, float forwardper16px);

    virtual ~Cyclist();

    bool alive() const;
    bool finished() const;
    bool paused() const;
    float elapsed() const;
    void add_power(int amount);
    void set_pause(bool pause);
    float get_pos();
    int get_track();
private:
    virtual void update_impl(double delta_t) override;
    virtual void draw_impl() override;
    virtual void handle_impl(const ALLEGRO_EVENT& event) override;

	float percent16px;
    float pos_;
    float speed_;
    bool sprinting_;
    float sprinting_ratio_;
    int track_;
    std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter> sprite_;

    float power_;
    float track_change_time_;
    int new_track_;
    bool paused_;
    //Right now, a soft maxpower used to limit the drawing of power
    static const int maxpower = 1500;
    std::vector<std::unique_ptr<Object>> objects_;
    void update_track_change(float delta_t);
    float timer_;
	Competitors& competitors;
};
#endif
