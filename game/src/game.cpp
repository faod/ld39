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

using namespace vivace;

#include <exception>
#include <iostream>
#include <string>

using namespace std;

#include <glm/vec2.hpp>

using namespace glm;

class Game: public Object {
public:
	virtual void update()
	{
		; // TODO
	};

	virtual void draw()
	{
		al_clear_to_color(al_map_rgb(0, 255, 0));
		al_flip_display();
	};

	virtual void handle(const ALLEGRO_EVENT& event)
	{
		float vecval = 0.;
		switch (event.type) {
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			cout << "Bye!" << endl;
			throw 1;

		// Handle Keyboard events
		case ALLEGRO_EVENT_KEY_DOWN:
			vecval = 1.;
		case ALLEGRO_EVENT_KEY_UP:
			switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				speed.y = vecval;
				break;
			case ALLEGRO_KEY_DOWN:
				speed.y = -vecval;
				break;
			case ALLEGRO_KEY_LEFT:
				speed.x = -vecval;
				break;
			case ALLEGRO_KEY_RIGHT:
				speed.x = vecval;
				break;
			}
			break;
		default:
			cout << "Unknown event type " << event.type << endl;
		}
	};

private:
	int color{0};
	vec2 position{0, 0}; // position in meter
	vec2 speed{0, 0}; // in meter per seconds
};

int main(void) {
	try {
		Vivace engine("GAME_NAME"s, ""s);
		unique_ptr<ALLEGRO_DISPLAY, al_display_deleter> dsp(al_create_display(800, 600));
		Game game;
		Basic_loop main_loop(0, game);
		main_loop.register_event_source(al_get_display_event_source(dsp.get()));
		main_loop.run();
	}
	catch (std::exception ex) {
		std::cerr << ex.what() << std::endl;
	}
}
