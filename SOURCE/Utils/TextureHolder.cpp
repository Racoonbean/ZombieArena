#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::instance = nullptr;

TextureHolder::TextureHolder()
{
	assert(instance == nullptr);
	instance = this;
}

Texture& TextureHolder::GetTexture(string filename)
{
	// 1.filename key map 에 있나 없나
	// 2. 있을때->map에서 찾아서 반환
	// 3. 없을때->만들어서 map에 insert

	auto& map = instance->mapTexture;
	auto it = map.find(filename);
	if (it == map.end())
	{
		auto& texture = map[filename];
		texture.loadFromFile(filename);
	}
	return map[filename];

}
