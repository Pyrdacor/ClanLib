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
**    Mark Page
*/

#pragma once


#pragma once

#include <memory>

namespace clan
{
/// \addtogroup clanCore_Crypto clanCore Crypto
/// \{

class DataBuffer;
class TLSClient_Impl;

/// \brief Transport Layer Security (TLS) client class
class TLSClient
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a TLS client
	TLSClient();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Return a pointer to decrypted data available for consumption.
	const void *get_decrypted_data() const;

	/// \brief Returns how much decrypted data is available.
	int get_decrypted_data_available() const;

	/// \brief Return a pointer to encrypted data available for consumption.
	const void *get_encrypted_data() const;

	/// \brief Returns how much encrypted data is available.
	int get_encrypted_data_available() const;
/// \}

/// \name Operations
/// \{
public:
	/// \brief Adds data to be encrypted.
	int encrypt(const void *data, int size);

	/// \brief Adds data to be decrypted.
	int decrypt(const void *data, int size);

	/// \brief Marks decrypted data as consumed.
	void decrypted_data_consumed(int size);

	/// \brief Marks encrypted data as consumed.
	void encrypted_data_consumed(int size);
/// \}

/// \name Implementation
/// \{
private:
	std::shared_ptr<TLSClient_Impl> impl;
/// \}
};

}

/// \}
