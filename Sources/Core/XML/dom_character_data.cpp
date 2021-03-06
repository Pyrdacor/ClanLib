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

#include "Core/precomp.h"
#include "API/Core/XML/dom_character_data.h"
#include "API/Core/XML/dom_document.h"
#include "dom_node_generic.h"
#include "dom_document_generic.h"
#include "dom_tree_node.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// DomCharacterData construction:

DomCharacterData::DomCharacterData()
{
}

DomCharacterData::DomCharacterData(const std::shared_ptr<DomNode_Impl> &impl) : DomNode(impl)
{
}

DomCharacterData::DomCharacterData(DomDocument &doc, unsigned short node_type)
: DomNode(doc, node_type)
{
}

DomCharacterData::~DomCharacterData()
{
}

/////////////////////////////////////////////////////////////////////////////
// DomCharacterData attributes:

unsigned long DomCharacterData::get_length()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// DomCharacterData operations:

DomString DomCharacterData::substring_data(unsigned long offset, unsigned long count)
{
	if (impl)
		return get_node_value().substr(offset, count);
	return DomString();
}

void DomCharacterData::append_data(const DomString &arg)
{
	if (impl)
	{
		DomDocument_Impl *doc = static_cast<DomDocument_Impl *>(impl->owner_document.lock().get());
		DomString value = impl->get_tree_node()->get_node_value();
		impl->get_tree_node()->node_value += arg;
	}
}

void DomCharacterData::insert_data(unsigned long offset, const DomString &arg)
{
	if (impl)
	{
		DomDocument_Impl *doc = static_cast<DomDocument_Impl *>(impl->owner_document.lock().get());
		DomString value = impl->get_tree_node()->get_node_value();
		if (offset > value.length())
			offset = value.length();
		impl->get_tree_node()->node_value = value.substr(0, offset) + arg + value.substr(offset);
	}
}

void DomCharacterData::delete_data(unsigned long offset, unsigned long count)
{
	if (impl)
	{
		DomString value = impl->get_tree_node()->get_node_value();
		if (offset > value.length())
			offset = value.length();
		if (offset + count > value.length())
			count = value.length() - offset;
		if (count == 0)
			return;

		if (count < value.length())
		{
			value = value.substr(0, offset) + value.substr(offset + count);
		}
		else
		{
			value = DomString();
		}
		impl->get_tree_node()->node_value = value;
	}
}

void DomCharacterData::replace_data(unsigned long offset, unsigned long count, const DomString &arg)
{
	delete_data(offset, count);
	insert_data(offset, arg);
}

/////////////////////////////////////////////////////////////////////////////
// DomCharacterData implementation:

}
