#include <cstdlib>
#include <Engine.hpp>
#include <Entity.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <Tile.hpp>
#include <tinyxml.h>

namespace terra {
	struct TileInfo {
		unsigned int gid;
		std::string filename;
		sf::IntRect region;
	};
	
	Engine::Engine() {
	}
	
	Engine::~Engine() {
	}
	
	Engine& Engine::Get() {
		static Engine engine;
		return engine;
	}
	
	const std::string& Engine::GetLevelProperty( const std::string& Name ) {
		return m_level_properties[ Name ];
	}
	
	std::shared_ptr< sf::Texture > Engine::GetTexture( std::string Filename ) {
		if( m_textures.find( Filename ) == m_textures.end() ) {
			std::shared_ptr< sf::Texture > new_texture( new sf::Texture );
			if( !new_texture->LoadFromFile( Filename ) ) {
				return std::shared_ptr< sf::Texture >( nullptr );
			}
			m_textures[ Filename ] = new_texture;
		}
		return m_textures[ Filename ];
	}
	
	sf::RenderWindow& Engine::GetWindow() {
		return m_window;
	}
	
	std::vector< Layer >::iterator Engine::LayersBegin() {
		return m_layers.begin();
	}
	
	std::vector< Layer >::iterator Engine::LayersEnd() {
		return m_layers.end();
	}
	
