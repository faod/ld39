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

/*
 *
 * OBJECT
 * 
 */

Object::Object() : activated_(true)
{
}

bool Object::activated() const
{
    return activated_;
}

void Object::activate(bool activate)
{
    activated_ = activate;
}

void Object::update(double delta_t)
{
    if (activated())
        update_impl(delta_t);
}

void Object::draw()
{
    if (activated())
        draw_impl();
}

void Object::handle(const ALLEGRO_EVENT& event)
{
    if (activated())
        handle_impl(event);
}
/*
 *
 * DRAWABLE
 *
 */
Drawable::Drawable(std::function<void()> draw) : draw_(draw)
{}

void Drawable::update_impl(double delta_t)
{
    (void) delta_t;
}

void Drawable::handle_impl(const ALLEGRO_EVENT& event)
{
    (void) event;
}

void Drawable::draw_impl()
{
    draw_();
}

/*
 *
 * UPDATABLE
 *
 */
Updatable::Updatable(std::function<void(double)> update) : update_(update)
{}

void Updatable::update_impl(double delta_t)
{
    update_(delta_t);
}

void Updatable::handle_impl(const ALLEGRO_EVENT& event)
{
    (void) event;
}

void Updatable::draw_impl()
{}

/*
 *
 * LISTENER
 *
 */
Listener::Listener(std::function<void(const ALLEGRO_EVENT&)> listen) : listen_(listen)
{}

void Listener::update_impl(double delta_t)
{
    (void) delta_t;
}

void Listener::handle_impl(const ALLEGRO_EVENT& event)
{
    listen_(event);
}

void Listener::draw_impl()
{}

/*
 *
 * OBJECT AGGREGRATOR
 *
 */

void Object_aggregator::add(Object& object)
{
	objects.push_front(object);
}

void Object_aggregator::remove(Object& object)
{
    auto it = std::begin(objects);
    while (it != std::end(objects))
    {
        if (&(it->get()) == &object)
        {
            objects.erase(it);
            return;
        }
    }
}

void Object_aggregator::update_impl(double delta_t)
{
	for (auto& object: objects) {
        object.get().update(delta_t);
	}
}

void Object_aggregator::draw_impl()
{
	for (auto& object: objects) {
        object.get().draw();
	}
}

void Object_aggregator::handle_impl(const ALLEGRO_EVENT& event)
{
	for (auto& object: objects) {
        object.get().handle(event);
	}
}

} // namespace vivace
