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
    Object();

    bool activated() const;
    void activate(bool activate);

    void update(double delta_t);
    void draw();
    void handle(const ALLEGRO_EVENT& event);
protected:
    // Update this object
	virtual void update_impl(double delta_t) = 0;
	// Draw this object
	virtual void draw_impl() = 0;
	// Handle event (not a TIMER event)
	virtual void handle_impl(const ALLEGRO_EVENT& event) = 0;
private:
    bool activated_;
};

// Can only draw a drawble
class Drawable: public virtual Object 
{
public:
    Drawable(std::function<void()> draw);

private:
    virtual void draw_impl() override;
    virtual void update_impl(double delta_t) override;
    virtual void handle_impl(const ALLEGRO_EVENT& event) override;
    std::function<void()> draw_;
};

//Can only update
class Updatable: public virtual Object
{
public:
    Updatable(std::function<void(double)> update);

private:
    virtual void draw_impl() override;
    virtual void update_impl(double delta_t) override;
    virtual void handle_impl(const ALLEGRO_EVENT& event) override;
    std::function<void(double)> update_;
};

//Can only capture events
class Listener: public virtual Object
{
public:
    Listener(std::function<void(const ALLEGRO_EVENT&)> listen);

private:
    virtual void draw_impl() override;
    virtual void update_impl(double delta_t) override;
    virtual void handle_impl(const ALLEGRO_EVENT& event) override;
    std::function<void(const ALLEGRO_EVENT&)> listen_;
};

// An implementation that delegates to multiple instances
class Object_aggregator: public virtual Object {
public:
	Object_aggregator() {};
	Object_aggregator(Object_aggregator& v) = delete;

	// Add given object to a forward_list (insert at head)
	// Objects will be visited in the reverse order
	void add(Object& object);
    void remove(Object& object);
protected:
	virtual void update_impl(double delta_t) override;
	virtual void draw_impl() override;
	virtual void handle_impl(const ALLEGRO_EVENT& event) override;

private:
	std::list<std::reference_wrapper<Object>> objects;
};

} // namespace vivace

#endif // V_OBJECT_HPP
