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

#ifndef COMPETITORS_HPP
#define COMPETITORS_HPP
#pragma once

#include <vivace/object.hpp>
#include <glm/glm.hpp>

#include "map.hpp"

// A competitor (bot)
class Competitor {
public:
	Competitor(std::function<glm::dvec3(float)> position_provider, int track, float pos, float speed);

	float get_pos();
    int get_track();

	void draw();
	void update(double delta_t);

private:
	std::function<glm::dvec3(float)> position_provider;
	int track;
	float pos, speed;
	ALLEGRO_COLOR color;

	static ALLEGRO_BITMAP* bmp;
};

// -----------------------------------------------------------------------------

// All competitors
class Competitors {
public:
	// Does nothing
	Competitors() = default;
	// Sets the level and generate competitors for it
	void set_level(map* level);
	// Draw competitors
	void draw();
	// Update competitors
	void update(double delta_t);

private:
	map* level;
	std::vector<std::vector<Competitor> > competitors;
};

#endif // COMPETITORS_HPP
