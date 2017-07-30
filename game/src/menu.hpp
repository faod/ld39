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

#ifndef V_MENU_HPP
#define V_MENU_HPP
#pragma once

#include <vivace/object.hpp>
#include <memory>
#include <vector>

class Menu : public virtual vivace::Object
{
public:
    Menu();

private:
    virtual void update_impl(double delta_t) override;
    virtual void draw_impl() override;
    virtual void handle_impl(const ALLEGRO_EVENT& event) override;

    void select();

    std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter> menu_;
    
    unsigned cursor_;
    std::vector<std::string> choices_;
};
#endif
