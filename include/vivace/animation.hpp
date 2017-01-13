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

// Animation template
template<class T>
class Animation {
public:
	// A frame of the animation
	struct Frame {
		T data;
		double duration;
	};

	Animation(bool loop = false);

	// Update the state of the animation with the elapsed time since last call, delta_t is in seconds
	void update(double delta_t);
	// Get the current frame
	T current() const;
	// Restart animation
	void reset();
	// Add a frame to the animation (order is kept), duration is in seconds
	void add_frame(T frame_data, double frame_duration);

private:
	const bool loop;
	unsigned int current_frame_index;
	// Absolute start time of the current frame (al_get_time())
	double current_frame_start_date;
	double current_frame_delta_t;
	std::vector<Frame> frames;
};

// --- Implementation ---

template<class T>
Animation<T>::Animation(bool loop): loop(loop)
{
	// Initialises our member variables (segfaults otherwise)
	reset();
}

template<class T>
void Animation<T>::update(double delta_t)
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

template<class T>
T Animation<T>::current() const
{
	if (!frames.empty()) {
		return frames[current_frame_index].data;
	}
	throw Vivace_Error("Animation contains no frame");
}

template<class T>
void Animation<T>::reset()
{
	current_frame_index = 0;
	current_frame_start_date = al_get_time();
	current_frame_delta_t = 0.;
}

template<class T>
void Animation<T>::add_frame(T frame_data, double frame_duration)
{
	frames.push_back(Animation::Frame{frame_data, frame_duration});
}

}

#endif // V_ANIM_HPP
