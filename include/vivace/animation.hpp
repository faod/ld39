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

#ifndef V_ANIM_HPP
#define V_ANIM_HPP
#pragma once

#include <vector>
#include <memory>

namespace vivace {

// Manage sprite based animations
// FIXME would a Builder be a better way to construct a const Animation?
// A const Animation would be better as we use an interator in the internal state ...
class Sprite_animation {
public:
	// A frame of the animation
	struct Frame {
		std::shared_ptr<ALLEGRO_BITMAP> sprite;
		double duration;
	};

	Sprite_animation(bool loop = false);

	// Update the state of the animation with the elapsed time since last call, delta_t is in seconds
	void update(double delta_t);
	// Get the current frame
	std::shared_ptr<ALLEGRO_BITMAP> current();
	// Restart animation
	void reset();
	// Add a frame to the animation (order is kept), duration is in seconds
	void add_frame(std::shared_ptr<ALLEGRO_BITMAP> frame_bitmap, double frame_duration);

private:
	const bool loop;
	unsigned int current_frame_index;
	// Absolute start time of the current frame (al_get_time())
	double current_frame_start_date;
	double current_frame_delta_t;
	std::vector<Frame> frames;
};

}

#endif // V_ANIM_HPP
