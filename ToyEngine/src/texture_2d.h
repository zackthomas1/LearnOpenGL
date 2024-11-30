#pragma once
namespace ToyEngine
{
	class Texture2D
	{
	public:
		Texture2D(const char* path, bool flip_vertically = false);
		~Texture2D();

		void Bind() const;
		void Activate() const;
		void SetParameters(unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filter, unsigned int mag_filter);

		unsigned int id() const { return id_; };
		unsigned int width() const { return width_; };
		unsigned int height() const { return height_; };

	private:
		unsigned int id_;
		unsigned int width_, height_, nr_channels_;
		const char* path_;
	};
}