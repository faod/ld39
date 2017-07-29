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

#ifndef MAP_HPP
#define MAP_HPP
#pragma once

#include <vivace/vivace.hpp>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

extern "C" {
    void* al_img_loader(const char *path);
}
// Track (line where a cyclist ride)
class track {
public:
	track(double **points, int points_len, double off_x, double off_y);

	// (x, y, p)   p (Phi) = map rotation angle in rads
	glm::dvec3 getPosition(double completion_percentage) const;
	double get16pxPercentage();

	std::vector<glm::dvec2> points;
	double length; // Sum of length of all segments
};

// Loads and render a Tiled map (TMX) into an ALLEGRO_BITMAP
class map {
public:
	// Call once to initialize TMX library
	static void initialize();

	// Creates a new instance, immediately loads and renders map at given location
	// Throws a Vivace_error if cannot load/render map
	map(const char *map_location);

	std::vector<track> tracks;
	std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter> bitmap;
	unsigned int width, height;
	ALLEGRO_COLOR bg_color;
};
#endif /* MAP_HPP */
