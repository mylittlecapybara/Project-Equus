#include <NonCopyable.hpp>

namespace terra {
	NonCopyable::NonCopyable() {
	}
	
	NonCopyable::NonCopyable( const NonCopyable& ) {
	}
	
	const NonCopyable& NonCopyable::operator=( const NonCopyable& ) {
		return *this;
	}
}
