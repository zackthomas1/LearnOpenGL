#include "pch.h"
#include "texture_2d.h"

#include <glad/glad.h>
#include "stb_image.h"

namespace LearnOpenGL
{
	Texture2D::Texture2D(const char* path, bool flip_vertically)
	{
		path_ = path;

		// generate texture sampler
		glGenTextures(1, &id_);
		Bind();

		SetParameters(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

		// load image
		int width, height, nr_channels;
		stbi_set_flip_vertically_on_load(flip_vertically);
		stbi_uc* data = stbi_load(path_, &width, &height, &nr_channels, 0);
		width_ = width;
		height_ = height;
		nr_channels_ = nr_channels;

		GLenum internal_format = 0, data_format = 0;
		if (nr_channels_ == 3) {
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}
		else if (nr_channels_ == 4)
		{
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		}

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, data_format, width_, height_, 0, data_format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			TY_CORE_ERROR("Failed to load texture");
		}
		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &id_);
	}

	void Texture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, id_);
	}

	void Texture2D::Activate() const
	{
		glActiveTexture(GL_TEXTURE0 + (id_)-1); // activate texture unit before binding
		Bind();
	}

	void Texture2D::SetParameters(uint32_t wrap_s, uint32_t wrap_t, uint32_t min_filter, uint32_t mag_filter)
	{
		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	}
}