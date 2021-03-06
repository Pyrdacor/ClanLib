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

#include "span_layout_view_impl.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/brush.h"
#include "API/Display/Font/glyph_metrics.h"
#include "line_metrics.h"
#include <algorithm>
#include "API/Display/Render/shared_gc_data.h"

namespace clan
{
	SpanLayoutViewImpl::SpanLayoutViewImpl()
	{
	}

	void SpanLayoutViewImpl::set_text_alignment(TextAlignment text_alignment)
	{
		alignment = text_alignment;
	}

	void SpanLayoutViewImpl::clear()
	{
		objects.clear();
		text.clear();
	}

	void SpanLayoutViewImpl::add_text(const std::string &more_text, const Font &font, const Colorf &color, int id)
	{
		SpanObject object;
		object.type = SpanObjectType::text;
		object.start = text.length();
		object.end = object.start + more_text.length();
		object.font = font;
		object.color = color;
		object.id = id;
		objects.push_back(object);
		text += more_text;
	}

	void SpanLayoutViewImpl::add_subview(const std::shared_ptr<View> &view, float baseline_offset, int id)
	{
		SpanObject object;
		object.type = SpanObjectType::view;
		object.view = view;
		object.baseline_offset = baseline_offset;
		object.id = id;
		object.start = text.length();
		object.end = object.start + 1;
		objects.push_back(object);
		text += "*";
	}

	void SpanLayoutViewImpl::set_last_baseline_offset(float baseline_offset)
	{
		objects.back().baseline_offset = baseline_offset;
	}

