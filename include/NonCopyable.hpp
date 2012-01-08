#pragma once

namespace terra {
	class NonCopyable {
		protected:
			NonCopyable();
		
		private:
			NonCopyable( const NonCopyable& );
			const NonCopyable& operator=( const NonCopyable& );
	};
}
