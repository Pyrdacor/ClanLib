lib_LTLIBRARIES = libclan40GL.la

libclan40GL_la_SOURCES = \
setup_gl.cpp \
State/opengl_blend_state.cpp \
State/opengl_depth_stencil_state.cpp \
State/opengl_rasterizer_state.cpp \
GL3/gl3_pixel_buffer_provider.cpp \
GL3/gl3_frame_buffer_provider.cpp \
GL3/gl3_occlusion_query_provider.cpp \
GL3/gl3_standard_programs.cpp \
GL3/gl3_vertex_array_buffer_provider.cpp \
GL3/gl3_element_array_buffer_provider.cpp \
GL3/gl3_buffer_object_provider.cpp \
GL3/gl3_storage_buffer_provider.cpp \
GL3/gl3_graphic_context_provider.cpp \
GL3/gl3_uniform_buffer_provider.cpp \
GL3/gl3_render_buffer_provider.cpp \
GL3/gl3_texture_provider.cpp \
GL3/gl3_transfer_buffer_provider.cpp \
GL3/gl3_primitives_array_provider.cpp \
GL3/gl3_program_object_provider.cpp \
GL3/gl3_shader_object_provider.cpp \
opengl.cpp \
opengl_target.cpp \
precomp.cpp \
GL1/pbuffer.cpp \
GL1/gl1_frame_buffer_provider.cpp \
GL1/gl1_vertex_array_buffer_provider.cpp \
GL1/gl1_transfer_buffer_provider.cpp \
GL1/gl1_render_buffer_provider.cpp \
GL1/gl1_uniform_buffer_provider.cpp \
GL1/gl1_program_object_provider.cpp \
GL1/gl1_graphic_context_provider.cpp \
GL1/gl1_texture_provider.cpp \
GL1/gl1_primitives_array_provider.cpp \
opengl_target_provider.cpp \
opengl_graphic_context.cpp \
opengl_window_description.cpp

if WIN32
libclan40GL_la_SOURCES += \
WGL/opengl_creation_helper.cpp \
WGL/opengl_window_provider_wgl.cpp \
WGL/pbuffer_impl.cpp
else
libclan40GL_la_SOURCES += \
GLX/opengl_window_provider_glx.cpp \
GLX/pbuffer_impl.cpp
endif
libclan40GL_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanGL)

libclan40GL_la_CXXFLAGS=$(clanGL_CXXFLAGS) $(extra_CFLAGS_clanGL)

# EOF #
