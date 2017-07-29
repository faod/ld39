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
#include <list>

#include <vivace/allegro.hpp>

namespace vivace {

// Base class for "things" handled by mainloops
class Object {
public:
	// Virtual dtor so Object can be properly overloaded
    virtual ~Object() = default;
    // Update this object
	virtual void update(double delta_t) = 0;
	// Draw this object
	virtual void draw() = 0;
	// Handle event (not a TIMER event)
	virtual void handle(const ALLEGRO_EVENT& event) = 0;
};

// An implementation that delegates to multiple instances
class Object_aggregator: public virtual Object {
public:
	Object_aggregator() {};
	Object_aggregator(Object_aggregator& v) = delete;

	// Add given object to a forward_list (insert at head)
	// Objects will be visited in the reverse order
	virtual void add(Object& object);

	virtual void update(double delta_t);
	virtual void draw();
	virtual void handle(const ALLEGRO_EVENT& event);

protected:
	std::forward_list<std::reference_wrapper<Object>> objects;
};

// Delegates to multiple functions
class Object_split_aggregator: public virtual Object {
public:
	Object_split_aggregator() {};
	Object_split_aggregator(Object_split_aggregator& v) = delete;

	virtual void update(double delta_t);
	virtual void draw();
	virtual void handle(const ALLEGRO_EVENT& event);

	// Add a draw() function to a list
	// At front (first visited) or at back (last visited)
	void add_draw_front(std::function<void()> draw_func);
	void add_draw_back(std::function<void()> draw_func);
	// Add an update() function to a forward_list
	void add_update(std::function<void(double)> update_func);
	// Add a handle() function to a forward_list
	void add_handle(std::function<void(const ALLEGRO_EVENT&)> handle_func);

protected:
	std::list<std::function<void()>> draw_functions;
	std::forward_list<std::function<void(double)>> update_functions;
	std::forward_list<std::function<void(const ALLEGRO_EVENT&)>> handle_functions;
};

class Object_full_aggregator: public Object_split_aggregator, public Object_aggregator {
public:
    Object_full_aggregator() = default;

    virtual void update(double delta_t);
    virtual void draw();
    virtual void handle(const ALLEGRO_EVENT& event);

    virtual void add(Object& object);
};

} // namespace vivace

#endif // V_OBJECT_HPP
