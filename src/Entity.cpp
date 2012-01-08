#include <Entity.hpp>

namespace terra {
	Entity::Entity() {
	}
	
	Entity::~Entity() {
	}

	void Entity::Cleanup() {
	}
	
	void Entity::JoystickButtonPressed( unsigned int, unsigned int ) {
	}
	
	void Entity::JoystickButtonReleased( unsigned int, unsigned int ) {
	}
	
	void Entity::JoystickMoved( unsigned int, sf::Joystick::Axis, float ) {
	}
	
	void Entity::KeyPressed( sf::Keyboard::Key, bool, bool, bool, bool ) {
	}
	
	void Entity::KeyReleased( sf::Keyboard::Key, bool, bool, bool, bool ) {
	}
	
	void Entity::MouseWheelMoved( int, int, int ) {
	}
	
	void Entity::MouseButtonPressed( sf::Mouse::Button, int, int ) {
	}
	
	void Entity::MouseButtonReleased( sf::Mouse::Button, int, int ) {
	}
	
	void Entity::MouseMoved( int, int ) {
	}
	
	void Entity::TextEntered( sf::Uint32 ) {
	}

	void Entity::Update() {
	}
	
	void Entity::Render( sf::RenderTarget& ) {
	}

	const std::string& Entity::GetType() const {
		static std::string Name( "Entity" );
		return Name;
	}
};
