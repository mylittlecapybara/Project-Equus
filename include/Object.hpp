#pragma once

#include <map>
#include <memory>
#include <NonCopyable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <string>

namespace terra {
	class Engine;
	class Layer;
	
	class Object : public NonCopyable {
		public:
			friend class Engine;
			friend class Layer;
			
			Object();
			virtual ~Object();
			
			virtual void Update();
			virtual void Render( sf::RenderTarget& Target );
		
		protected:
			virtual const std::string& GetType() const = 0;
	};
}
