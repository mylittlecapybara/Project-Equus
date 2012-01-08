#pragma once

#include <Layer.hpp>
#include <map>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>

namespace terra {
	class Engine : public NonCopyable {
		public:
			typedef std::shared_ptr< Object >( *Callback )( float, float, float, float, std::map< std::string, std::string > );
		
			~Engine();
			
			static Engine& Get();

			const std::string& GetLevelProperty( const std::string& Name );
			std::shared_ptr< sf::Texture > GetTexture( std::string Filename );
			sf::RenderWindow& GetWindow();
			std::vector< Layer >::iterator LayersBegin();
			std::vector< Layer >::iterator LayersEnd();
			bool LoadLevel( std::string Filename );
			bool Initialize();
			void Register( std::string Name, Callback Creator );
			void Run();

		private:
			std::map< std::string, Callback > m_creators;
			std::vector< Layer > m_layers;
			std::string m_level_orientation;
			std::map< std::string, std::string> m_level_properties;
			std::map< std::string, std::shared_ptr< sf::Texture > > m_textures;
			sf::RenderWindow m_window;
			
			Engine();
	};
}
