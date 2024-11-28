#pragma once
class Texture2D
{
public:
	Texture2D(const char* path, bool flip_vertically = false);
	~Texture2D();

	void Bind() const;
	void Activate() const;
	void SetParameters(uint32_t wrap_s, uint32_t wrap_t, uint32_t min_filter, uint32_t mag_filter);

	uint32_t id() const { return id_; };
	uint32_t width() const { return width_; };
	uint32_t height() const { return height_; };

private:
	uint32_t id_;
	uint32_t width_, height_, nr_channels_;
	const char* path_;
};