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

#include <vivace/vivace.hpp>

using std::shared_ptr;

namespace vivace {

Sprite_animation::Sprite_animation(bool loop): loop(loop)
{
	// Initialises our member variables (segfaults otherwise)
	reset();
}

void Sprite_animation::update(double delta_t)
{
	current_frame_delta_t += delta_t;

	if (current_frame_delta_t  > frames[current_frame_index].duration) {
		current_frame_delta_t -= frames[current_frame_index].duration;

		if (current_frame_index == frames.size() - 1) {
			if (loop) {
				current_frame_index = 0;
			}
		}
		else {
			current_frame_index++;
		}
	}
}

shared_ptr<ALLEGRO_BITMAP> Sprite_animation::current()
{
	if (!frames.empty()) {
		return frames[current_frame_index].sprite;
	}
	return shared_ptr<ALLEGRO_BITMAP>(NULL);
}

void Sprite_animation::reset()
{
	current_frame_index = 0;
	current_frame_start_date = al_get_time();
	current_frame_delta_t = 0.;
}

void Sprite_animation::add_frame(shared_ptr<ALLEGRO_BITMAP> frame_bitmap, double frame_duration)
{
	frames.push_back(Sprite_animation::Frame{frame_bitmap, frame_duration});
}

}
