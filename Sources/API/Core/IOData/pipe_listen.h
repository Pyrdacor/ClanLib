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
*/


#pragma once

#include <memory>

namespace clan
{
/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

class Event;
class PipeConnection;
class PipeListen_Impl;

/// \brief Pipe listen class.
class PipeListen
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a PipeListen
	///
	/// \param name = String
	PipeListen(const std::string &name);

	~PipeListen();

/// \}
/// \name Attributes
/// \{

public:
#ifdef WIN32
	/// \brief Returns the OS handle for the pipe.
	HANDLE get_handle() const;
#else

	/// \brief Get Handle
	///
	/// \return handle
	int get_handle() const;
#endif

/// \}
/// \name Operations
/// \{

public:
	/// \brief Begin listening for pipe connection attempts.
	Event begin_accept();

	/// \brief Establish connection.
	PipeConnection complete_accept();

	/// \brief Stop listening for connection attempts..
	void cancel_accept();

	/// \brief Block until a connection is established.
	PipeConnection accept();

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<PipeListen_Impl> impl;
/// \}
};

}

/// \}
