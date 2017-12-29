#include "StartScreen.h"

namespace Voyage
{
	StartScreen::StartScreen(Effects::STARFIELD_DIRECTION inDirection)
	{
		starField = new Effects::Starfield(inDirection);
		title = new GUIObject("Art/Voyage_Title.png", sf::Vector2f(), NULL, CENTER);
		title->setOnline();

		tOTexture.loadFromFile("Art/titleObscura.png");
		titleObscura.setTexture(tOTexture);
		titleObscura.setPosition(50, 300);

		titleMusic.openFromFile("Audio/titleDrone.wav");
		

		fadeTexture.loadFromFile("Art/fadeCover.png");
		fadeSprite.setTexture(fadeTexture);
		fadeSprite.setColor(sf::Color(255, 255, 255, 0.0f));
		fadeOut = false;

		start = false;

		//add an event listener, creating a function refence bound to this instance
		EventManager::Get()->addEventListener(BUTTON_PRESSED, boost::bind(&StartScreen::onEvent, this, _1));
	}

	void StartScreen::onEvent(EventData* event)
	{
		//cast the event to a button pressed event
		ButtonPressedEventData* buttonEvent = (ButtonPressedEventData*)event;
		//remove the event listener
		EventManager::Get()->removeEventListener(BUTTON_PRESSED, boost::bind(&StartScreen::onEvent, this, _1) );
	}

	void StartScreen::Update()
	{
		if(start)
		{
			if(titleObscura.getPosition().x > 900)
			{
				fadeOut = true;
			}
		
			titleObscura.move(0.38f, 0);
		
			if(fadeOut == true)
			{
				sf::Uint8 newA = fadeSprite.getColor().a;
				if(newA < 255)
				{
					newA += 1;

					fadeSprite.setColor(sf::Color(255, 255, 255, newA));
				}
				else
				{
					//fade complete - dispatch a button pressed event to all listeners
					EventManager::Get()->dispatchEvent(BUTTON_PRESSED, new ButtonPressedEventData(4));
				}
			}

			starField->Update();
		}
	}

	void StartScreen::Render(sf::RenderWindow* inWindow)
	{
		//starField->Render(inWindow);
		title->Render(inWindow);
		inWindow->draw(titleObscura);
		inWindow->draw(fadeSprite);
	}

	void StartScreen::HandleEvent(sf::Event inEvent)
	{
		if(inEvent.type == sf::Event::KeyPressed)
		{
			start = true;
			titleMusic.play();
		}
	}
}