#ifndef mobi_net_toolbox_queue_hpp
#define mobi_net_toolbox_queue_hpp

#include <mutex>
#include <queue>
#include <condition_variable>

namespace mobi { namespace net { namespace toolbox { namespace concurrent {

	/* Dummy concurrent queue implementation taken from
	 * "C++ Concurrency in Action" Anthony Williams.
	 * Do not use "return value pop" due to possible
	 * value loss on case of exceptions during copy construction */

	template <typename T>
	class queue {
		private:
			mutable std::mutex m_mtx;
			std::queue<T> m_que;
			std::condition_variable m_cond;

		public:
			queue() {}

			void push(T v) {
				std::lock_guard<std::mutex> lock(m_mtx);
				m_que.push(v);
				m_cond.notify_one();
			}

			void pop(T & v) {
				std::unique_lock<std::mutex> lock(m_mtx);
				m_cond.wait(lock, [this] { return !m_que.empty(); });
				v = std::move(m_que.front());
				m_que.pop();
			}

			bool try_pop(T & v) {
				std::lock_guard<std::mutex> lock(m_mtx);
				if (m_que.empty()) {
					return false;
				}
				v = std::move(m_que.front());
				m_que.pop();
				return true;
			}

			bool empty() const {
				std::lock_guard<std::mutex> lock(m_mtx);
				return m_que.empty();
			}
	};

} } } }

#endif
