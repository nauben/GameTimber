#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    const int   SCREEN_WIDTH                    = 1920;
    const int   SCREEN_HEIGHT                   = 1080;
    const float TREE_HORIZONTAL_POSITION        = 810;
    const float TREE_VERTICAL_POSITION          = 0;
    const float BEE_HORIZONTAL_START_POSITION   = 0;
    const float BEE_VERTICAL_START_POSITION     = 800;
    
    VideoMode vm({ SCREEN_WIDTH, SCREEN_HEIGHT });
    RenderWindow window(vm, "Timber", Style::None, State::Windowed);


    // Background
    Texture textureBackground;
    bool loadSuccessfull = textureBackground.loadFromFile("graphics/background.png");

    Sprite spriteBackground(textureBackground);
    spriteBackground.setPosition({ 0, 0 });

    // Tree
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree(textureTree);
    spriteTree.setPosition({ TREE_HORIZONTAL_POSITION, TREE_VERTICAL_POSITION });

    // Bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee(textureBee);
    spriteBee.setPosition({ BEE_HORIZONTAL_START_POSITION, BEE_VERTICAL_START_POSITION });
    bool beeActive = false;
    float beeSpeed = 0.0f;

    // Clouds
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1(textureCloud);
    Sprite spriteCloud2(textureCloud);
    Sprite spriteCloud3(textureCloud);
    spriteCloud1.setPosition({ 0, 0 });
    spriteCloud2.setPosition({ 0, 250 });
    spriteCloud3.setPosition({ 0, 500 });
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // Time control
    Clock clock;

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Scancode::Escape))
        {
            window.close();
        }

        /*
         ******************************
         * Update the scene
         ******************************
         */
        Time dt = clock.restart();

        // Bee
        if (!beeActive)
        {
            // Setup bee
            srand((int)time(0));
            beeSpeed = (rand() % 200) + 200;
            srand((int)time(0) * 10);
            float height = (rand() % 500) + 500;
            spriteBee.setPosition({ 2000, height });
            beeActive = true;
        }
        else
        {
            // Move the bee
            spriteBee.setPosition({
                (spriteBee.getPosition().x - (beeSpeed * dt.asSeconds())),
                spriteBee.getPosition().y
            });

            if (spriteBee.getPosition().x < -100)
            {
                beeActive = false;
            }
        }

        // Clouds
        // Cloud 1
        if (!cloud1Active)
        {
            srand((int)time(0) * 10);
            cloud1Speed = (rand() % 200);
            srand((int)time(0) * 10);
            float height = (rand() % 150);
            spriteCloud1.setPosition({ -200, height });
            cloud1Active = true;
        }
        else
        {
            spriteCloud1.setPosition({
                spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
                spriteCloud1.getPosition().y
            });

            if (spriteCloud1.getPosition().x > 1920)
            {
                cloud1Active = false;
            }
        }
        // Cloud 2
        if (!cloud2Active)
        {
            srand((int)time(0) * 20);
            cloud2Speed = (rand() % 200);
            srand((int)time(0) * 20);
            float height = (rand() % 150);
            spriteCloud2.setPosition({ -200, height });
            cloud2Active = true;
        }
        else
        {
            spriteCloud2.setPosition({
                spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
                spriteCloud2.getPosition().y
            });

            if (spriteCloud2.getPosition().x > 1920)
            {
                cloud2Active = false;
            }
        }
        // Cloud 3
        if (!cloud3Active)
        {
            srand((int)time(0) * 30);
            cloud3Speed = (rand() % 200);
            srand((int)time(0) * 30);
            float height = (rand() % 150);
            spriteCloud3.setPosition({ -200, height });
            cloud3Active = true;
        }
        else
        {
            spriteCloud3.setPosition({
                spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
                spriteCloud3.getPosition().y
            });

            if (spriteCloud3.getPosition().x > 1920)
            {
                cloud3Active = false;
            }
        }


        /*
         ******************************
         * Draw the scene
         ******************************
         */ 
        window.clear();

        window.draw(spriteBackground);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteTree);
        window.draw(spriteBee);

        window.display();
    }


    return 0;
}
