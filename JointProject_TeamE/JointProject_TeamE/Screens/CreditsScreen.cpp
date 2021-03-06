#include "CreditsScreen.h"

/// <summary>
/// Default constructor for the credits screen
/// </summary>
CreditsScreen::CreditsScreen()
	: Screen(GameState::CreditsScreen), m_playing(true)
{
	try
	{
		m_creditsVideo.openFromFile("Resources/Movies/Credits.mov");
	}
	catch (std::exception e)
	{
		std::cout << typeid(*this).name() << std::endl;
		std::cout << "ERROR: " << e.what() << std::endl;
	}

	m_creditsVideo.setPosition(0, 0);
	m_creditsVideo.fit(0, 0, 800, 600);
}

/// <summary>
/// default destructor for the credits screen
/// </summary>
CreditsScreen::~CreditsScreen()
{

}

/// <summary>
/// Update the credits screen to allow playing of the sfeMovie
/// file loaded in
/// </summary>
/// <param name="controller"></param>
void CreditsScreen::update(XboxController & controller)
{
	if (!m_startVideo)
	{
		m_creditsVideo.play();
		m_startVideo = true;
	}

	if (m_playing)
	{
		m_creditsVideo.update();
		if (!m_skip)
		{
			for (unsigned int i = 0; i < 10; i++)
			{
				if (controller.isButtonPressed(i))
				{
					m_creditsVideo.pause();
					m_skip = true;
					break;
				}
			}
		}
		if (m_creditsVideo.getStatus() == sfe::Status::Paused || m_creditsVideo.getStatus() == sfe::Status::Stopped)
		{
			m_nextGameState = GameState::MainMenu;
		}
	}
}

/// <summary>
/// Render the credits movie to the screen
/// </summary>
/// <param name="window">Current render window</param>
void CreditsScreen::render(sf::RenderWindow &window)
{
	if (m_playing)
	{
		window.draw(m_creditsVideo);
	}
}

/// <summary>
/// Reset the current screen
/// </summary>
void CreditsScreen::reset()
{
	m_creditsVideo.stop();
	m_playing = true;
	m_skip = false;
	m_startVideo = false;
}