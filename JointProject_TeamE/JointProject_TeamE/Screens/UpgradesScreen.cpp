#include "UpgradesScreen.h"

/// <summary>
/// Default constructor for the upgrades screen that uses a vector of CarData and 
/// screen width to set the display positions of the car sprites. The Car data is used to display 
/// thecorresponding statistics for the car on the upgrades screen.
/// </summary>
/// <param name="carsIn">Vector of car data received from the track</param>
/// <param name="screenWidth">The width of the current screen</param>
UpgradesScreen::UpgradesScreen(std::vector<CarData> & carsIn, int & credits, int screenWidth) : Screen(GameState::UpgradesScreen), m_backButtonPressed(false), m_cars(carsIn), m_currentCarIndex(0),
m_credits(credits)
{
	m_transitionIn = true;
	m_upgradeCost = 0;
	m_baseCost = 25;
	sf::Color focusColor = sf::Color::Red;
	sf::Color nofocusColor = sf::Color::Magenta;
	sf::Color fillColor = sf::Color::Blue;

	sf::Vector2f endTranstionPos = sf::Vector2f(1400.0f, 400.0f);

	m_title = new Label("Upgrades", nullptr, 50, sf::Vector2f(420.0f, 50.0f), endTranstionPos);
	m_title->setPosition(sf::Vector2f(-400.0f, 50.0f));
	m_accelerationButton = new Button(focusColor, nofocusColor, fillColor, "Acceleration", nullptr, sf::Vector2f(-320.0f, 350.0f),
		18, 100.0f, 40.0f, sf::Vector2f(220.0f, 430.0f), endTranstionPos);
	m_brakingButton = new Button(focusColor, nofocusColor, fillColor, "Braking", nullptr, sf::Vector2f(-320.0f, 350.0f),
		18, 100.0f, 40.0f, sf::Vector2f(420.0f, 430.0f), endTranstionPos);
	m_corneringButton = new Button(focusColor, nofocusColor, fillColor, "Cornering", nullptr, sf::Vector2f(-320.0f, 350.0f),
		18, 100.0f, 40.0f, sf::Vector2f(620.0f, 430.0f), endTranstionPos);
	m_backButton = new Button(focusColor, nofocusColor, fillColor, "Back", nullptr, sf::Vector2f(-320.0f, 350.0f),
		18, 100.0f, 40.0f, sf::Vector2f(420.0f, 500.0f), endTranstionPos);
	m_accelerationSlider = new Slider(focusColor, nofocusColor, fillColor, "Acceleration", nullptr, sf::Vector2f(400.0f, -200.0f),
		18, 250.0f, 15.0f, sf::Vector2f(650.0f, 150.0f), endTranstionPos);
	m_brakingSlider = new Slider(focusColor, nofocusColor, fillColor, "Braking", nullptr, sf::Vector2f(400.0f, -200.0f),
		18, 250.0f, 15.0f, sf::Vector2f(650.0f, 200.0f), endTranstionPos);
	m_corneringSlider = new Slider(focusColor, nofocusColor, fillColor, "Cornering", nullptr, sf::Vector2f(400.0f, -200.0f),
		18, 250.0f, 15.0f, sf::Vector2f(650.0f, 250.0f), endTranstionPos);
	m_costPosition = sf::Vector2f(400, -400);
	m_creditsPosition = sf::Vector2f(400, -400);
	m_costText.setFont(g_resourceMgr.fontHolder["GameFont"]);
	m_costText.setPosition(sf::Vector2f(400, -200.0f));
	m_costText.setString("Cost: ");
	m_creditsText.setFont(g_resourceMgr.fontHolder["GameFont"]);
	m_creditsText.setPosition(sf::Vector2f(400, -200.0f));
	m_creditsText.setString("Credits: ");
	int baseXPos = (screenWidth / (m_cars.size() + 1));
	for (int i = 0; i < m_cars.size(); i++)
	{
		int xPos = baseXPos * (i + 1) + 15;
		RadioButton * rad = new RadioButton(focusColor, nofocusColor, fillColor, m_cars.at(i).m_name, nullptr,
			endTranstionPos, m_radButtons, sf::Vector2f(xPos, 350.0f), endTranstionPos,
			22, 30.0f, 30.0f);
		rad->select = std::bind(&UpgradesScreen::radButtonCallback, this);
		m_radButtons.push_back(rad);
		m_gui.add(rad);
	}
	for (int i = 0; i < m_cars.size(); i++)
	{
		if(i > 0)
		m_radButtons.at(i)->m_left = m_radButtons.at((i - 1) % m_cars.size());
		if(i < m_cars.size() - 1)
		m_radButtons.at(i)->m_right = m_radButtons.at((i + 1) % m_cars.size());
		m_radButtons.at(i)->m_down = m_accelerationButton;
		m_radButtons.at(i)->m_up = m_backButton;
	}
	m_radButtons.at(0)->activate(); // Always have a radiobutton activated

	m_accelerationButton->promoteFocus();

	m_accelerationButton->m_up = m_radButtons.at(0);
	m_accelerationButton->m_right = m_brakingButton;
	m_accelerationButton->m_left = m_corneringButton;
	m_accelerationButton->m_down = m_backButton;
	m_brakingButton->m_right = m_corneringButton;
	m_brakingButton->m_left = m_accelerationButton;
	m_brakingButton->m_down = m_backButton;
	m_brakingButton->m_up = m_radButtons.at(0);
	m_corneringButton->m_right = m_accelerationButton;
	m_corneringButton->m_left = m_brakingButton;
	m_corneringButton->m_down = m_backButton;
	m_corneringButton->m_up = m_radButtons.at(0);
	m_backButton->m_up = m_accelerationButton;
	m_backButton->m_down = m_radButtons.at(0);

	m_backButton->select = std::bind(&UpgradesScreen::backCallback, this);
	m_accelerationButton->select = std::bind(&UpgradesScreen::accelerationCallback, this);
	m_brakingButton->select = std::bind(&UpgradesScreen::brakingCallback, this);
	m_corneringButton->select = std::bind(&UpgradesScreen::corneringCallback, this);

	m_gui.add(m_title);
	m_gui.add(m_accelerationButton);
	m_gui.add(m_brakingButton);
	m_gui.add(m_corneringButton);
	m_gui.add(m_backButton);
	m_gui.add(m_accelerationSlider);
	m_gui.add(m_brakingSlider);
	m_gui.add(m_corneringSlider);

	m_gui.update();

	m_carSprite.setPosition(sf::Vector2f(50, 100));

	setSliders();
}

