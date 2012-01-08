#pragma once

#include <Object.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace terra {
	class Engine;
	
	class Tile : public Object {
		public:
			friend class Engine;
			
			Tile();
			virtual ~Tile();
			
			void Update();
			void Render( sf::RenderTarget& Target );

		private:
			sf::Sprite m_tile;
			
			const std::string& GetType() const;
	};
}
