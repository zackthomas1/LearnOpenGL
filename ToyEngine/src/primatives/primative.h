#pragma once
namespace ToyEngine
{
	class Primative
	{
	public:
		virtual ~Primative() {}

		virtual void Draw() const = 0;

	protected:
		unsigned int vao_, vbo_, ebo_;
	};
}
