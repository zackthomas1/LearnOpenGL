#pragma once
#include "primative.h"
namespace LearnOpenGL
{
    class Cube : public Primative
    {
    public:
        Cube();
        ~Cube();

        void Draw() const override;
    private:
        static const float vertices_[288];
    };
}