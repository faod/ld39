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
#include <string>

class Game;
class Menu : public virtual vivace::Object_aggregator
{
public:
    Menu(Menu* parent = nullptr);
    void add_entry(std::string display, std::function<void()> on_select);
    void add_entry(std::string display, std::unique_ptr<Menu>&& submenu);
    void escape();
    void disable_parent();
    void sub_opened(bool);
	void set_entry_settings(float offset_x, float offset_y, float scale);
	void set_bg_col(ALLEGRO_COLOR col);
	void set_background(std::shared_ptr<ALLEGRO_BITMAP> bg, float scale_x, float scale_y);
private:
    virtual void update_impl(double delta_t) override;
    virtual void draw_impl() override;
    virtual void handle_impl(const ALLEGRO_EVENT& event) override;

    void select();

	float off_x, off_y, scale; // entry rendering settings
	std::shared_ptr<ALLEGRO_BITMAP> background;
	float bg_scale_x, bg_scale_y;
	unsigned int bg_h, bg_w;
	ALLEGRO_COLOR bg_color;
    std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter> menu_;
    unsigned cursor_;
    bool cursor_activate_;
    std::vector<std::tuple<std::string, std::function<void()>, std::unique_ptr<Menu>>> choices_;
    bool sub_opened_;
    Menu* parent_;

	static ALLEGRO_BITMAP* cursor_bmp;
};

std::unique_ptr<Menu> make_map_selection_menu(Game* g, Menu* parent);
#endif
