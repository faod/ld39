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

#include <memory>


// Loads and render a Tiled map (TMX) into an ALLEGRO_BITMAP
class map {
public:
	// Call once to initialize TMX library
	static void initialize();

	// Creates a new instance, immediately loads and renders map at given location
	// Throws a Vivace_error if cannot load/render map
	map(const char *map_location);

	const ALLEGRO_BITMAP* get_bitmap() const;

private:
	std::list<std::vector<std::pair<int, int>>> tracks;
	std::unique_ptr<ALLEGRO_BITMAP, al_bitmap_deleter> bitmap;
};

#endif /* MAP_HPP */
