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

#ifndef V_LOOP_HPP
#define V_LOOP_HPP
#pragma once

#include <memory>

namespace vivace {

// Simplest monothreaded loop, calls `update` on each Objects before `draw`
class Basic_loop {
public:
	// framerate: timespan between two invocations
	// object: invoked at a fixed rate
	// throws a Vivace_Error exception if framerate < 0.
	Basic_loop(double framerate, Object &object);
	// You have no interest in copying this object
	Basic_loop(Basic_loop& v) = delete;

	// get the framerate
	double get_framerate();
	// set the framerate
	// throws a Vivace_Error exception if framerate < 0.
	void set_framerate(double framerate);

	// Add and event source, must not be a Timer event source
	void register_event_source(ALLEGRO_EVENT_SOURCE* ev_source);
	// Blocking member function (until an exception is thrown deeper in the call stack)
	void run();

protected:
	// Constructor parameter
	Object& object;
	// To manage event sources
	const std::unique_ptr<ALLEGRO_EVENT_QUEUE, al_event_queue_deleter> ev_queue;
	// To generate timer events at a fixed rate
	const std::unique_ptr<ALLEGRO_TIMER, al_timer_deleter> timer;

private:
	void run_timed();
	void run_untimed();
};

} // namespace vivace

#endif // V_LOOP_HPP