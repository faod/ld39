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

#include <iostream>

#include "map.hpp"

#include "tmx/tmx.h"

extern "C" {

// Loads a bitmap and return pointer, used by TMX
static void* al_img_loader(const char *path)
{
	ALLEGRO_BITMAP *res    = NULL;
	ALLEGRO_PATH   *alpath = NULL;

	if (!(alpath = al_create_path(path))) return NULL;

	al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA);
	res = al_load_bitmap(al_path_cstr(alpath, ALLEGRO_NATIVE_PATH_SEP));

	al_destroy_path(alpath);

	return reinterpret_cast<void*> (res);
}

} // extern "C"

static int gid_extract_flags(unsigned int gid)
{
	int res = 0;

	if (gid & TMX_FLIPPED_HORIZONTALLY) res |= ALLEGRO_FLIP_HORIZONTAL;
	if (gid & TMX_FLIPPED_VERTICALLY)   res |= ALLEGRO_FLIP_VERTICAL;
	/* FIXME allegro has no diagonal flip */
	return res;
}

static unsigned int gid_clear_flags(unsigned int gid)
{
	return gid & TMX_FLIP_BITS_REMOVAL;
}

static void draw_layer(tmx_map *map, tmx_layer *layer)
{
	unsigned long i, j;
	unsigned int gid, x, y, w, h, flags;
	float op;
	tmx_tileset *ts;
	tmx_image *im;
	ALLEGRO_BITMAP *tileset;
	op = layer->opacity;
	for (i = 0; i < map->height; i++) {
		for (j = 0; j < map->width; j++) {
			gid = gid_clear_flags(layer->content.gids[(i * map->width) + j]);
			if (map->tiles[gid] != NULL) {
				ts = map->tiles[gid]->tileset;
				im = map->tiles[gid]->image;
				x  = map->tiles[gid]->ul_x;
				y  = map->tiles[gid]->ul_y;
				w  = ts->tile_width;
				h  = ts->tile_height;
				if (im) {
					tileset = (ALLEGRO_BITMAP*) im->resource_image;
				}
				else {
					tileset = (ALLEGRO_BITMAP*) ts->image->resource_image;
				}
				flags = gid_extract_flags(layer->content.gids[(i * map->width) + j]);
				al_draw_tinted_bitmap_region(tileset, al_map_rgba_f(op, op, op, op), x, y, w, h, j * ts->tile_width, i * ts->tile_height, flags);
			}
		}
	}
}

static void draw_all_layers(tmx_map *map, tmx_layer *layers)
{
	while (layers) {
		if (layers->visible) {
			if (layers->type == L_GROUP) {
				draw_all_layers(map, layers->content.group_head);
			}
			else if (layers->type == L_IMAGE) {
				if (layers->opacity < 1.) {
					float op = layers->opacity;
					al_draw_tinted_bitmap((ALLEGRO_BITMAP*) layers->content.image->resource_image, al_map_rgba_f(op, op, op, op), 0, 0, 0);
				}
				al_draw_bitmap((ALLEGRO_BITMAP*) layers->content.image->resource_image, 0, 0, 0);
			}
			else if (layers->type == L_LAYER) {
				draw_layer(map, layers);
			}
		}
		layers = layers->next;
	}
}

ALLEGRO_COLOR int_to_al_color(int color)
{
	unsigned char r, g, b;

	r = (color >> 16) & 0xFF;
	g = (color >>  8) & 0xFF;
	b = (color)       & 0xFF;

	return al_map_rgb(r, g, b);
}

void map::initialize()
{
	tmx_img_load_func = al_img_loader;
	tmx_img_free_func = (void (*)(void*))al_destroy_bitmap;
}

map::map(const char* map_location)
{
	tmx_map *loaded_map = tmx_load(map_location);
	vivace::runtime_assert(loaded_map, tmx_strerr());

	width = loaded_map->width * loaded_map->tile_width;
	height = loaded_map->height * loaded_map->tile_height;

	// Render all graphic layers to a bitmap
	bitmap = std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter>(al_create_bitmap(width, height));
	ALLEGRO_BITMAP *restore = al_get_target_bitmap();
	al_set_target_bitmap(bitmap.get());
	draw_all_layers(loaded_map, loaded_map->ly_head);
	al_set_target_bitmap(restore);

	// Copy tracks
	tmx_layer *obj_layer = loaded_map->ly_head->next;
	vivace::runtime_assert(obj_layer->type == L_OBJGR, "second layer is not an objgr layer");
	tmx_object *pos = obj_layer->content.objgr->head;
	while (pos) {
		vivace::runtime_assert(pos->obj_type == OT_POLYLINE, "object is not a polyline");
		track track_item(pos->content.shape->points, pos->content.shape->points_len, pos->x, pos->y);
		tracks.insert(tracks.begin(), track_item);
		pos = pos->next;
	}

	bg_color = int_to_al_color(loaded_map->backgroundcolor);
	tmx_map_free(loaded_map);
};

track::track(double** points, int points_len, double off_x, double off_y)
{
	for (int it=0; it<points_len; it++) {
		double x, y;
		x = off_x + points[it][0];
		y = off_y + points[it][1];
#ifndef NDEBUG
		std::cout << "tracks point " << it+1 << " (x, y) = " << x << ", " << y << std::endl;
#endif
		this->points.push_back(glm::dvec2(x, y));
	}

	// Compute length
	this->length = 0.;
	for (int it=1; it<points_len; it++) {
		glm::dvec2 &point = this->points.at(it);
		glm::dvec2 &prev  = this->points.at(it - 1);
		length += glm::distance<double>(point, prev);
	}
#ifndef NDEBUG
	std::cout << "length of track = " << length << std::endl;
#endif
}

double track::get16pxPercentage()
{
	return 1./(length/16.);
}

glm::dvec2 track::getPosition(double completion_percentage) const
{
	if (completion_percentage <= 0.) {
		return points.front();
	}
	if (completion_percentage >= 1.) {
		return points.back();
	}

	double real_length = completion_percentage * length;

	for (std::vector<glm::dvec2>::size_type it=1; it<points.size(); it++) {
		const glm::dvec2 &point = points.at(it);
		const glm::dvec2 &prev  = points.at(it - 1);
		double dist = glm::distance<double>(point, prev);
		if (dist < real_length) {
			real_length -= dist;
		}
		else {
			glm::dvec2 direction = glm::normalize(glm::dvec2(point.x - prev.x, point.y - prev.y));
			return prev + (real_length * direction);
		}
	}

	return points.back();
}
