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

#ifndef V_OBJECT_HPP
#define V_OBJECT_HPP
#pragma once

#include <forward_list>
#include <functional>

namespace vivace {

// Base class for "things" handled by mainloops
class Object {
public:
	// Update this object
	virtual void update() = 0;
	// Draw this object
	virtual void draw() = 0;
	// Handle event (not a TIMER event)
	virtual void handle(const ALLEGRO_EVENT& event) = 0;
};

// An implementation that delegates to multiple instances
class Object_aggregator: public Object {
public:
	Object_aggregator() {};
	Object_aggregator(Object_aggregator& v) = delete;

	void add(Object& object);

	virtual void update();
	virtual void draw();
	virtual void handle(const ALLEGRO_EVENT& event);

protected:
	std::forward_list<std::reference_wrapper<Object>> objects;
};

}

#endif // V_OBJECT_HPP