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

#include <cassert>

#include <vivace/vivace.hpp>

using namespace std::literals::string_literals;

namespace vivace {

Basic_loop::Basic_loop(double framerate, Object& object):
	object(object),
	ev_queue(al_create_event_queue()),
	timer(framerate > 0 ? al_create_timer(framerate) : nullptr)
{
	runtime_assert(framerate >= 0., "Invalid framerate, must be greater than 0"s);
	runtime_assert(static_cast<bool>(ev_queue), "Could not create an event queue"s);
	runtime_assert(framerate == 0. || static_cast<bool>(timer), "Could not create a timer"s);
	// Timer is NULL if framerate == 0
	if (timer) {
		al_register_event_source(ev_queue.get(), al_get_timer_event_source(timer.get()));
	}
}

double Basic_loop::get_framerate()
{
	return al_get_timer_speed(timer.get());
}

void Basic_loop::set_framerate(double framerate)
{
	runtime_assert(framerate > 0., "Invalid framerate, must be greater than 0"s);
	al_set_timer_speed(timer.get(), framerate);
}

void Basic_loop::register_event_source(ALLEGRO_EVENT_SOURCE* ev_source)
{
	assert(ev_source);
	al_register_event_source(ev_queue.get(), ev_source);
}

void Basic_loop::run()
{
	if (timer) {
		run_timed();
	}
	else {
		run_untimed();
	}
}

// if timer != nullptr
void Basic_loop::run_timed()
{
	al_flush_event_queue(ev_queue.get());
	al_pause_event_queue(ev_queue.get(), false);
	al_start_timer(timer.get());
	finally f([&]{
			al_stop_timer(timer.get());
			al_pause_event_queue(ev_queue.get(), true);
			al_flush_event_queue(ev_queue.get());
	});

	ALLEGRO_EVENT_SOURCE* timer_evs = al_get_timer_event_source(timer.get());
	ALLEGRO_EVENT ev;
	for(;;)
	{
		al_wait_for_event(ev_queue.get(), &ev);
		if (ev.any.source == timer_evs) {
			object.update();
			object.draw();
		}
		else {
			object.handle(ev);
		}
	}
}

// if timer == nullptr
void Basic_loop::run_untimed()
{
	al_flush_event_queue(ev_queue.get());
	al_pause_event_queue(ev_queue.get(), false);
	finally f([&]{
			al_pause_event_queue(ev_queue.get(), true);
			al_flush_event_queue(ev_queue.get());
	});

	ALLEGRO_EVENT ev;
	for(;;)
	{
		// Warning: if the event queue is never empty or handle() is slow, risk of starvation
		if (al_get_next_event(ev_queue.get(), &ev)) {
			object.handle(ev);
		}
		else {
			object.update();
			object.draw();
		}
	}
}

} // namespace vivace
