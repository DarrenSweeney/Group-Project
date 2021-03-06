#include "HelpScreen.h"

/// <summary>
/// Initalises the help screen information and the back button
/// </summary>
HelpScreen::HelpScreen() : Screen(GameState::HelpScreen)
{
	m_transitionIn = true;

	sf::Color focusColor = sf::Color::Red;
	sf::Color nofocusColor = sf::Color::Magenta;
	sf::Color fillColor = sf::Color::Blue;

	sf::Vector2f endTranstionPos = sf::Vector2f(400.0f, 1400.0f);

	m_helpScreenText.push_back(new Label("Help", nullptr, 50, sf::Vector2f(400.0f, 50.0f), endTranstionPos));
	m_helpScreenText.push_back(new Label("In Game", nullptr, 30, sf::Vector2f(200.0f, 100.0f), endTranstionPos));
	m_helpScreenText.push_back(new Label("* RT to acclerate", nullptr, 22, sf::Vector2f(250.0f, 150.0f), endTranstionPos));
	m_helpScreenText.push_back(new Label("* LT to brake", nullptr, 22, sf::Vector2f(230.0f, 180.0f), endTranstionPos));
	m_helpScreenText.push_back(new Label("* Left analogue stick to steer", nullptr, 22, sf::Vector2f(310.0f, 210.0f), endTranstionPos));
	m_helpScreenText.push_back(new Label("* A to fire Projectiles", nullptr, 22, sf::Vector2f(250.0f, 240.0f), endTranstionPos));
	m_helpScreenText.push_back(new Label("* Start to pause", nullptr, 22, sf::Vector2f(245.0f, 270.0f), endTranstionPos));
	m_helpScreenText.push_back(new Label("In Menu", nullptr, 30, sf::Vector2f(200.0f, 350.0f), endTranstionPos));
	m_helpScreenText.push_back(new Label("* Left Stick or D-Pad to navigate", nullptr, 22, sf::Vector2f(330.0f, 400.0f), endTranstionPos));
	m_helpScreenText.push_back(new Label("* A to select", nullptr, 22, sf::Vector2f(228.0f, 430.0f), endTranstionPos));
	m_backButton = new Button(focusColor, nofocusColor, fillColor, "Back", nullptr,
		sf::Vector2f(-400.0f, 550.0f), 18, 100.0f, 40.0f, sf::Vector2f(400.0f, 550.0f), endTranstionPos);

	m_backButton->promoteFocus();

	m_backButton->select = std::bind(&HelpScreen::backButtonCallback, this);

	for (Label *label : m_helpScreenText)
	{
		label->setPosition(endTranstionPos);
		m_gui.add(label);
	}
	m_gui.add(m_backButton);
}

HelpScreen::~HelpScreen()
{

}

/// <summary>
/// Updates the widgets on screen
/// </summary>
void HelpScreen::update(XboxController & controller)
{
	if (m_transitionIn)
	{
		m_gui.transitionIn(0.03f, m_interpolation);
		if (m_interpolation >= 1.0f)
		{
			m_transitionIn = false;
			m_interpolation = 0.0f;
		}
	}
	else if (m_backButtonSelected)
	{
		transOut(GameState::OptionsScreen);
	}
	m_gui.processInput(controller);
}

/// <summary>
/// reset the current screen
/// </summary>
void HelpScreen::reset()
{
	m_interpolation = 0.f;
	m_transitionIn = true;
	m_backButtonSelected = false;
}

/// <summary>
/// Call back function for the back button pressed
/// </summary>
void HelpScreen::backButtonCallback()
{
	m_backButtonSelected = true;
}