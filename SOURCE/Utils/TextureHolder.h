#pragma once
#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class TextureHolder
{
private:
	map<string, Texture>mapTexture;
	static TextureHolder* instance;

public:
	TextureHolder();

	static Texture& GetTexture(string filename);


};

