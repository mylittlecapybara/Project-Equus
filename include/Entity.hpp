#pragma once

#include <Object.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace terra {
	class Entity : public Object {
		public:
			Entity();
			virtual ~Entity();
			
			virtual void Cleanup();
			virtual void JoystickButtonPressed( unsigned int JoystickID, unsigned int Button );
			virtual void JoystickButtonReleased( unsigned int JoystickID, unsigned int Button );
			virtual void JoystickMoved( unsigned int JoystickID, sf::Joystick::Axis Axis, float Position );
			virtual void KeyPressed( sf::Keyboard::Key Code, bool Alt, bool Control, bool Shift, bool System );
			virtual void KeyReleased( sf::Keyboard::Key Code, bool Alt, bool Control, bool Shift, bool System );
			virtual void MouseWheelMoved( int Delta, int X, int Y );
			virtual void MouseButtonPressed( sf::Mouse::Button Button, int X, int Y );
			virtual void MouseButtonReleased( sf::Mouse::Button Button, int X, int Y );
			virtual void MouseMoved( int X, int Y );
			virtual void TextEntered( sf::Uint32 Unicode );
			
			virtual void Update();
			virtual void Render( sf::RenderTarget& Target );
		
		private:
			const std::string& GetType() const;
	};
}
