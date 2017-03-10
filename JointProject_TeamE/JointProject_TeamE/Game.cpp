#include "Game.h"

#define MS_PER_UPDATE 10.0

Game::Game()
	: m_window(sf::VideoMode(800, 600, 32), "Joint Project Team E", sf::Style::Default),
	m_xboxController(CONTROLLER_ONE),
	m_player(m_xboxController)
{

}

void Game::run()
{
	LevelLoader::load(m_level);
	g_resourceMgr.loadAssets(m_level);
	m_track.setTrack(m_level);
	m_screenManager.init();
	m_player.setCar();
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	sf::Clock clock;
	timeSinceLastUpdate = clock.restart();

	std::cout << m_level.m_playerData.position.x << ", " << m_level.m_playerData.position.y << std::endl;

	while (m_window.isOpen())
	{
		processEvents();

		timeSinceLastUpdate += clock.restart();
		if (timeSinceLastUpdate > timePerFrame)
		{
			update(timeSinceLastUpdate.asMilliseconds());
			timeSinceLastUpdate = sf::Time::Zero;
		}

		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		processGameEvents(event);
	}

	m_screenManager.processInput(m_xboxController);
}

void Game::processGameEvents(sf::Event& event)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window.close();
	}
}

void Game::update(double dt)
{
	switch (currentGameState)
	{
	case GameState::MainMenu:
		m_screenManager.update();
		break;
	case GameState::Play:
		m_player.update(dt);
		break;
	default:
		break;
	}
}

void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	m_track.render(m_window);
	m_player.render(m_window);
	switch (currentGameState)
	{
	case GameState::MainMenu:
		m_screenManager.render(m_window);
		break;
	case GameState::Play:
		m_track.render(m_window);
		break;
	default:
		break;
	}
	m_window.display();
}