	bool Engine::LoadLevel( std::string Filename ) {
		std::string level_folder = Filename;
		size_t clear_begin;
		for( clear_begin = level_folder.size() - 1; clear_begin >= 0; --clear_begin ) {
			if( level_folder.at( clear_begin ) == '/' ||
				level_folder.at( clear_begin ) == '\\' ) {
				++clear_begin;
				break;
			}
		}
		level_folder.erase( clear_begin );
		
		// Load the level
		TiXmlDocument level_document;
		if( !level_document.LoadFile( Filename ) ) {
			return false;
		}
		
		// Obtain the root element & verify its name
		TiXmlElement *level_root = level_document.RootElement();
		if( level_root == nullptr || level_root->Value() != "map" ) {
			return false;
		}
		
		// Root Attributes
		std::string level_orientation;
		unsigned int level_width;
		unsigned int level_height;
		unsigned int level_tile_width;
		unsigned int level_tile_height;
		
		// Root Attribute Verification
		if( level_root->Attribute( "orientation" ) == nullptr ||
			level_root->Attribute( "width" ) == nullptr || level_root->Attribute( "height" ) == nullptr ||
			level_root->Attribute( "tilewidth" ) == nullptr || level_root->Attribute( "tileheight" ) == nullptr ) {
				return false;
		}
		
		// Root Attribute Processing
		level_orientation = level_root->Attribute( "orientation" );
		level_width = atoi( level_root->Attribute( "width" ) );
		level_height = atoi( level_root->Attribute( "height" ) );
		level_tile_width = atoi( level_root->Attribute( "tilewidth" ) );
		level_tile_height = atoi( level_root->Attribute( "tileheight" ) );
		if( level_orientation != "isometric" && level_orientation != "orthogonal" ) {
			return false;
		}
		m_level_orientation = level_orientation;
		
		// Level Properties
		if( level_root->FirstChild( "properties" ) == nullptr ||
			level_root->FirstChild( "properties" )->ToElement() == nullptr ) {
			return false;
		}
		TiXmlElement *level_properties = level_root->FirstChild( "properties" )->ToElement();
		for( TiXmlNode *level_property_node = level_properties->FirstChild( "property" );
			 level_property_node != nullptr;
			 level_property_node = level_properties->IterateChildren( "property", level_property_node ) ) {
			if( level_property_node->ToElement() == nullptr ) {
				return false;
			}
			
			// Property Attributes
			std::string property_name;
			std::string property_value;
			
			// Property Attribute Verification
			if( level_property_node->ToElement()->Attribute( "name" ) == nullptr ||
				level_property_node->ToElement()->Attribute( "value" ) == nullptr) {
				return false;
			}
			
			// Property Attribute Processing
			property_name = level_property_node->ToElement()->Attribute( "name" );
			property_value = level_property_node->ToElement()->Attribute( "value" );
			m_level_properties[ property_name ] = property_value;
		}
		
		std::map< unsigned int, TileInfo > tile_list;
		
		// Tilesets
		for( TiXmlNode *level_tileset_node = level_root->FirstChild( "tileset" );
			 level_tileset_node != nullptr;
			 level_tileset_node = level_root->IterateChildren( "tileset", level_tileset_node ) ) {
			if( level_tileset_node->ToElement() == nullptr ) {
				return false;
			}
			
			// Tileset Attributes
			unsigned int first_gid;
			std::string source;
			unsigned int tile_width;
			unsigned int tile_height;
			unsigned int spacing = 0;
			unsigned int margin = 0;
			
			// Tileset Attribute (and Child!) Verification
			if( level_tileset_node->ToElement()->Attribute( "firstgid" ) == nullptr ||
				level_tileset_node->ToElement()->Attribute( "tilewidth" ) == nullptr ||
				level_tileset_node->ToElement()->Attribute( "tileheight" ) == nullptr ||
				( level_tileset_node->ToElement()->Attribute( "source" ) == nullptr &&
				  ( level_tileset_node->ToElement()->FirstChild( "image" ) == nullptr ||
					level_tileset_node->ToElement()->FirstChild( "image" )->ToElement() == nullptr ||
					level_tileset_node->ToElement()->FirstChild( "image" )->ToElement()->Attribute( "source" ) == nullptr ) ) ) {
				return false;
			}
			
			// Tileset Attribute Processing
			first_gid = atoi( level_tileset_node->ToElement()->Attribute( "firstgid" ) );
			tile_width = atoi( level_tileset_node->ToElement()->Attribute( "tilewidth" ) );
			tile_height = atoi( level_tileset_node->ToElement()->Attribute( "tileheight" ) );
			if( level_tileset_node->ToElement()->Attribute( "spacing" ) != nullptr ) {
				spacing = atoi( level_tileset_node->ToElement()->Attribute( "spacing" ) );
			}
			if( level_tileset_node->ToElement()->Attribute( "margin" ) != nullptr ) {
				margin = atoi( level_tileset_node->ToElement()->Attribute( "margin" ) );
			}
			if( level_tileset_node->ToElement()->Attribute( "source" ) != nullptr ) {
				source = level_folder + level_tileset_node->ToElement()->Attribute( "source" );
				//! @todo TSX Parsing
				return false;
			}
			else {
				// Process the tileset
				source = level_folder + level_tileset_node->ToElement()->FirstChild( "image" )->ToElement()->Attribute( "source" );
				sf::IntRect temp_rect;
				unsigned int tile_gid = first_gid;
				unsigned int tileset_width = GetTexture( source )->GetWidth();
				unsigned int tileset_height = GetTexture( source )->GetHeight();
				//! @todo Support Transparency (a slight issue with SFML)
				temp_rect.Width = tile_width;
				temp_rect.Height = tile_height;
				
				// Process tiles
				for( unsigned int x = margin;
					 x <= temp_rect.Width - tile_width - margin;
					 x += tile_width + spacing ) {
					for( unsigned int y = margin;
						 y <= temp_rect.Height - tile_height - margin;
						 y += tile_height + spacing ) {
						temp_rect.Left = x;
						temp_rect.Top = y;
						TileInfo new_tile;
						new_tile.filename = source;
						new_tile.gid = tile_gid;
						new_tile.region = temp_rect;
						tile_list[ new_tile.gid ] = new_tile;
						++tile_gid;
					}
				}
			}
		}
		
		m_layers.clear();
		
		// Layers & Object Groups
		for( TiXmlNode *level_layer_node = level_root->FirstChild();
			 level_layer_node != nullptr;
			 level_layer_node = level_root->IterateChildren( level_layer_node ) ) {
			if( level_layer_node->ToElement() == nullptr ) {
				return false;
			}
			
			if( level_layer_node->Value() != "layer" && level_layer_node->Value() != "objectgroup" ) {
				continue;
			}
			
			if( level_layer_node->Value() == "layer" ) {
				Layer new_layer;
				unsigned int tile_num = 0;
				
				// Layer Attributes
				//! @todo Support Layer Names
				//! @todo Support Layer Opacity
				//! @todo Support Tile Flipping
				//! @todo Support Layer Properties
				
				// Data Processing
				if( level_layer_node->ToElement()->FirstChild( "data" ) == nullptr ||
					level_layer_node->ToElement()->FirstChild( "data" )->ToElement() == nullptr ) {
					return false;
				}
				TiXmlElement *level_layer_data = level_layer_node->ToElement()->FirstChild( "data" )->ToElement();
				std::string encoding;
				std::string compression;
				if( level_layer_data->Attribute( "encoding" ) != nullptr ) {
					encoding = level_layer_data->Attribute( "encoding" );
				}
				if( level_layer_data->Attribute( "compression" ) != nullptr ) {
					compression = level_layer_data->Attribute( "compression" );
				}
				if( compression == "gzip" ) {
					return false; // gzip is unsupported due to the developer's unfortunate choice to use the GPL.
				}
				if( compression == "zlib" ) {
					//! @todo zlib translation
					return false;
				}
				if( encoding == "base64" ) {
					//! @todo Base64 translation
					return false;
				}
				else if( encoding == "csv" ) {
					//! @todo CSV translation
					return false;
				}
				else {
					for( TiXmlNode *level_layer_tile_node = level_layer_data->FirstChild( "tile" );
						 level_layer_tile_node != nullptr;
						 level_layer_tile_node = level_layer_data->IterateChildren( "tile", level_layer_tile_node ) ) {
						if( level_layer_tile_node->ToElement() == nullptr ||
							level_layer_tile_node->ToElement()->Attribute( "gid" ) == nullptr ) {
							return false;
						}
						
						// Process New Tile
						unsigned int tile_gid = atoi( level_layer_tile_node->ToElement()->Attribute( "gid" ) );
						std::shared_ptr< Object > new_tile( reinterpret_cast< Object* >( new Tile ) );
						sf::Sprite new_tile_sprite( *( GetTexture( tile_list[ tile_gid ].filename ).get() ), tile_list[ tile_gid ].region );
						
						// Calculate Tile Position
						if( level_orientation == "orthogonal" ) {
							unsigned int tile_x_index = tile_num % level_width;
							unsigned int tile_y_index = ( tile_num - tile_x_index ) / level_width;
							sf::Vector2f tile_pos;
							tile_pos.x = tile_x_index * level_tile_width;
							tile_pos.y = tile_y_index * level_tile_height;
							new_tile_sprite.SetPosition( tile_pos );
						}
						if( level_orientation == "isometric" ) {
							unsigned int tile_x_index = tile_num % level_width;
							unsigned int tile_y_index = ( tile_num - tile_x_index ) / level_width;
							unsigned int total_width = level_width * level_tile_width;
							unsigned int total_height = level_height * level_tile_height;
							sf::Vector2f tile_pos;
							tile_pos.x = static_cast< float >( total_width + level_tile_width * ( tile_x_index - tile_y_index - 1 ) ) / 2.f;
							tile_pos.y = static_cast< float >( level_tile_height * ( tile_x_index + tile_y_index ) ) / 2.f;
							new_tile_sprite.SetPosition( tile_pos );
						}
						
						// Set up new tile
						reinterpret_cast< Tile* >( new_tile.get() )->m_tile = new_tile_sprite;
						new_layer.Add( new_tile );
						
						++tile_num;
					}
				}
				
				// Add Layer
				m_layers.push_back( new_layer );
			}
			
			if( level_layer_node->Value() == "objectgroup" ) {
				Layer new_layer;
				
				// Layer Attributes
				//! @todo Support Object Group Names
				//! @todo Support Object Group Opacity and Visibility
				
				// Layer Properties
				//! @todo Support Object Group Properties
				
				// Object Processing
				for( TiXmlNode *level_object_node = level_layer_node->ToElement()->FirstChild( "object" );
					 level_object_node != nullptr;
					 level_object_node = level_layer_node->ToElement()->IterateChildren( "object", level_object_node ) ) {
					if( level_object_node->ToElement() == nullptr ) {
						return false;
					}
					
					// Object Attributes
					float object_x;
					float object_y;
					float object_width;
					float object_height;
					std::string object_type( "" );
					
					// Object Attribute Verification
					if( level_object_node->ToElement()->Attribute( "x" ) == nullptr ||
						level_object_node->ToElement()->Attribute( "y" ) == nullptr ) {
						return false;
					}
					
					// Object Property Processing
					std::map< std::string, std::string > object_properties;
					TiXmlNode *level_object_properties = level_object_node->ToElement()->FirstChild( "properties" );
					if( level_object_properties != nullptr &&
						level_object_properties->ToElement() != nullptr ) {
						for( TiXmlNode *level_object_property = level_object_properties->ToElement()->FirstChild( "property" );
							 level_object_property != nullptr;
							 level_object_property = level_object_properties->ToElement()->IterateChildren( "property", level_object_properties ) ) {
							if( level_object_property->ToElement() == nullptr ) {
								return false;
							}
							
							// Property Attributes
							std::string property_name;
							std::string property_value;
							
							// Property Attribute Verification
							if( level_object_property->ToElement()->Attribute( "name" ) == nullptr ||
								level_object_property->ToElement()->Attribute( "value" ) == nullptr) {
								return false;
							}
							
							// Property Attribute Processing
							property_name = level_object_property->ToElement()->Attribute( "name" );
							property_value = level_object_property->ToElement()->Attribute( "value" );
							
							// Add the property
							object_properties[ property_name ] = property_value;
						}
					}
					
					// Object Attribute Processing
					object_x = atof( level_object_node->ToElement()->Attribute( "x" ) );
					object_y = atof( level_object_node->ToElement()->Attribute( "y" ) );
					
					if( level_object_node->ToElement()->Attribute( "type" ) != nullptr ) {
						object_type = level_object_node->ToElement()->Attribute( "type" );
					}
					if( level_object_node->ToElement()->Attribute( "gid" ) != nullptr ) {
						object_width = tile_list[ atoi( level_object_node->ToElement()->Attribute( "gid" ) ) ].region.Width;
						object_height = tile_list[ atoi( level_object_node->ToElement()->Attribute( "gid" ) ) ].region.Height;
					}
					if( level_object_node->ToElement()->Attribute( "width" ) != nullptr ) {
						object_width = atof( level_object_node->ToElement()->Attribute( "width" ) );
					}
					if( level_object_node->ToElement()->Attribute( "height" ) != nullptr ) {
						object_width = atof( level_object_node->ToElement()->Attribute( "height" ) );
					}
					
					// Create & Add Object
					if( m_creators.find( object_type ) != m_creators.end() ) {
						new_layer.Add( m_creators[ object_type ]( object_x, object_y, object_width, object_height, object_properties ) );
					}
				}
				
				// Add Layer
				m_layers.push_back( new_layer );
			}
		}
	}
	
