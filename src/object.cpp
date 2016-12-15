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

namespace vivace {

void Object_aggregator::add(Object& object)
{
	objects.push_front(object);
}

void Object_aggregator::update()
{
	for (auto& object: objects) {
		object.get().update();
	}
}

void Object_aggregator::draw()
{
	for (auto& object: objects) {
		object.get().draw();
	}
}

void Object_aggregator::handle(const ALLEGRO_EVENT& event)
{
	for (auto& object: objects) {
		object.get().handle(event);
	}
}

// ----

void Object_split_aggregator::draw()
{
	for (auto& function: draw_functions) {
		function();
	}
}

void Object_split_aggregator::update()
{
	for (auto& function: update_functions) {
		function();
	}
}

void Object_split_aggregator::handle(const ALLEGRO_EVENT& event)
{
	for (auto& function: handle_functions) {
		function(event);
	}
}


void Object_split_aggregator::add_draw_front(std::function<void()> draw_func)
{
	draw_functions.push_front(draw_func);
}

void Object_split_aggregator::add_draw_back(std::function<void()> draw_func)
{
	draw_functions.push_back(draw_func);
}

void Object_split_aggregator::add_update(std::function<void()> update_func)
{
	update_functions.push_front(update_func);
}

void Object_split_aggregator::add_handle(std::function<void(const ALLEGRO_EVENT&)> handle_func)
{
	handle_functions.push_front(handle_func);
}

} // namespace vivace
