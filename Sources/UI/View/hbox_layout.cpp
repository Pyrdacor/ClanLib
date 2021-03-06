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

#include "UI/precomp.h"
#include "hbox_layout.h"
#include <algorithm>
#include <cmath>

namespace clan
{
	float HBoxLayout::get_preferred_width(View *view)
	{
		if (!view->style.is_width_auto())
			return view->style.width();

		float width = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style.is_static() && !subview->hidden())
			{
				width += subview->style.margin_left();
				width += subview->style.border_left();
				width += subview->style.padding_left();
				if (subview->style.is_flex_basis_auto())
					width += subview->get_preferred_width();
				else
					width += subview->style.flex_basis();
				width += subview->style.padding_right();
				width += subview->style.border_right();
				width += subview->style.margin_right();
			}
		}
		return width;
	}

	float HBoxLayout::get_preferred_height(View *view, float width)
	{
		if (!view->style.is_height_auto())
			return view->style.height();

		// Calculate flex properties:

		float total_grow_factor = 0.0f;
		float total_shrink_factor = 0.0f;
		float basis_width = 0.0f;
		float noncontent_width = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style.is_static() && !subview->hidden())
			{
				noncontent_width += subview->style.margin_left();
				noncontent_width += subview->style.border_left();
				noncontent_width += subview->style.padding_left();
				noncontent_width += subview->style.padding_right();
				noncontent_width += subview->style.border_right();
				noncontent_width += subview->style.margin_right();

				total_grow_factor += subview->style.flex_grow();
				total_shrink_factor += subview->style.flex_shrink();

				if (subview->style.is_flex_basis_auto())
					basis_width += subview->get_preferred_width();
				else
					basis_width += subview->style.flex_basis();
			}
		}

		float free_space = width - noncontent_width - basis_width;

		// Layout using flex properties:

		float height = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style.is_static() && !subview->hidden())
			{
				float subview_width = subview->style.flex_basis();
				if (subview->style.is_flex_basis_auto())
					subview_width = subview->get_preferred_width();

				if (free_space < 0.0f && total_shrink_factor != 0.0f)
					subview_width += subview->style.flex_shrink() * free_space / total_shrink_factor;
				else if (free_space > 0.0f && total_grow_factor != 0.0f)
					subview_width += subview->style.flex_grow() * free_space / total_grow_factor;

				subview_width = std::round(subview_width); // To do: this way of rounding may cause the total width to go beyond the available content width

				float margin_box_height = 0.0f;
				margin_box_height += subview->style.margin_top();
				margin_box_height += subview->style.border_top();
				margin_box_height += subview->style.padding_top();
				margin_box_height += subview->get_preferred_height(subview_width);
				margin_box_height += subview->style.padding_bottom();
				margin_box_height += subview->style.border_bottom();
				margin_box_height += subview->style.margin_bottom();
				height = clan::max(height, margin_box_height);
			}
		}
		return height;
	}

	float HBoxLayout::get_first_baseline_offset(View *view, float width)
	{
		const auto &subviews = view->subviews();
		for (auto it = subviews.begin(); it != subviews.end(); ++it)
		{
			if (!(*it)->hidden())
				return (*it)->get_first_baseline_offset(width);
		}
		return 0.0f;
	}

	float HBoxLayout::get_last_baseline_offset(View *view, float width)
	{
		const auto &subviews = view->subviews();
		for (auto it = subviews.rbegin(); it != subviews.rend(); ++it)
		{
			if (!(*it)->hidden())
				return (*it)->get_last_baseline_offset(width);
		}
		return 0.0f;
	}

	void HBoxLayout::layout_subviews(View *view)
	{
		// Calculate flex properties:

		float total_grow_factor = 0.0f;
		float total_shrink_factor = 0.0f;
		float basis_width = 0.0f;
		float noncontent_width = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style.is_static() && !subview->hidden())
			{
				noncontent_width += subview->style.margin_left();
				noncontent_width += subview->style.border_left();
				noncontent_width += subview->style.padding_left();
				noncontent_width += subview->style.padding_right();
				noncontent_width += subview->style.border_right();
				noncontent_width += subview->style.margin_right();

				total_grow_factor += subview->style.flex_grow();
				total_shrink_factor += subview->style.flex_shrink();

				if (subview->style.is_flex_basis_auto())
					basis_width += subview->get_preferred_width();
				else
					basis_width += subview->style.flex_basis();
			}
		}

		float free_space = view->geometry().content.get_width() - noncontent_width - basis_width;

		// Layout using flex properties:

		float x = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style.is_static() && !subview->hidden())
			{
				float top_noncontent = 0.0f;
				top_noncontent += subview->style.margin_top();
				top_noncontent += subview->style.border_top();
				top_noncontent += subview->style.padding_top();

				float bottom_noncontent = 0.0f;
				bottom_noncontent += subview->style.margin_bottom();
				bottom_noncontent += subview->style.border_bottom();
				bottom_noncontent += subview->style.padding_bottom();

				float subview_height = view->geometry().content.get_height() - top_noncontent - bottom_noncontent;
				if (subview_height < 0.0f)
				{
					bottom_noncontent = 0.0f;
					subview_height = view->geometry().content.get_height() - top_noncontent;
					if (subview_height < 0.0f)
					{
						top_noncontent = 0.0f;
						subview_height = view->geometry().content.get_height();
					}
				}

				float subview_width = subview->style.flex_basis();
				if (subview->style.is_flex_basis_auto())
					subview_width = subview->get_preferred_width();

				if (free_space < 0.0f && total_shrink_factor != 0.0f)
					subview_width += subview->style.flex_shrink() * free_space / total_shrink_factor;
				else if (free_space > 0.0f && total_grow_factor != 0.0f)
					subview_width += subview->style.flex_grow() * free_space / total_grow_factor;

				subview_width = std::round(subview_width); // To do: this way of rounding may cause the total width to go beyond the available content width

				x += subview->style.margin_left();
				x += subview->style.border_left();
				x += subview->style.padding_left();

				subview->set_geometry(ViewGeometry::from_content_box(subview->style, Rectf::xywh(x, top_noncontent, subview_width, subview_height)));

				x += subview_width;
				x += subview->style.padding_right();
				x += subview->style.border_right();
				x += subview->style.margin_right();

				subview->layout_subviews();
			}
		}
	}
}
