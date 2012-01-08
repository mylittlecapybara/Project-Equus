#include <Layer.hpp>
#include <Object.hpp>

namespace terra {
	Layer::Layer() {
	}
	
	Layer::~Layer() {
	}
	
	void Layer::Add( std::shared_ptr< Object > NewObject ) {
		if( NewObject->GetType() == GetType() || GetSize() == 0 ) {
			m_objects.push_back( NewObject );
		}
	}
	
	std::vector< std::shared_ptr< Object > >::iterator Layer::Begin() {
		return m_objects.begin();
	}
	
	std::vector< std::shared_ptr< Object > >::iterator Layer::End() {
		return m_objects.end();
	}
	
	std::shared_ptr< Object > Layer::Get( unsigned int Index ) {
		if( Index < GetSize() ) {
			return m_objects.at( Index );
		}
	}
	
	const unsigned int Layer::GetSize() const {
		return m_objects.size();
	}
	
	const std::string& Layer::GetType() {
		static std::string Empty( "" );
		if( GetSize() == 0 ) {
			return Empty;
		}
		return Get( 0 )->GetType();
	}
	
	void Layer::Remove( unsigned int Index ) {
		if( Index < GetSize() ) {
			m_objects.erase( Begin() + Index );
		}
	}
	
	void Layer::Remove( std::shared_ptr< Object > Pointer ) {
		for( auto i = Begin(); i != End(); ++i ) {
			if( *i == Pointer ) {
				m_objects.erase( i );
			}
		}
	}
}
