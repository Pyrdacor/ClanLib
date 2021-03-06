/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Harry Storbacka
*/


#pragma once

#include <memory>

namespace clan
{
/// \addtogroup clanDisplay_Input clanDisplay Input
/// \{

class InputDevice;
class InputContext_Impl;

/// \brief InputContext
class InputContext
{
/// \name Construction
/// \{

public:
	/// \brief Constructs an input context.
	InputContext();

	~InputContext();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the number of keyboards available.
	int get_keyboard_count() const;

	/// \brief Returns the number of mice available.
	int get_mouse_count() const;

	/// \brief Returns the number of joysticks available.
	int get_joystick_count() const;

	/// \brief Returns the number of tablets available.
	int get_tablet_count() const;

	/// \brief Returns the input device for the specified keyboard.
	InputDevice &get_keyboard(int keyboard = 0);

	/// \brief Returns the input device for the specified mouse.
	InputDevice &get_mouse(int mouse = 0);

	/// \brief Returns the input device for the specified joystick.
	InputDevice &get_joystick(int joystick = 0);

	/// \brief Returns the input device for the specified tablet.
	InputDevice &get_tablet(int tablet = 0);

	/// \brief Returns the input device with the given devicename
	InputDevice &get_device(const std::string &device_name);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Removes all devices from this input context.
	void clear();

	/// \brief Adds a new keyboard to the input context.
	void add_keyboard(const InputDevice keyboard);

	/// \brief Adds a new mouse to the input context.
	void add_mouse(const InputDevice mouse);

	/// \brief Adds a new joystick to the input context.
	void add_joystick(const InputDevice joystick);

	/// \brief Adds a new tablet to the input context.
	void add_tablet(const InputDevice tablet);

	/// \brief Process and dispatch messages to signals.
	void process_messages();

	/// \brief Stop anyone accessing this input context
	///
	/// This should is only called internally by the display window destructor
	void dispose();

	/// \brief Has dispose() been called on this input context
	///
	/// \return true = Has been disposed
	bool is_disposed() const;

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<InputContext_Impl> impl;

/// \}
};

}

/// \}
