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
#include <glm/gtc/random.hpp>

#include "competitors.hpp"

ALLEGRO_BITMAP* Competitor::bmp = NULL;

Competitor::Competitor(std::function<glm::dvec3(float)> position_provider, int track, float pos, float speed, float percent16px):
	position_provider(position_provider),
	track(track),
	pos(pos),
	speed(speed),
	percent16px(percent16px)
{
	if (!bmp) {
		bmp = reinterpret_cast<ALLEGRO_BITMAP*>(al_img_loader("data/maillot_blend.png"));
		vivace::runtime_assert(bmp, "Could not load bitmap data/maillot_blend.png");
	}
	color = al_map_rgb(glm::linearRand(0,255), glm::linearRand(0,255), glm::linearRand(0,255));
}

float Competitor::get_speed() const
{
	return speed;
}

float Competitor::get_pos() const
{
	return pos;
}

int Competitor::get_track() const
{
	return track;
}

void Competitor::draw()
{
	glm::dvec3 pos2d = position_provider(pos);
	al_draw_tinted_rotated_bitmap(bmp, color,
		5.5f, 14.5f,
		pos2d.x, pos2d.y,
		-pos2d.p, 0);
#ifndef NDEBUG
	al_draw_rectangle(
		pos2d.x-5.f, pos2d.y-5.f,
		pos2d.x+5.f, pos2d.y+5.f,
		al_map_rgb(0, 0, 255),
		1.f);
#endif
}

void Competitor::update(double delta_t, std::vector<Competitor>& track_comps)
{
	float new_pos = pos + speed * delta_t;
	// Check collision
	for (Competitor& competitor: track_comps) {
		if (competitor.pos != pos && competitor.speed != speed) { // cannot collision with itself
			if (competitor.pos <= pos + percent16px/16.f*11.f && competitor.pos > pos) {
				if (competitor.speed < speed) {
					new_pos = pos + competitor.speed * delta_t;
				}
			}
		}
	}
	pos = new_pos;
}

// -----------------------------------------------------------------------------

void Competitors::draw()
{
	for (auto& competitors_array: competitors) {
		for (Competitor& competitor: competitors_array) {
			competitor.draw();
		}
	}
}

void Competitors::update(double delta_t)
{
	for (auto& competitors_array: competitors) {
		for (Competitor& competitor: competitors_array) {
			competitor.update(delta_t, competitors_array);
		}
	}
}

const Competitor* Competitors::get_colliding_front(int track, float pos, float min_dist) const
{
	auto& competitors_array = competitors[track];
	for (const Competitor& competitor: competitors_array) {
		if (competitor.get_pos() <= pos + min_dist && competitor.get_pos() >= pos) {
			return &competitor;
		}
	}
	return NULL;
}

const Competitor* Competitors::get_colliding(int track, float pos, float min_dist) const
{
	auto& competitors_array = competitors[track];
	for (const Competitor& competitor: competitors_array) {
		if (competitor.get_pos() <= pos + min_dist && competitor.get_pos() >= pos - min_dist) {
			return &competitor;
		}
	}
	return NULL;
}

void Competitors::set_level(map* level)
{
	competitors.clear();
	vivace::runtime_assert(level, "Competitors: level must not be null");
	this->level = level;

	track& track_ref = level->tracks[2];

	for (std::vector<track>::size_type it=0; it<level->tracks.size(); it++) {

		std::vector<Competitor> competitors_array;

		track& track_it = level->tracks[it];
		int spawn_number = static_cast<int>((.1 * track_it.length) / 16);
		std::cout << "Spawning " << spawn_number << " competitors" << std::endl;

		double g_unit = track_it.get_16px_percentage();
		for (; spawn_number > 0; spawn_number--) {
			float spawn_pos = glm::linearRand(2*g_unit, .5);
			competitors_array.push_back(
				Competitor(
					[&track_ref, &track_it](float pos){ return track_it.get_position(track_ref, pos); },
					it,
					spawn_pos,
					g_unit * glm::linearRand(1.5f, 3.f),
					g_unit
				)
			);
			std::cout << "Spawned competitor at " << spawn_pos*100 << "%" << std::endl;	
		}
		competitors.push_back(competitors_array);
	}
}