	void SpanLayoutViewImpl::remove_subview(const std::shared_ptr<View> &view)
	{
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			if (it->view == view)
			{
				objects.erase(it);
				break;
			}
		}
	}

	void SpanLayoutViewImpl::render_content(Canvas &canvas, float width)
	{
		float y = 0.0f;
		size_t obj_start = 0;
		size_t text_start = 0;
		while (obj_start < objects.size())
		{
			SpanLineMetrics metrics = find_line_metrics(obj_start, text_start, width);

			float x = 0.0f;

			if (alignment == TextAlignment::center)
			{
				x = (width - metrics.advance_width) * 0.5f;
			}
			else if (alignment == TextAlignment::right)
			{
				x = width - metrics.advance_width;
			}

			size_t obj_end = metrics.text_end != 0 ? metrics.object_end + 1 : metrics.object_end;
			for (size_t obj_index = metrics.object_start; obj_index < obj_end; obj_index++)
			{
				SpanObject &object = objects[obj_index];

				if (object.type == SpanObjectType::text)
				{
					size_t obj_text_start = obj_index == metrics.object_start ? metrics.text_start : object.start;
					size_t obj_text_end = obj_index == metrics.object_end ? metrics.text_end : object.end;
					size_t obj_text_length = obj_text_end - obj_text_start;

					std::string obj_text = text.substr(obj_text_start, obj_text_length);

					GlyphMetrics advance = object.font.get_glyph_metrics(canvas, obj_text);

					canvas.text(object.font, x, y + metrics.ascent + object.baseline_offset, obj_text, object.color);

					x += advance.advance.width;
				}
				else if (object.type == SpanObjectType::view)
				{
					x += object.view->geometry().margin_box().get_width();
				}
			}

			y += metrics.ascent + metrics.descent;

			obj_start = metrics.object_end;
			text_start = metrics.text_end;
		}
	}

	void SpanLayoutViewImpl::layout_views(float width)
	{
		float y = 0.0f;
		size_t obj_start = 0;
		size_t text_start = 0;
		Canvas canvas = SharedGCData::get_resource_canvas();
		while (obj_start < objects.size())
		{
			SpanLineMetrics metrics = find_line_metrics(obj_start, text_start, width);

			float x = 0.0f;

			if (alignment == TextAlignment::center)
			{
				x = (width - metrics.advance_width) * 0.5f;
			}
			else if (alignment == TextAlignment::right)
			{
				x = width - metrics.advance_width;
			}

			size_t obj_end = metrics.text_end != 0 ? metrics.object_end + 1 : metrics.object_end;
			for (size_t obj_index = metrics.object_start; obj_index < obj_end; obj_index++)
			{
				SpanObject &object = objects[obj_index];

				if (object.type == SpanObjectType::text)
				{
					size_t obj_text_start = obj_index == metrics.object_start ? metrics.text_start : object.start;
					size_t obj_text_end = obj_index == metrics.object_end ? metrics.text_end : object.end;
					size_t obj_text_length = obj_text_end - obj_text_start;

					GlyphMetrics advance = object.font.get_glyph_metrics(canvas, text.substr(obj_text_start, obj_text_length));
					x += advance.advance.width;
				}
				else if (object.type == SpanObjectType::view)
				{
					float obj_x = x;
					float obj_y = y + metrics.ascent + object.baseline_offset;

					float obj_width = object.view->get_preferred_width();
					float obj_height = object.view->get_preferred_height(obj_width);
					float obj_baseline_offset = object.view->get_first_baseline_offset(obj_width);

					if (obj_baseline_offset == 0.0f) // Hmm, do we need get_first_baseline_offset to be able to return that there is no baseline?
						obj_baseline_offset = obj_height;

					obj_width += object.view->style.margin_left();
					obj_width += object.view->style.border_left();
					obj_width += object.view->style.padding_left();
					obj_width += object.view->style.margin_right();
					obj_width += object.view->style.border_right();
					obj_width += object.view->style.padding_right();

					obj_height += object.view->style.margin_top();
					obj_height += object.view->style.border_top();
					obj_height += object.view->style.padding_top();
					obj_height += object.view->style.margin_bottom();
					obj_height += object.view->style.border_bottom();
					obj_height += object.view->style.padding_bottom();

					obj_baseline_offset += object.view->style.margin_top();
					obj_baseline_offset += object.view->style.border_top();
					obj_baseline_offset += object.view->style.padding_top();

					obj_y -= obj_baseline_offset;

					object.view->set_geometry(ViewGeometry::from_margin_box(object.view->style, Rectf::xywh(obj_x, obj_y, obj_width, obj_height)));

					x += obj_width;
				}
			}

			y += metrics.ascent + metrics.descent;

			obj_start = metrics.object_end;
			text_start = metrics.text_end;
		}
	}

	float SpanLayoutViewImpl::get_preferred_width()
	{
		float x = 0.0f;
		Canvas canvas = SharedGCData::get_resource_canvas();
		for (SpanObject &object : objects)
		{
			if (object.type == SpanObjectType::text)
			{
				GlyphMetrics advance = object.font.get_glyph_metrics(canvas, text.substr(object.start, object.end - object.start));
				x += advance.advance.width;
			}
			else if (object.type == SpanObjectType::view)
			{
				x += object.view->get_preferred_width();
			}
		}
		return x;
	}

	float SpanLayoutViewImpl::get_preferred_height(float width)
	{
		float y = 0.0f;
		size_t obj_start = 0;
		size_t text_start = 0;
		while (obj_start < objects.size())
		{
			SpanLineMetrics metrics = find_line_metrics(obj_start, text_start, width);
			y += metrics.ascent + metrics.descent;

			obj_start = metrics.object_end;
			text_start = metrics.text_end;
		}
		return y;
	}

	float SpanLayoutViewImpl::get_first_baseline_offset(float width)
	{
		return find_line_metrics(0, 0, width).ascent;
	}

	float SpanLayoutViewImpl::get_last_baseline_offset(float width)
	{
		float y = 0.0f;
		size_t obj_start = 0;
		size_t text_start = 0;
		while (true)
		{
			SpanLineMetrics metrics = find_line_metrics(obj_start, text_start, width);

			if (metrics.object_end == objects.size())
				return y + metrics.ascent;

			y += metrics.ascent + metrics.descent;
			obj_start = metrics.object_end;
			text_start = metrics.text_end;
		}
	}

	SpanLineMetrics SpanLayoutViewImpl::find_line_metrics(size_t obj_start, size_t text_start, float width)
	{
		float line_ascent = 0.0f;
		float line_descent = 0.0f;
		float x = 0.0f;
		Canvas canvas = SharedGCData::get_resource_canvas();

		for (size_t obj_index = obj_start; obj_index < objects.size(); obj_index++)
		{
			SpanObject &object = objects[obj_index];

			float obj_ascent = 0.0f;
			float obj_descent = 0.0f;
			float obj_advance_width = 0.0f;

			size_t obj_text_start = obj_index == obj_start ? text_start : object.start;
			size_t obj_text_length = object.end - obj_text_start;

			if (object.type == SpanObjectType::text)
			{
				std::string obj_text = text.substr(obj_text_start, obj_text_length);

				obj_advance_width = object.font.get_glyph_metrics(canvas, obj_text).advance.width;

				LineMetrics object_line(object.font);
				obj_ascent = object_line.ascent;
				obj_descent = object_line.descent;

				if (x + obj_advance_width > width)
				{
					size_t char_clip_pos = object.font.clip_from_left(canvas, obj_text, width - x);

					size_t word_clip_pos = char_clip_pos;
					if (word_clip_pos > 0)
					{
						word_clip_pos = obj_text.find_last_of(" \t", word_clip_pos - 1);
						word_clip_pos = word_clip_pos == std::string::npos ? 0 : word_clip_pos + 1;
					}

					if (x == 0.0f && word_clip_pos == 0)
					{
						word_clip_pos = char_clip_pos;
					}

					if (x != 0.0f || word_clip_pos > 0)
					{
						obj_text_length = word_clip_pos;
						obj_advance_width = object.font.get_glyph_metrics(canvas, text.substr(obj_text_start, obj_text_length)).advance.width;

						obj_ascent = std::max(obj_ascent + object.baseline_offset, 0.0f);
						obj_descent = std::max(obj_descent - object.baseline_offset, 0.0f);

						line_ascent = std::max(line_ascent, obj_ascent);
						line_descent = std::max(line_descent, obj_descent);

						x += obj_advance_width;

						return SpanLineMetrics(obj_start, obj_index, text_start, obj_text_start + obj_text_length, line_ascent, line_descent, x);
					}
				}
			}
			else if (object.type == SpanObjectType::view)
			{
				float obj_width = object.view->get_preferred_width();
				float obj_height = object.view->get_preferred_height(obj_width);
				float obj_baseline_offset = object.view->get_first_baseline_offset(obj_width);

				if (obj_baseline_offset == 0.0f) // Hmm, do we need get_first_baseline_offset to be able to return that there is no baseline?
					obj_baseline_offset = obj_height;

				obj_width += object.view->style.margin_left();
				obj_width += object.view->style.border_left();
				obj_width += object.view->style.padding_left();
				obj_width += object.view->style.margin_right();
				obj_width += object.view->style.border_right();
				obj_width += object.view->style.padding_right();

				obj_height += object.view->style.margin_top();
				obj_height += object.view->style.border_top();
				obj_height += object.view->style.padding_top();
				obj_height += object.view->style.margin_bottom();
				obj_height += object.view->style.border_bottom();
				obj_height += object.view->style.padding_bottom();

				obj_baseline_offset += object.view->style.margin_top();
				obj_baseline_offset += object.view->style.border_top();
				obj_baseline_offset += object.view->style.padding_top();

				obj_ascent = obj_baseline_offset;
				obj_descent = obj_height - obj_baseline_offset;

				obj_advance_width = obj_width;

				if (x != 0.0f && x + obj_advance_width > width)
				{
					return SpanLineMetrics(obj_start, obj_index, text_start, obj_text_start + obj_text_length, line_ascent, line_descent, x);
				}
			}

			obj_ascent = std::max(obj_ascent + object.baseline_offset, 0.0f);
			obj_descent = std::max(obj_descent - object.baseline_offset, 0.0f);

			line_ascent = std::max(line_ascent, obj_ascent);
			line_descent = std::max(line_descent, obj_descent);

			x += obj_advance_width;
		}

		return SpanLineMetrics(obj_start, objects.size(), text_start, 0, line_ascent, line_descent, x);
	}
}
