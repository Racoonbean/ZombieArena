#pragma once
#include "SFML/Graphics.hpp"
#include <random>

using namespace sf;

enum class Pivots 
{
	//L, C, R
	//T, C, R
	LT, CT, RT,
	LC, CC, RC,
	LB, CB, RB
};

class Utils 
{
private:
	static std::random_device rd;
	static std::mt19937 gen;

public:
	static void SetOrigin(Shape& shape, Pivots preset);
	static void SetOrigin(Text& text, Pivots preset);
	static void SetOrigin(Sprite& sprite, Pivots preset);
	static void SetOrigin(Transformable& tr, FloatRect bounds, Pivots preset);

	static int RandomRange(int min, int excludeMax);	//포함안되는 max사용해야함

	static float GetLength(const Vector2f& vector);
	static Vector2f Normalize(Vector2f vector);
	static float GetAngle(const Vector2f& from, const Vector2f& to);

};