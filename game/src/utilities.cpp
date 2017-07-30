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

#include <vivace/vivace.hpp>
#include <string>

void draw_scaled_ustr(const ALLEGRO_FONT *font, ALLEGRO_COLOR color,
		float x, float y, float scale, int flags, const ALLEGRO_USTR *ustr)
{
	int v, w, h;
	al_get_ustr_dimensions(font, ustr, &v, &v, &w, &h);
	ALLEGRO_BITMAP *res = al_create_bitmap(w, h);
	vivace::runtime_assert(res, "failed to create bitmap");

	ALLEGRO_BITMAP *save = al_get_target_bitmap();
	al_set_target_bitmap(res);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_ustr(font, color, 0, 0, 0, ustr);
	al_set_target_bitmap(save);

	if (flags & ALLEGRO_ALIGN_CENTRE) {
		x = x - (w*scale)/2.;
	}
	else if (flags & ALLEGRO_ALIGN_RIGHT) {
		x = x - w*scale;
	}
	y -= h*scale/2.;

	al_draw_scaled_bitmap(res, 0, 0, w, h, x, y, w*scale, h*scale, 0);
#ifndef NDEBUG
	al_draw_rectangle(x, y, x + w*scale, y + h*scale, al_map_rgb(255, 0, 0), 1.);
#endif
	al_destroy_bitmap(res);
}

void draw_scaled_text(const ALLEGRO_FONT *font, ALLEGRO_COLOR color,
		float x, float y, float scale, int flags, const char *text)
{
	ALLEGRO_USTR_INFO info;
	draw_scaled_ustr(font, color, x, y, scale, flags, al_ref_cstr(&info, text));
}

void draw_scaled_string(const ALLEGRO_FONT *font, ALLEGRO_COLOR color,
		float x, float y, float scale, int flags, const std::string str)
{
	draw_scaled_text(font, color, x, y, scale, flags, str.c_str());
}
