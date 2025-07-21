#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;


void updateBranches(int seed);
const int NUM_BRANCHES = 6;
std::optional<Sprite> branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	const int   SCREEN_WIDTH = 1920;
	const int   SCREEN_HEIGHT = 1080;
	const float TREE_HORIZONTAL_POSITION = 810;
	const float TREE_VERTICAL_POSITION = 0;
	const float BEE_HORIZONTAL_START_POSITION = 0;
	const float BEE_VERTICAL_START_POSITION = 800;

	VideoMode vm({ SCREEN_WIDTH, SCREEN_HEIGHT });
	RenderWindow window(vm, "Timber", Style::None, State::Windowed);
	bool loadSuccessfull;

	// Background
	Texture textureBackground;
	loadSuccessfull = textureBackground.loadFromFile("graphics/background.png");

	Sprite spriteBackground(textureBackground);
	spriteBackground.setPosition({ 0, 0 });

	// Tree
	Texture textureTree;
	loadSuccessfull = textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree(textureTree);
	spriteTree.setPosition({ TREE_HORIZONTAL_POSITION, TREE_VERTICAL_POSITION });

	// Bee
	Texture textureBee;
	loadSuccessfull = textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee(textureBee);
	spriteBee.setPosition({ BEE_HORIZONTAL_START_POSITION, BEE_VERTICAL_START_POSITION });
	bool beeActive = false;
	float beeSpeed = 0.0f;

	// Clouds
	Texture textureCloud;
	loadSuccessfull = textureCloud.loadFromFile("graphics/cloud.png");
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

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400.0f;
	float timeBarHeight = 80.0f;
	timeBar.setSize({ timeBarStartWidth, timeBarHeight });
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition({ (1920 / 2) - timeBarStartWidth / 2, 980 });
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;


	bool paused = true;

	int score = 0;
	Font font;
	loadSuccessfull = font.openFromFile("fonts/KOMIKAP_.ttf");
	Text messageText(font);
	Text scoreText(font);
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin({
		textRect.position.x + textRect.size.x / 2.0f,
		textRect.position.y + textRect.size.y / 2.0f
		});
	messageText.setPosition({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });
	scoreText.setPosition({ 20, 20 });

	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i] = std::optional<Sprite>{ textureBranch };
		branches[i].value().setPosition({ -2000, -2000 });
		branches[i].value().setOrigin({ 220, 20 });
	}

	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer{ texturePlayer };
	spritePlayer.setPosition({ 580, 720 });
	side playerSide = side::LEFT;

	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP{ textureRIP };
	spriteRIP.setPosition({ 600, 800 });

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe{ textureAxe };
	spriteAxe.setPosition({ 700, 830 });

	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog{ textureLog };
	spriteLog.setPosition({ 810, 720 });

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop{ chopBuffer };
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death{ deathBuffer };
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime{ ootBuffer };

	while (window.isOpen())
	{
		/*
		 ******************************
		 * Handle the players input
		 ******************************
		 */

		while (const std::optional event = window.pollEvent())
		{
			if (event.value().is<Event::KeyReleased>() && !paused) {
				acceptInput = true;
				spriteAxe.setPosition({ 2000, spriteAxe.getPosition().y });
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::Escape))
		{
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::Enter))
		{
			paused = false;
			score = 0;
			timeRemaining = 6;
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}
			spriteRIP.setPosition({ 675, 2000 });
			spritePlayer.setPosition({ 580, 720 });
			acceptInput = true;
		}
		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Scancode::Right))
			{
				playerSide = side::RIGHT;
				score++;
				timeRemaining += (2 / score) + 0.15;
				spriteAxe.setPosition({ AXE_POSITION_RIGHT, spriteAxe.getPosition().y });
				spritePlayer.setPosition({ 1200, 720 });
				updateBranches(score);
				spriteLog.setPosition({ 810, 720 });
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;
				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Scancode::Left))
			{
				playerSide = side::LEFT;
				score++;
				timeRemaining += (2 / score) + 0.15;
				spriteAxe.setPosition({ AXE_POSITION_LEFT, spriteAxe.getPosition().x });
				spritePlayer.setPosition({ 580, 720 });
				updateBranches(score);
				spriteLog.setPosition({ 810, 720 });
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false;
				chop.play();
			}
		}

		/*
		 ******************************
		 * Update the scene
		 ******************************
		 */

		if (!paused)
		{
			Time dt = clock.restart();
			timeRemaining -= dt.asSeconds();
			timeBar.setSize({ timeBarWidthPerSecond * timeRemaining, timeBarHeight });

			if (timeRemaining <= 0.0f)
			{
				paused = true;
				messageText.setString("Out of time!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin({
					textRect.position.x + textRect.size.x / 2.0f,
					textRect.position.y + textRect.size.y / 2.0f
					});
				messageText.setPosition({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });
				outOfTime.play();
			}

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

			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					branches[i].value().setPosition({ 610, height });
					branches[i].value().setRotation(degrees(180));
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					branches[i].value().setPosition({ 1330, height });
					branches[i].value().setRotation(degrees(0));
				}
				else
				{
					branches[i].value().setPosition({ 3000, height });
				}
			}

			if (logActive) 
			{
				spriteLog.setPosition({ 
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())
				});

				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition({ 810, 720 });
				}
			}

			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				spriteRIP.setPosition({ 525, 760 });
				spritePlayer.setPosition({ 2000, 660 });
				messageText.setString("SQUISHED!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin({
					textRect.position.x + textRect.size.x / 2.0f,
					textRect.position.y + textRect.size.y / 2.0f
					});
				messageText.setPosition({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });
				death.play();
			}

		} // End if (!paused)


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
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i].value());
		}
		window.draw(spriteTree);
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);
		window.draw(spriteBee);
		window.draw(scoreText);
		window.draw(timeBar);
		if (paused)
		{
			window.draw(messageText);
		}

		window.display();
	}


	return 0;
}

void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default: branchPositions[0] = side::NONE;
		break;
	}
}