/// <summary>
/// Default destructor for the upgrades screen
/// </summary>
UpgradesScreen::~UpgradesScreen() {}

/// <summary>
/// if the screen is transiitoning update the screens position 
/// else process input for the gui objects in the screen
/// </summary>
/// <param name="controller"></param>
void UpgradesScreen::update(XboxController & controller)
{
	if (m_transitionIn)
	{
		m_gui.transitionIn(0.03f, m_interpolation);
		if (m_interpolation >= 1.0f)
		{
			m_transitionIn = false;
			m_interpolation = 0.0f;
			m_costText.setString("Cost: " + std::to_string(m_upgradeCost));
		}
	}
	else if (m_backButtonPressed)
	{
		transOut(GameState::MainMenu);
	}
	m_creditsPosition = m_accelerationSlider->getPosition() - sf::Vector2f(50, 70);
	m_costPosition = m_accelerationButton->getPosition() - sf::Vector2f(50,65);


	if (m_accelerationButton->getFocus()) {
		m_costPosition = sf::Vector2f(m_accelerationButton->getPosition() - sf::Vector2f(50, 65));
		if (m_cars.at(m_currentCarIndex).m_acceleration < 300)
		{
			m_upgradeCost = m_baseCost * (m_cars.at(m_currentCarIndex).m_acceleration / 25.0f);
		}
		else {
			m_upgradeCost = 0;
		}
		checkAvailable(m_accelerationUpgradeAvailable);
		setDisplayInfo(m_accelerationUpgradeAvailable);
	}
	else if (m_brakingButton->getFocus()) {
		m_costPosition = sf::Vector2f(m_brakingButton->getPosition() - sf::Vector2f(50, 65));
		if (m_cars.at(m_currentCarIndex).m_deceleration < 300)
		{
			m_upgradeCost = m_baseCost * (m_cars.at(m_currentCarIndex).m_deceleration / 25.0f);
		}
		else {
			m_upgradeCost = 0;
		}
		checkAvailable(m_brakingUpgradeAvailable);
		setDisplayInfo(m_brakingUpgradeAvailable);
	}
	else if (m_corneringButton->getFocus()) {
		m_costPosition = sf::Vector2f(m_corneringButton->getPosition() - sf::Vector2f(50, 65));
		if (m_cars.at(m_currentCarIndex).m_turnRate < 300)
		{
			m_upgradeCost = m_baseCost * (m_cars.at(m_currentCarIndex).m_turnRate / 25.0f);
		}
		else {
			m_upgradeCost = 0;
		}
		checkAvailable(m_corneringUpgradeAvailable);
		setDisplayInfo(m_corneringUpgradeAvailable);
	}
	m_costText.setPosition(m_costPosition);
	m_creditsText.setPosition(m_creditsPosition);
	m_creditsText.setString("Credits: " + std::to_string(m_credits));
	m_gui.processInput(controller);
}

