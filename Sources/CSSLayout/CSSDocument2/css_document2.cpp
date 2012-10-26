/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/css_document2.h"
#include "css_document2_impl.h"

namespace clan
{

CSSDocument::CSSDocument()
: impl(new CSSDocument_Impl())
{
}

CSSDocument::~CSSDocument()
{
}

void CSSDocument::add_sheet(const std::string &filename, const std::string &base_uri)
{
	File file(filename, File::open_existing, File::access_read);
	add_sheet(file, base_uri);
}

void CSSDocument::add_sheet(IODevice &iodevice, const std::string &base_uri)
{
	CSSTokenizer tokenizer(iodevice);
	impl->read_stylesheet(tokenizer, base_uri);
}

CSSPropertyList CSSDocument::select(const DomElement &node, const std::string &pseudo_element)
{
	DomSelectNode select_node(node);
	return select(&select_node, pseudo_element);
}

CSSPropertyList CSSDocument::select(CSSSelectNode *node, const std::string &pseudo_element)
{
	std::vector<CSSRulesetMatch> rulesets = impl->select_rulesets(node, pseudo_element);
	CSSPropertyList properties;
	for (size_t i = rulesets.size(); i > 0; i--)
	{
		for (size_t j = rulesets[i-1].ruleset->properties.size(); j > 0; j--)
		{
			if (rulesets[i-1].ruleset->properties[j-1].is_important())
				properties.push_back(rulesets[i-1].ruleset->properties[j-1]);
		}
	}
	for (size_t i = rulesets.size(); i > 0; i--)
	{
		for (size_t j = rulesets[i-1].ruleset->properties.size(); j > 0; j--)
		{
			if (!rulesets[i-1].ruleset->properties[j-1].is_important())
				properties.push_back(rulesets[i-1].ruleset->properties[j-1]);
		}
	}
	return properties;
}

CSSPropertyList CSSDocument::get_style_properties(const std::string &style_string, const std::string &base_uri)
{
	CSSTokenizer tokenizer(style_string);
	CSSToken token;
	std::vector<CSSProperty> property_list;
	while (true)
	{
		tokenizer.read(token, true);
		if (token.type == CSSToken::type_ident)
		{
			std::string property_name = token.value;
			tokenizer.read(token, true);
			if (token.type == CSSToken::type_colon)
			{
				tokenizer.read(token, true);

				CSSProperty property;
				property.set_name(property_name);
				CSSDocument_Impl::read_property_value(tokenizer, token, property, base_uri);
				if (!property.get_value_tokens().empty())
					property_list.push_back(property);
			}
			else
			{
				bool end_of_scope = CSSDocument_Impl::read_end_of_statement(tokenizer, token);
				if (end_of_scope)
					break;
			}
		}
		else if (token.type == CSSToken::type_null)
		{
			break;
		}
	}

	CSSPropertyList properties;
	for (size_t i = property_list.size(); i > 0; i--)
		properties.push_back(property_list[i - 1]);

	return properties;
}

}
