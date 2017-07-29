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

class Cyclist : public virtual vivace::Object {
public:
    Cyclist();
    virtual void update(double delta_t) override;
    virtual void draw() override;
    virtual void handle(const ALLEGRO_EVENT& event) override;

    virtual ~Cyclist();

protected:
    glm::vec2 pos_{0., 0.};
    float speed_;
    std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter> sprite_;
};

class PlayerCyclist : public Cyclist, public vivace::Object_split_aggregator {
public:
    PlayerCyclist();
    virtual void update(double delta_t) override;
    virtual void draw() override;
    virtual void handle(const ALLEGRO_EVENT& event) override;

    virtual ~PlayerCyclist();

private:
    int power_;
    //Right now, a soft maxpower used to limit the drawing of power
    static const int maxpower = 1500;
};
#endif
