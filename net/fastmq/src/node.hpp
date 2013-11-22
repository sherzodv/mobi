#ifndef fastmq_node_hpp
#define fastmq_node_hpp

#include <stack>
#include <vector>
#include <cstddef>

#include "subscriber.hpp"

namespace fastmq {

	template <class PeerT>
	class node_base {

		friend PeerT;
		typedef PeerT peer_t;

		std::vector<peer_t *> m_book;
		std::stack<std::size_t> m_hole;

		public:
			node_base() {
				m_book.reserve(10);
			}

			virtual ~node_base() {
				for (peer_t * p: m_book)
					destroy(p);
			}

			virtual msgu * create_message(std::size_t len = 0) = 0;
			virtual void destroy_message(msgu * const msg) = 0;

			virtual void on_message_recv(PeerT * peer, msgu * const msg) = 0;
			virtual void on_message_send(PeerT * peer, msgu * const msg) = 0;

			virtual void on_send_error(PeerT * peer) = 0;
			virtual void on_recv_error(PeerT * peer) = 0;

		protected:
			template<typename ... Args>
			peer_t * create(Args & ... args) {
				std::size_t idx = 0;
				peer_t * p = new peer_t(idx, args ...);
				if (!m_hole.empty()) {
					idx = m_hole.top();
					m_hole.pop();
					m_book[idx] = p;
				} else {
					idx = m_book.size();
					m_book.push_back(p);
				}
				return p;
			}

			void destroy(peer_t * p) {
				m_book.at(p->m_idx) = nullptr;
				m_hole.push(p->m_idx);
				delete p;
			}
	};

}

#endif
