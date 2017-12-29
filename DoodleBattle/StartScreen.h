#ifndef START_SCREEN_H
#define START_SCREEN_H


#include "Effects\Starfield\Starfield.h"
#include "GUIObject.h"
#include "EventManager.h"

#include "SFML\Audio.hpp"

using namespace Utils;

namespace Voyage
{
	class StartScreen
	{
	public:
		StartScreen(Effects::STARFIELD_DIRECTION inDirection);

		void Update();
		void Render(sf::RenderWindow* inWindow);
		void HandleEvent(sf::Event inEvent);
		void onEvent(EventData* event);

	private:
		Effects::Starfield* starField;
		GUIObject* title;
		sf::Sprite titleObscura;
		sf::Texture tOTexture;

		sf::Music titleMusic;
		sf::Sprite fadeSprite;
		sf::Texture fadeTexture;

		sf::Sound titleBang;
		sf::SoundBuffer titleBangBuf;

		bool fadeOut;
		bool start;
	};
}

#endif