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

#include <memory>
#include <vivace/allegro.hpp>

ALLEGRO_FONT* debug_font()
{
    static std::unique_ptr<ALLEGRO_FONT, al_font_deleter> dbg_ft;
    if (!dbg_ft)
        dbg_ft = std::unique_ptr<ALLEGRO_FONT, al_font_deleter>(al_create_builtin_font());

    return dbg_ft.get();
}
