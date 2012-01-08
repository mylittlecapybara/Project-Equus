#include <Tile.hpp>

namespace terra {
	Tile::Tile() {
	}
	
	Tile::~Tile() {
	}
	
	void Tile::Update() {
	}
	
	void Tile::Render( sf::RenderTarget& Target ) {
		Target.Draw( m_tile );
	}
	
	const std::string& Tile::GetType() const {
		static std::string Name( "Tile" );
		return Name;
	}
}
