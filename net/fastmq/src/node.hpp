#ifndef fastmq_node_hpp
#define fastmq_node_hpp

#include <stack>
#include <vector>
#include <cstddef>

#include "terminal.hpp"

namespace fastmq {

	class node_base {

		public:
			node_base() {
			}

			virtual ~node_base() {
			}

		protected:
			virtual void on_connect_error() = 0;
			virtual void on_recv(terminal_base * p, msgu * msg) = 0;
			virtual void on_send(terminal_base * p, msgu * msg) = 0;
			virtual void on_recv_error(terminal_base * p) = 0;
			virtual void on_send_error(terminal_base * p, msgu * msg) = 0;
			virtual bool register_terminal(terminal_base * p) = 0;
			virtual void unregister_terminal(terminal_base * p) = 0;
	};

}

#endif
