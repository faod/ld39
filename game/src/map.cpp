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
void* al_img_loader(const char *path)
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

map::map(const char* map_location) : location_(map_location)
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

// Direction _MUST_ be normalized!
static double compute_rotation(glm::dvec2 direction)
{
	glm::dvec2 orient(0, -1);
	double angle = glm::acos(glm::dot(direction, orient));
	return angle * (((direction.x * orient.y) >= 0)? 1: -1);
}

static glm::vec2 compute_direction(glm::dvec2 from, glm::vec2 to)
{
	return glm::normalize(glm::dvec2(to.x - from.x, to.y - from.y));
}

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

	// Compute length of all segments, distance_to_next and direction_to_next and map_rotation for each segment
	this->length = 0.;
	for (int it=1; it<points_len; it++) {
		glm::dvec2 &point = this->points.at(it);
		glm::dvec2 &prev  = this->points.at(it - 1);

		double dist = glm::distance<double>(point, prev);
		distance_to_next.push_back(dist);
		length += dist;

		glm::dvec2 dir = compute_direction(prev, point);
		direction_to_next.push_back(dir);

		map_rotation.push_back(compute_rotation(dir));
#ifndef NDEBUG
		std::cout << "segment [" << it-1 << ", " << it << "] length=" << distance_to_next[it-1] << " map rotation=" << map_rotation[it-1] << std::endl;
#endif
	}
#ifndef NDEBUG
	std::cout << "length of track = " << length << std::endl;
#endif
}

double track::get_16px_percentage()
{
	return 1./(length/16.);
}

// returns 0.0 at next-16px to next, 0.5 at next, 0.5 at previous, 1.0 at previous+16px
static double compute_proximity_ratio(glm::dvec2 pos, glm::dvec2 prev_point, glm::dvec2 next_point)
{
	double dist_to_next   = glm::distance(pos, next_point);
	double dist_from_prev = glm::distance(prev_point, pos);
	if (dist_to_next <= 16) {
		return 0.5 - dist_to_next/32.;
	}
	if (dist_from_prev <= 16) {
		return 0.5 + dist_from_prev/32.;
	}
	return 1.;
}

#define PI 3.141592653589793

// to smooth rotation according to the proximity ratio
// prev_angle must be the map rotation engle of the segment BEFORE the proximal point
// next_angle must be the map rotation engle of the segment AFTER  the proximal point
static double apply_proximity_ratio(double prox_ratio, double prev_angle, double next_angle)
{
	double rotation = (next_angle - prev_angle);
	if (rotation > PI)
	{
		rotation = PI - rotation;
	}
	if (rotation < -PI)
	{
		rotation = -rotation - PI;
	}
	return prev_angle + prox_ratio * rotation;
}

glm::dvec3 track::get_position(track& reference, double completion_percentage) const
{
	if (completion_percentage <= 0.) {
		return glm::dvec3(points.front(), map_rotation.front());
	}
	if (completion_percentage >= 1.) {
		return glm::dvec3(points.back(), map_rotation.back());
	}

	double real_length = completion_percentage * reference.length;

	std::vector<double>::size_type last = reference.distance_to_next.size() - 1;
	for (std::vector<double>::size_type it=0; it<=last; it++) {
		double dist = reference.distance_to_next[it];
		if (dist < real_length) {
			real_length -= dist;
		}
		else {
			glm::dvec2 point = points[it];
			if (real_length < 1e-6)
			{
				if (it == 0 || it == last) {
					return glm::dvec3(point, map_rotation[it]);
				}
				// Smooth angle
				double angle = apply_proximity_ratio(0.5, map_rotation[it-1], map_rotation[it]);
				return glm::dvec3(point, angle);
			}
			double segment_completion = real_length / dist; // percentage of completion of current segment on reference track
			glm::dvec2 direction = direction_to_next[it];
			real_length = segment_completion * distance_to_next[it]; // apply %age of completion to same segment on this track
			glm::dvec2 position = point + (real_length * direction);

			// Smooth angle
			double angle = map_rotation[it];
			double prox_ratio = compute_proximity_ratio(position, point, points[it+1]);
			if (it < last && prox_ratio > 0. && prox_ratio < 0.5) {
				angle = apply_proximity_ratio(prox_ratio, map_rotation[it], map_rotation[it+1]);
			}
			else if (it > 0 && prox_ratio < 1. && prox_ratio >= 0.5) {
				angle = apply_proximity_ratio(prox_ratio, map_rotation[it-1], map_rotation[it]);
			}
			return glm::dvec3(position, angle);
		}
	}

	return glm::dvec3(points.back(), map_rotation.back());
}
