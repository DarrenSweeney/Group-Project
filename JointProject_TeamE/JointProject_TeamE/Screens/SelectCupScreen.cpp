#include "SelectCupScreen.h"

/// <summary>
/// Default constructor for the set cars screen which allows the user to 
/// select a race type which then sets the level data in turn
/// </summary>
/// <param name="carsIn">Vector of car data stored in the level</param>
/// <param name="cupsIn">Cup datat in the level</param>
/// <param name="screenWidth">Screen widtth for display purposes</param>
SelectCupScreen::SelectCupScreen(std::vector<CarData> & carsIn, std::vector<CupData> & cupsIn, int screenWidth) 
	: Screen(GameState::SelectCupScreen), m_cups(cupsIn)
{
	m_transitionIn = true;

	sf::Color focusColor = sf::Color::Red;
	sf::Color nofocusColor = sf::Color::Magenta;
	sf::Color fillColor = sf::Color::Blue;

	sf::Vector2f endTranstionPos = sf::Vector2f(1400.0f, 400.0f);

	m_title = new Label("Select Cup", nullptr, 50, sf::Vector2f(420.0f, 50.0f), endTranstionPos);
	m_title->setPosition(sf::Vector2f(-400.0f, 50.0f));
	m_backButton = new Button(focusColor, nofocusColor, fillColor, "Back", nullptr, sf::Vector2f(-320.0f, 350.0f),
		18, 100.0f, 40.0f, sf::Vector2f(320.0f, 430.0f), endTranstionPos);
	m_confirmButton = new Button(focusColor, nofocusColor, fillColor, "Confirm", nullptr, sf::Vector2f(-320.0f, 350.0f),
		18, 100.0f, 40.0f, sf::Vector2f(520.0f, 430.0f), endTranstionPos);
	int baseXPos = (screenWidth / (m_cups.size() + 1));
	for (int i = 0; i < m_cups.size(); i++)
	{
		int xPos = baseXPos * (i + 1) + 15;
		RadioButton * rad = new RadioButton(focusColor, nofocusColor, fillColor, m_cups.at(i).m_name, nullptr,
			endTranstionPos, m_radButtons, sf::Vector2f(xPos, 150.0f), endTranstionPos,
			22, 30.0f, 30.0f);
		m_radButtons.push_back(rad);
		m_gui.add(rad);
	}
	for (int i = 0; i < m_cups.size(); i++)
	{
		if (i > 0)
			m_radButtons.at(i)->m_left = m_radButtons.at((i - 1) % m_cups.size());
		if (i < m_cups.size() - 1)
			m_radButtons.at(i)->m_right = m_radButtons.at((i + 1) % m_cups.size());
		m_radButtons.at(i)->m_down = m_confirmButton;
		m_radButtons.at(i)->m_up = m_confirmButton;
	}
	m_radButtons.at(0)->activate();
	m_currentCarIndex = 0;

	m_confirmButton->m_left = m_backButton;
	m_confirmButton->m_right = m_backButton;
	m_confirmButton->m_up = m_radButtons.at(0);
	m_confirmButton->m_down = m_radButtons.at(0);
	m_backButton->m_left = m_confirmButton;
	m_backButton->m_right = m_confirmButton;
	m_backButton->m_up = m_radButtons.at(0);
	m_backButton->m_down = m_radButtons.at(0);

	m_confirmButton->select = std::bind(&SelectCupScreen::confirmButtonCallback, this);
	m_backButton->select = std::bind(&SelectCupScreen::backButtonCallback, this);

	m_gui.add(m_title);
	m_gui.add(m_confirmButton);
	m_gui.add(m_backButton);

	m_confirmButton->promoteFocus();
}

SelectCupScreen::~SelectCupScreen() {}

/// <summary>
/// If not transitiionig process input for all gui objects
/// </summary>
/// <param name="controller"></param>
void SelectCupScreen::update(XboxController & controller)
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
	else if (m_confirmButtonPressed)
	{
		transOut(GameState::GamePlay);
	}
	else if (m_backButtonPressed)
	{
		transOut(GameState::SelectCarScreen);
	}
	m_gui.processInput(controller);
}

/// <summary>
/// resetthe current screen
/// </summary>
void SelectCupScreen::reset()
{
	m_transitionIn = true;
	m_interpolation = 0.f;
	m_confirmButtonPressed = false;
	m_backButtonPressed = false;
}
/// <summary>
/// Get the current cup index
/// </summary>
/// <returns></returns>
unsigned int SelectCupScreen::selectedCupIndex()
{
	return m_currentCarIndex;
}
//Callback functions
void SelectCupScreen::confirmButtonCallback()
{
	for (int i = 0; i < m_radButtons.size(); i++)
	{
		if(m_radButtons.at(i)->getState())
		{
			m_currentCarIndex = i;
		}
	}
	m_confirmButtonPressed = true;
}

void SelectCupScreen::backButtonCallback()
{
	m_backButtonPressed = true;
}