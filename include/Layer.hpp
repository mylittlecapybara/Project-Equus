#pragma once

#include <memory>
#include <NonCopyable.hpp>
#include <vector>

namespace terra {
	class Object;
	
	class Layer{
		public:
			Layer();
			~Layer();
			
			void Add( std::shared_ptr< Object > NewObject );
			std::vector< std::shared_ptr< Object > >::iterator Begin();
			std::vector< std::shared_ptr< Object > >::iterator End();
			std::shared_ptr< Object > Get( unsigned int Index );
			const unsigned int GetSize() const;
			const std::string& GetType();
			void Remove( unsigned int Index );
			void Remove( std::shared_ptr< Object > Pointer );
		
		private:
			std::vector< std::shared_ptr< Object > > m_objects;
	};
}
