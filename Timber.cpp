#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    VideoMode vm({ 1920, 1080 });
    RenderWindow window(vm, "Timber", Style::None, State::Windowed);

    Texture textureBackground;
    bool loadSuccessfull = textureBackground.loadFromFile("graphics/background.png");

    Sprite spriteBackground(textureBackground);
    spriteBackground.setPosition({ 0, 0 });

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Scancode::Escape))
        {
            window.close();
        }

        // Update the scene

        // Draw the scene
        window.clear();

        window.draw(spriteBackground);

        window.display();
    }


    return 0;
}
