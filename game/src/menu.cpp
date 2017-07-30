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
#include "game.hpp"
#include "vivace/base.hpp"
#include "vivace/utils.hpp"
#include <algorithm>
Menu::Menu(Menu* parent) : bg_color(al_map_rgb(0, 0, 0)),
               menu_(al_create_bitmap(800, 600)),
               cursor_(0),
               cursor_activate_(false),
               sub_opened_(false),
               parent_(parent)
{
	if (!Menu::cursor_bmp)
	{
		Menu::cursor_bmp = reinterpret_cast<ALLEGRO_BITMAP*>(al_img_loader("data/cursor.png"));
	}
}

ALLEGRO_BITMAP* Menu::cursor_bmp = NULL;

void Menu::update_impl(double delta_t)
{
    (void) delta_t;
    if (cursor_activate_)
    {
        cursor_activate_ = false;
        sub_opened_ = true;
        std::get<2>(choices_[cursor_])->activate(true);
    }
}

void Menu::add_entry(std::string display, std::function<void()> on_select)
{
    choices_.push_back(std::make_tuple(display, on_select, nullptr));
}

void Menu::add_entry(std::string display, std::unique_ptr<Menu>&& submenu)
{
    add(*submenu);
    choices_.push_back(std::make_tuple(display, std::function<void()>(), std::move(submenu))); 
}

void Menu::set_bg_col(ALLEGRO_COLOR col)
{
	bg_color = col;
}

void Menu::sub_opened(bool op)
{
    sub_opened_ = op;
}

void Menu::set_background(std::shared_ptr<ALLEGRO_BITMAP> bg, float scale_x, float scale_y)
{
	vivace::runtime_assert(bg.get(), "Cannot set NULL background to menu");
	background = bg;
	bg_scale_x = scale_x;
	bg_scale_y = scale_y;
	bg_h = al_get_bitmap_height(bg.get());
	bg_w = al_get_bitmap_width(bg.get());
}

void Menu::draw_impl() 
{
    ALLEGRO_BITMAP *restore = al_get_target_bitmap();
    al_set_target_bitmap(menu_.get());
    al_clear_to_color(bg_color);

	if (background.get())
	{
		al_draw_scaled_rotated_bitmap(background.get(), bg_w/2.f, bg_h/2.f, 400, 300, bg_scale_x, bg_scale_y, 0.f, 0);
	}

    for (unsigned i = 0; i < choices_.size(); ++i)
    {
        auto&s = std::get<0>(choices_[i]);
        if (cursor_ == i) {
			al_draw_scaled_rotated_bitmap(cursor_bmp,
			    8.5f, 11.f,
			    370, 205 + 40 * i,
			    2.f, 2.f,
			    1.57f, 0);
		}
        draw_scaled_string(debug_font(), al_map_rgb(255, 255, 255), 400, 200 + 40 * i, 2.f, 0, s);
    }

    al_set_target_bitmap(restore);
    al_draw_bitmap(menu_.get(), 0, 0, 0);

    Object_aggregator::draw_impl();
}

void Menu::handle_impl(const ALLEGRO_EVENT& event)
{
    if (sub_opened_)
    {
        Object_aggregator::handle_impl(event);
        return;
    }
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
                case ALLEGRO_KEY_ESCAPE:
                    escape();
                    break;
            }
            break;
    }
    
}

void Menu::select()
{
    auto& f = std::get<1>(choices_[cursor_]);
    if (f != nullptr)
        f();
    else
    {
        cursor_activate_ = true;
    }
}

void Menu::escape()
{
    if (parent_)
    {
        activate(false);
        parent_->sub_opened_ = false;
    }
}

void Menu::disable_parent()
{
    assert(parent_);
    parent_->activate(false);
}
std::unique_ptr<Menu> make_map_selection_menu(Game* g, Menu* parent)
{
    using fsep = std::unique_ptr<ALLEGRO_FS_ENTRY, al_fs_entry_deleter>;
    fsep fse(al_create_fs_entry("data/maps"));

    vivace::runtime_assert(al_fs_entry_exists(fse.get()), "data/maps was not found");
    vivace::runtime_assert(al_open_directory(fse.get()), "ensure data/maps is a directory");
  
    std::vector<std::string> entries;
    fsep read;
    
    while ((read = fsep(al_read_directory(fse.get()))))
    {
        std::string fname(al_get_fs_entry_name(read.get()));
        const std::string suffix(".tmx");

        if (fname.size() > suffix.size() && fname.compare(fname.size() - suffix.size(), suffix.size(), suffix) == 0)
            entries.push_back(fname);
    }
    vivace::runtime_assert(!entries.empty(), "no map found");
    vivace::runtime_assert(al_close_directory(fse.get()), "can't close data/maps directory");


    auto m = std::make_unique<Menu>(parent);
    std::sort(entries.begin(), entries.end());
    for (auto& s : entries)
    {
        auto n = m.get();
        m->add_entry(s, [s, g, n](){ n->disable_parent();
                                     n->escape();
                                     g->load_game(s);});
    }
    m->activate(false);
    return m;
}
