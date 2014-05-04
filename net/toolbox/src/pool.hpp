
#ifndef mobi_net_toolbox_pool_hpp
namespace mobi { namespace net { namespace toolbox { namespace concurrent {
	class pool_allocator {

		public:
			pool_allocator(std::size_t initial_size) {

				pbegin = cur = new char [initial_size];// static_cast<char*>(malloc(initial_size));
				pend = pbegin + initial_size;
			}

			~pool_allocator() {
				
				free(static_cast<void*>(pbegin));
			}

			std::size_t size() {
				return static_cast<std::size_t>(pend - pbegin);
			}

			//https://github.com/glenfe/align/blob/master/include/boost/align/detail/align.hpp
			inline
			void * align(	std::size_t alignment,
							std::size_t size,
							void *& ptr,
							std::size_t & space) {

				std::size_t n = reinterpret_cast<std::size_t>(ptr) & (alignment - 1);

				if (n != 0) {
					n = alignment - n;
				}

				void * p = 0;
				if (n <= space && size <= space - n) {
					p = static_cast<char*>(ptr) + n;
					ptr = p;
					space -= n;
				}

				return p;
			}

			template <class T>
			T * alloc(std::size_t num) {
	 
				std::size_t al = alignof(T);
				std::size_t sz = num * sizeof(T);
				void * tcur = static_cast<void *>(cur);
				std::size_t space = static_cast<std::size_t>(pend-cur);

				if (align(al, sz, tcur, space)) {

					cur = static_cast<char*>(tcur) + sz; 
					return static_cast<T *>(tcur);

				} else {
					return NULL;
				}
			}

			void state() {
				std::cout << std::endl;
				std::cout << "pool head: " << (int*)(pbegin) << std::endl;
				std::cout << "float pos: " << (int*)(cur) << std::endl;
				std::cout << "pool tail: " << (int*)(pend) << std::endl;
				std::cout << std::endl;
			}

		private:
			char * pbegin;
			char * cur;
			char * pend;
	};
} } } };


