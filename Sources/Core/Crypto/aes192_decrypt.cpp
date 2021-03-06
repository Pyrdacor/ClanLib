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
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/Crypto/aes192_decrypt.h"
#include "API/Core/System/databuffer.h"
#include "aes192_decrypt_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// AES192_Decrypt Construction:

AES192_Decrypt::AES192_Decrypt()
: impl(std::make_shared<AES192_Decrypt_Impl>())
{
}

/////////////////////////////////////////////////////////////////////////////
// AES192_Decrypt Attributes:

DataBuffer AES192_Decrypt::get_data() const
{
	return impl->get_data();
}

/////////////////////////////////////////////////////////////////////////////
// AES192_Decrypt Operations:

void AES192_Decrypt::reset()
{
	impl->reset();
}

void AES192_Decrypt::set_iv(const unsigned char iv[16])
{
	impl->set_iv(iv);
}

void AES192_Decrypt::set_key(const unsigned char key[24])
{
	impl->set_key(key);
}

void AES192_Decrypt::set_padding(bool value, bool use_pkcs7)
{
	impl->set_padding(value, use_pkcs7);
}


void AES192_Decrypt::add(const void *data, int size)
{
	impl->add(data, size);
}

void AES192_Decrypt::add(const DataBuffer &data)
{
	add(data.get_data(), data.get_size());
}

bool AES192_Decrypt::calculate()
{
	return impl->calculate();
}

/////////////////////////////////////////////////////////////////////////////
// AES192_Decrypt Implementation:

}
