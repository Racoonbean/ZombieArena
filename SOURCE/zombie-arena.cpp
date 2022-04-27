#include <SFML/Graphics.hpp>
#include "../SOURCE/Utils/Utils.h"
#include "Utils/InputManager.h"
#include "player/Player.h"
#include <iostream>
#include "Utils/TextureHolder.h"
#include "zombie/Zombie.h"
#include "../SOURCE/Utils/PickUp.h"

using namespace sf;

int CreateBackground(VertexArray& va, IntRect arena)
{
    const int TILE_SIZE = 50;
    const int TILE_TYPES = 3;
    const int VERTEX_IN_QUAD = 4;

    int cols = arena.width / TILE_SIZE;
    int rows = arena.height / TILE_SIZE;

    va.setPrimitiveType(Quads);
    va.resize(cols*rows*VERTEX_IN_QUAD);

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int index = r * cols + c;

            float x = c * TILE_SIZE;
            float y = r * TILE_SIZE;

            int vertexIndex = index * VERTEX_IN_QUAD;
            va[vertexIndex + 0].position = Vector2f(x, y);
            va[vertexIndex + 1].position = Vector2f(x + TILE_SIZE, y);
            va[vertexIndex + 2].position = Vector2f(x + TILE_SIZE, y + TILE_SIZE);
            va[vertexIndex + 3].position = Vector2f(x, y + TILE_SIZE);

            bool outline = (c == 0 || r == 0 || c == cols - 1 || r == rows - 1);
            int texIndex = outline ? TILE_TYPES : Utils::RandomRange(0, TILE_TYPES);

            float offset = texIndex * TILE_SIZE;

            va[vertexIndex + 0].texCoords = Vector2f(0.f, offset);
            va[vertexIndex + 1].texCoords = Vector2f(TILE_SIZE, offset);
            va[vertexIndex + 2].texCoords = Vector2f(TILE_SIZE, offset + TILE_SIZE);
            va[vertexIndex + 3].texCoords = Vector2f(0.f, offset + TILE_SIZE);
        }
    }
    return cols * rows;
}

void CreateZombies(std::vector<Zombie*>& zombies, int count, IntRect arena)
{
    for (auto v : zombies)
    {
        delete v;
    }   //순회하면서 삭제 

    zombies.clear();

    int offset = 25;
    int minX = arena.left + offset;
    int maxX = arena.width - offset;
    int minY = arena.top + offset;
    int maxY = arena.height - offset;

    for (int i = 0; i < count; ++i)
    {
        int x = Utils::RandomRange(minX, maxX + 1);
        int y = Utils::RandomRange(minY, maxY + 1);
        ZombieTypes type = (ZombieTypes)Utils::RandomRange(0, (int)ZombieTypes::COUNT);

        Zombie* zombie = new Zombie();
        zombie->Spawn(x, y, type);

        zombies.push_back(zombie);

    }
}




int main()
{
    TextureHolder textureHolder;


    Vector2i resolution;
    resolution.x = VideoMode::getDesktopMode().width;
    resolution.y = VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Arena!",Style::Default);

    window.setMouseCursorVisible(false);

    Sprite spriteCrosshair;
    Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
    spriteCrosshair.setTexture(textureCrosshair);
    Utils::SetOrigin(spriteCrosshair, Pivots::CC);

    View mainView(FloatRect(0, 0, resolution.x, resolution.y));
    View uiView(FloatRect(0, 0, resolution.x, resolution.y));



    InputManager::Init();

    IntRect arena;
    arena.width = 500;
    arena.height = 500;

    PickUp ammoPickup(PickupTypes::Ammo);
    PickUp healthPickup(PickupTypes::Health);

    ammoPickup.SetArena(arena);
    healthPickup.SetArena(arena);

    std::list<PickUp*> items;
    items.push_back(&ammoPickup);
    items.push_back(&healthPickup);

    Player player;
    player.Spawn(arena, resolution, 0.f);

    std::vector<Zombie*>zombies;;
    CreateZombies(zombies, 10, arena);

    Clock clock;
    Time playTime;



    Texture& texBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

    VertexArray tileMap;
    CreateBackground(tileMap, arena);


    while (window.isOpen())
    {
        Time dt = clock.restart();
        playTime += dt;


        InputManager::ClearInput();
    
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            InputManager::ProcessInput(event);
        }

//Update//////////////////////////////////////////////////////////////////////////////////////////////////////////
        InputManager::Update(dt.asSeconds(),window, mainView);

        spriteCrosshair.setPosition(InputManager::GetMouseWorldPosition());

        player.Update(dt.asSeconds());
        mainView.setCenter(player.GetPosition());   // !



        for (auto zombie : zombies)
        {
            zombie->Update(dt.asSeconds(), player.GetPosition());
        }

        ammoPickup.Update(dt.asSeconds());
        healthPickup.Update(dt.asSeconds());

//Collision ////////////////////////////////////////////////////////////////////////////////////////////////////////

        player.UpdateCollision(zombies);
        for (auto zombie : zombies)
        {
            if (zombie->UpdateCollision(playTime, player))
            {
                break;
            }
        }

        player.UpdateCollision(items);

//Draw ///////////////////////////////////////////////////////////////////////////////////////////////////////
        window.clear();
        window.setView(mainView);   //!
        window.draw(tileMap,&texBackground);

        if (ammoPickup.IsSpawned())
        {
            window.draw(ammoPickup.GetSprite());
        }
        if (healthPickup.IsSpawned())
        {
            window.draw(healthPickup.GetSprite());
        }

        for (auto zombie : zombies)
        {
            window.draw(zombie->GetSprite());
        }

       // window.draw(player.GetSprite());
        player.Draw(window);
        window.draw(spriteCrosshair);

        window.setView(uiView);
        //UI Draws


        window.display();
    }

    return 0;
}