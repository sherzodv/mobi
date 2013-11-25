#ifndef fastmq_router_hpp
#define fastmq_router_hpp

#include <stack>
#include <vector>
#include <cstddef>
#include <unordered_map>

#include "pool.hpp"
#include "terminal.hpp"

namespace fastmq {

	struct route {
		enum type { on_id, on_type, on_idtype };
	};

	class router_base {
		public:
			router_base() {}
			virtual ~router_base() {}

			virtual void route_message(msgu * msg) = 0;

			bool route_add(route::type r, terminal_base * s) {
				if (subscribe(r, s)) {
					s->m_router = this;
					return true;
				} else {
					return false;
				}
			}

			bool route_del(route::type r, terminal_base * s) {
				s->m_router = nullptr;
				return unsubscribe(r, s);
			}

			bool route_add(terminal_base * s) {
				return route_add(route::on_idtype, s);
			}

			bool route_del(terminal_base * s) {
				return route_del(route::on_idtype, s);
			}

			void route_clear(terminal_base * s) {
				route_del(route::on_id, s);
				route_del(route::on_type, s);
				route_del(route::on_idtype, s);
			}

		protected:
			virtual bool subscribe(route::type r, terminal_base * s) = 0;
			virtual bool unsubscribe(route::type r, terminal_base * s) = 0;
	};

	class router: public router_base {

		message_pool_base & P;

		typedef std::unordered_multimap<fastmq::u16_t
			, terminal_base *> map16_t;
		typedef std::unordered_multimap<fastmq::u32_t
			, terminal_base *> map32_t;

		typedef map16_t::iterator iter16;
		typedef map32_t::iterator iter32;

		typedef std::pair<iter16, iter16> range16;
		typedef std::pair<iter32, iter32> range32;

		map16_t m_byid;
		map16_t m_bytype;
		map32_t m_byidtype;

		fastmq::u32_t make_key(fastmq::u16_t id, fastmq::u16_t type) {
			return ( id << 16 ) | type;
		}

		terminal_base * find_destination(terminal_base * s) {
			map16_t::iterator item16;
			map32_t::iterator item32;

			item32 = m_byidtype.find(make_key(s->id(), s->type()));
			if (item32 != m_byidtype.end())
				return item32->second;

			item16 = m_byid.find(s->id());
			if (item16 != m_byid.end())
				return item16->second;

			item16 = m_bytype.find(s->type());
			if (item16 != m_bytype.end())
				return item16->second;

			return nullptr;
		}

		bool subscribe_by_id(terminal_base * s) {
			range16 r = m_byid.equal_range(s->id());
			iter16 i = std::find_if(r.first, r.second
				, [s](const iter16::value_type & i){ return i.second == s; });
			if (i != r.second) {
				/* route exists */
				return false;
			}
			m_byid.insert(std::make_pair(s->id(), s));
			return true;
		}

		bool subscribe_by_type(terminal_base * s) {
			range16 r = m_bytype.equal_range(s->id());
			iter16 i = std::find_if(r.first, r.second
				, [s](const iter16::value_type & i){ return i.second == s; });
			if (i != r.second) {
				/* route exists */
				return false;
			}
			m_bytype.insert(std::make_pair(s->type(), s));
			return true;
		}

		bool subscribe_by_idtype(terminal_base * s) {
			range32 r = m_byidtype.equal_range(make_key(s->id(), s->type()));
			iter32 i = std::find_if(r.first, r.second
				, [s](const iter32::value_type & i){ return i.second == s; });
			if (i != r.second) {
				/* route exists */
				return false;
			}
			m_byidtype.insert(std::make_pair(make_key(s->id(), s->type()), s));
			return true;
		}

		bool unsubscribe_by_id(terminal_base * s) {
			range16 r = m_byid.equal_range(s->id());
			iter16 i = std::find_if(r.first, r.second
				, [s](const iter16::value_type & i){ return i.second == s; });
			if (i == r.second) {
				/* route doesn't exist */
				return false;
			}
			m_byid.erase(i);
			return true;
		}

		bool unsubscribe_by_type(terminal_base * s) {
			range16 r = m_bytype.equal_range(s->id());
			iter16 i = std::find_if(r.first, r.second
				, [s](const iter16::value_type & i){ return i.second == s; });
			if (i == r.second) {
				/* route doesn't exist */
				return false;
			}
			m_bytype.erase(i);
			return true;
		}

		bool unsubscribe_by_idtype(terminal_base * s) {
			range32 r = m_byidtype.equal_range(make_key(s->id(), s->type()));
			iter32 i = std::find_if(r.first, r.second
				, [s](const iter32::value_type & i){ return i.second == s; });
			if (i == r.second) {
				/* route doesn't exist */
				return false;
			}
			m_byidtype.erase(i);
			return true;
		}

		virtual bool subscribe(route::type r, terminal_base * s) {
			switch (r) {
				case route::on_id:		return subscribe_by_id(s);
				case route::on_type:	return subscribe_by_type(s);
				case route::on_idtype:	return subscribe_by_idtype(s);
				default:		return false; /* Unknown route type */
			}
		}

		virtual bool unsubscribe(route::type r, terminal_base * s) {
			switch (r) {
				case route::on_id:		return unsubscribe_by_id(s);
				case route::on_type:	return unsubscribe_by_type(s);
				case route::on_idtype:	return unsubscribe_by_idtype(s);
				default:		return false; /* Unknown route type */
			}
		}

		public:
			router(message_pool_base & p): P(p) {}
			virtual ~router() {}

			virtual void route_message(msgu * msg) {
				range16 r16;
				range32 r32;

				/* O: the preexisting copy of message could be used
				 * instead of destroying it deliver it to first subscriber */

				r32 = m_byidtype.equal_range(make_key(msg->id, msg->type));
				std::for_each(r32.first, r32.second
					, [msg, this](iter32::value_type & i) {
						msgu * nmsg = P.copy_message(msg);
						/* TODO: handle copy error */
						if (nmsg)
							i.second->process_message(msg);
					});

				r16 = m_byid.equal_range(msg->id);
				std::for_each(r16.first, r16.second
					, [msg, this](iter16::value_type & i) {
						msgu * nmsg = P.copy_message(msg);
						/* TODO: handle copy error */
						if (nmsg)
							i.second->process_message(msg);
					});

				r16 = m_bytype.equal_range(msg->id);
				std::for_each(r16.first, r16.second
					, [msg, this](iter16::value_type & i) {
						msgu * nmsg = P.copy_message(msg);
						/* TODO: handle copy error */
						if (nmsg)
							i.second->process_message(msg);
					});

				P.destroy_message(msg);
			}
	};

}

#endif
