#pragma once
namespace LearnOpenGL
{
    class Texture2D
    {
    public:
        Texture2D(const char* path, bool flip_vertically = false);
        ~Texture2D();

        void Bind() const;
        void Activate() const;
        void SetParameters(uint32_t wrap_s, uint32_t wrap_t, uint32_t min_filter, uint32_t mag_filter);

        unsigned int id() const { return id_; };
        unsigned int width() const { return width_; };
        unsigned int height() const { return height_; };

    private:
        unsigned int id_;
        unsigned int width_, height_, nr_channels_;
        const char* path_;
    };
}