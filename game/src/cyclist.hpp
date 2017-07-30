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

class Cyclist : public virtual vivace::Object {
public:
    Cyclist(float forwardp16px, ALLEGRO_BITMAP* sp = nullptr);

    virtual ~Cyclist();

    float get_pos();
    int get_track();

protected:
    virtual void update_impl(double delta_t) override;
    virtual void draw_impl() override;
    virtual void handle_impl(const ALLEGRO_EVENT& event) override;

    float pos_;
    float speed_;
    bool sprinting_;
    float sprinting_ratio_;
    int track_;
    
    std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter> sprite_;
    
    //protected fcts
    void update_sprinting_ratio(double delta_t);
};

class PlayerCyclist : public Cyclist, public vivace::Object_aggregator {
public:
    PlayerCyclist(float forwardper16px);

    virtual ~PlayerCyclist();

    bool alive() const;
    bool finished() const;
    void add_power(int amount);

private:
    virtual void update_impl(double delta_t) override;
    virtual void draw_impl() override;
    virtual void handle_impl(const ALLEGRO_EVENT& event) override;
    
    float power_;
    float track_change_time_;
    int new_track_;
    //Right now, a soft maxpower used to limit the drawing of power
    static const int maxpower = 1500;
    std::vector<std::unique_ptr<Object>> objects_;
    void update_track_change(float delta_t);
};
#endif
