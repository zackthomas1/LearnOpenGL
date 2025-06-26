#pragma once
#include "primative.h"
namespace LearnOpenGL
{
    class Plane : public Primative
    {
    public:
        Plane();
        ~Plane();

        void Draw() const override;
    private:
        static const float vertices_[32];

        //  Element Buffer Object (EBO)
        static const unsigned int indices_[6];
    };
}