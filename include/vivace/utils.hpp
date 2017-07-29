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

#ifndef V_UTILS_HPP
#define V_UTILS_HPP
#pragma once

#include <string>

namespace vivace {

// If first parameter is `false`, throws a Vivace_Error with given message
void inline runtime_assert(bool test, const std::string& message)
{
	if (!test) {
		throw new Vivace_Error(message);
	}
}

// We are working with C libraries and RAII is not possible
// This class emulates Java's finally block
class finally
{
public:
	finally(const std::function<void(void)>& functor) : functor(functor) {}

	~finally()
	{
		functor();
	}
private:
    std::function<void(void)> functor;
};

} // namespace vivace

#endif // V_UTILS_HPP