	bool Engine::Initialize() {
		m_window.Create( sf::VideoMode( 800, 600, 32 ), "Terra Game Engine" );
		return true;
	}
	
	void Engine::Register( std::string Name, Engine::Callback Creator ) {
		if( m_creators.find( Name ) == m_creators.end() ) {
			m_creators[ Name ] = Creator;
		}
	}
	
	void Engine::Run() {
		static std::string entity_string( "Entity" );
		while( m_window.IsOpened() ) {
			sf::Event event;
			while( m_window.PollEvent( event ) ) {
				switch( event.Type ) {
					case sf::Event::Closed:
						m_window.Close();
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->Cleanup();
							}
						}
						break;
						
					case sf::Event::Resized:
						//! @todo Handle Resize
						break;
						
					case sf::Event::LostFocus:
						//! @todo Handle Lost Focus
						break;
						
					case sf::Event::GainedFocus:
						//! @todo Handle Gained Focus
						break;
						
					case sf::Event::TextEntered:
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->TextEntered( event.Text.Unicode );
							}
						}
						break;
						
					case sf::Event::KeyPressed:
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->KeyPressed( event.Key.Code, event.Key.Alt, event.Key.Control, event.Key.Shift, event.Key.System );
							}
						}
						break;
						
					case sf::Event::KeyReleased:
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->KeyReleased( event.Key.Code, event.Key.Alt, event.Key.Control, event.Key.Shift, event.Key.System );
							}
						}
						break;
						
					case sf::Event::MouseWheelMoved:
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->MouseWheelMoved( event.MouseWheel.Delta, event.MouseWheel.X, event.MouseWheel.Y );
							}
						}
						break;
						
					case sf::Event::MouseButtonPressed:
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->MouseButtonPressed( event.MouseButton.Button, event.MouseButton.X, event.MouseButton.Y );
							}
						}
						break;
						
					case sf::Event::MouseButtonReleased:
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->MouseButtonReleased( event.MouseButton.Button, event.MouseButton.X, event.MouseButton.Y );
							}
						}
						break;
						
					case sf::Event::MouseMoved:
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->MouseMoved( event.MouseMove.X, event.MouseMove.Y );
							}
						}
						break;
						
					case sf::Event::MouseEntered:
						//! @todo Handle Mouse Entered
						break;
						
					case sf::Event::MouseLeft:
						//! @todo Handle Mouse Left
						break;
						
					case sf::Event::JoystickButtonPressed:
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->JoystickButtonPressed( event.JoystickButton.JoystickId, event.JoystickButton.Button );
							}
						}
						break;
						
					case sf::Event::JoystickButtonReleased:
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->JoystickButtonReleased( event.JoystickButton.JoystickId, event.JoystickButton.Button );
							}
						}
						break;
						
					case sf::Event::JoystickMoved:
						for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
							if( i->GetType() != entity_string ) {
								continue;
							}
							for( auto j = i->Begin(); j != i->End(); ++j ) {
								reinterpret_cast< Entity* >( ( *j ).get() )->JoystickMoved( event.JoystickMove.JoystickId, event.JoystickMove.Axis, event.JoystickMove.Position );
							}
						}
						break;
						
					case sf::Event::JoystickConnected:
						//! @todo Handle Joystick Connected
						break;
						
					case sf::Event::JoystickDisconnected:
						//! @todo Handle Joystick Disconnected
						break;
						
					default:
						// In case of new events in SFML
						break;
				}
			}
			
			for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
				for( auto j = i->Begin(); j != i->End(); ++j ) {
					( *j )->Update();
				}
			}
			
			m_window.Clear();
			
			for( auto i = LayersBegin(); i != LayersEnd(); ++i ) {
				for( auto j = i->Begin(); j != i->End(); ++j ) {
					( *j )->Render( m_window );
				}
			}
			
			m_window.Display();
		}
	}
}
