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

#ifndef V_ALLEGRO_HPP
#define V_ALLEGRO_HPP
#pragma once

// Deleter template using Alleg's destroy functions (for unique_ptr, shared_ptr, ...);
template <class al_type, class ret_type, ret_type (*del_func)(al_type*)> class al_deleter {
public:
	void operator()(al_type* ptr)
	{
		del_func(ptr);
	}
};

#include <allegro5/allegro.h>

class al_config_deleter: public al_deleter<ALLEGRO_CONFIG, void, al_destroy_config> {};
class al_display_deleter: public al_deleter<ALLEGRO_DISPLAY, void, al_destroy_display> {};
class al_event_queue_deleter: public al_deleter<ALLEGRO_EVENT_QUEUE, void, al_destroy_event_queue> {};
class al_file_deleter: public al_deleter<ALLEGRO_FILE, bool, al_fclose> {}; // returns bool, TODO: create function template to suppress returned value of a function
class al_fs_entry_deleter: public al_deleter<ALLEGRO_FS_ENTRY, void, al_destroy_fs_entry> {};
class al_bitmap_deleter: public al_deleter<ALLEGRO_BITMAP, void, al_destroy_bitmap> {};
class al_path_deleter: public al_deleter<ALLEGRO_PATH, void, al_destroy_path> {};
class al_shader_deleter: public al_deleter<ALLEGRO_SHADER, void, al_destroy_shader> {};
class al_thread_deleter: public al_deleter<ALLEGRO_THREAD, void, al_destroy_thread> {};
class al_cond_deleter: public al_deleter<ALLEGRO_COND, void, al_destroy_cond> {};
class al_mutex_deleter: public al_deleter<ALLEGRO_MUTEX, void, al_destroy_mutex> {};
class al_timer_deleter: public al_deleter<ALLEGRO_TIMER, void, al_destroy_timer> {};
class al_ustr_deleter: public al_deleter<ALLEGRO_USTR, void, al_ustr_free> {};

#include <allegro5/allegro_audio.h>

class al_voice_deleter: public al_deleter<ALLEGRO_VOICE, void, al_destroy_voice> {};
class al_sample_deleter: public al_deleter<ALLEGRO_SAMPLE, void, al_destroy_sample> {};
class al_sample_instance_deleter: public al_deleter<ALLEGRO_SAMPLE_INSTANCE, void, al_destroy_sample_instance> {};
class al_mixer_deleter: public al_deleter<ALLEGRO_MIXER, void, al_destroy_mixer> {};
class al_audio_stream_deleter: public al_deleter<ALLEGRO_AUDIO_STREAM, void, al_destroy_audio_stream> {};
//class al_audio_recorder_deleter: public al_deleter<ALLEGRO_AUDIO_RECORDER, al_destroy_audio_recorder> {}; // not found??

#include <allegro5/allegro_color.h>

#include <allegro5/allegro_font.h>

class al_font_deleter: public al_deleter<ALLEGRO_FONT, void, al_destroy_font> {};

#include <allegro5/allegro_memfile.h>

#include <allegro5/allegro_primitives.h>

class al_vertex_decl_deleter: public al_deleter<ALLEGRO_VERTEX_DECL, void, al_destroy_vertex_decl> {};
class al_vertex_buffer_deleter: public al_deleter<ALLEGRO_VERTEX_BUFFER, void, al_destroy_vertex_buffer> {};
class al_index_buffer_deleter: public al_deleter<ALLEGRO_INDEX_BUFFER, void, al_destroy_index_buffer> {};

#ifdef WANT_IMAGE
#include <allegro5/allegro_image.h>
#endif

#ifdef WANT_ACODEC
#include <allegro5/allegro_acodec.h>
#endif

#ifdef WANT_TTF
#include <allegro5/allegro_ttf.h>
#endif

#ifdef WANT_PHYSFS
#include <allegro5/allegro_physfs.h>
#endif

#ifdef WANT_VIDEO
#include <allegro5/allegro_video.h>
class al_video_deleter: public al_deleter<ALLEGRO_VIDEO, void, al_close_video> {};
#endif

#ifdef WANT_DIALOG
#include <allegro5/allegro_native_dialog.h>
class al_filechooser_deleter: public al_deleter<ALLEGRO_FILECHOOSER, void, al_destroy_native_file_dialog> {};
class al_textlog_deleter: public al_deleter<ALLEGRO_TEXTLOG, void, al_close_native_text_log> {};
class al_menu_deleter: public al_deleter<ALLEGRO_MENU, void, al_destroy_menu> {};
#endif

ALLEGRO_FONT* debug_font();

#endif // V_ALLEGRO_HPP