/// <summary>
/// Reset the current screen
/// </summary>
void UpgradesScreen::reset()
{
	m_backButtonPressed = false;
	m_transitionIn = true;
	m_interpolation = 0.f;
}

/// <summary>
/// Draw the current screen
/// </summary>
/// <param name="window">Current game window</param>
void UpgradesScreen::render(sf::RenderWindow& window)
{
	window.draw(m_gui);
	window.draw(m_carSprite);
	window.draw(m_costText);
	window.draw(m_creditsText);
}

/// <summary>
/// Callback function for back button pressed
/// </summary>
void UpgradesScreen::backCallback()
{
	m_backButtonPressed = true;
}


void UpgradesScreen::setSliders()
{
	m_accelerationSlider->setPercentageFull(m_cars.at(m_currentCarIndex).m_acceleration / 300.f);
	m_brakingSlider->setPercentageFull(m_cars.at(m_currentCarIndex).m_deceleration / 300.f);
	m_corneringSlider->setPercentageFull(m_cars.at(m_currentCarIndex).m_turnRate / 300.f);
	setSprite();
}

/// <summary>
/// callback function for whenever a radio button is selected to change 
/// the corresponding slider values
/// </summary>
void UpgradesScreen::radButtonCallback()
{
	for (int i = 0; i < m_radButtons.size(); i++)
	{
		if (m_radButtons.at(i)->getState())
		{
			m_currentCarIndex = i;
			setSliders();
			break;
		}
	}
}

/// <summary>
/// Callback function to set the acceleration slider value based
/// on the selection of radio buttons
/// </summary>
void UpgradesScreen::accelerationCallback()
{
	purchaseUpgrade(m_cars.at(m_currentCarIndex).m_acceleration);
}

/// <summary>
/// Callback function to set the braking slider value based
/// on the selection of radio buttons
/// </summary>
void UpgradesScreen::brakingCallback()
{
	purchaseUpgrade(m_cars.at(m_currentCarIndex).m_deceleration);
}

/// <summary>
/// Callback function to set the cornering slider value based
/// on the selection of radio buttons
/// </summary>
void UpgradesScreen::corneringCallback()
{
	purchaseUpgrade(m_cars.at(m_currentCarIndex).m_turnRate);
}

/// <summary>
/// Set the current car sprite to be drawn on the screen
/// </summary>
void UpgradesScreen::setSprite()
{
	m_carSprite.setTexture(g_resourceMgr.textureHolder[m_cars.at(m_currentCarIndex).m_texture], true);
	float width = m_carSprite.getLocalBounds().width;
	float height = m_carSprite.getLocalBounds().height;
	m_carSprite.setScale(sf::Vector2f(300 / width, 150 / height));
}

void UpgradesScreen::checkAvailable(bool & upgradeAvailable)
{
	if (m_credits >= m_upgradeCost && m_upgradeCost != 0)
		upgradeAvailable = true;
	else
		upgradeAvailable = false;
}

/// <summary>
/// Method to purchase any upgrade
/// </summary>
/// <param name="upgradeAvailable">set the upgrade to be unavialble if you dont have enough credits</param>
void UpgradesScreen::purchaseUpgrade(float & upgrade)
{
	if (m_credits >= m_upgradeCost && m_upgradeCost != 0) {
		if (upgrade < 275) {
			upgrade += 25;
		}
		else {
			upgrade = 300;
		}
		m_credits -= m_upgradeCost;
	}
	setSliders();
}

void UpgradesScreen::setDisplayInfo(bool & upgradeAvailable)
{
	if (upgradeAvailable) {
		m_costText.setString("Cost: " + std::to_string(m_upgradeCost));
	}
	else if (m_credits <= m_upgradeCost) {
		m_costText.setString("Not enough Credits");
	}
	else{
		m_costText.setString("Max");
	}
}
