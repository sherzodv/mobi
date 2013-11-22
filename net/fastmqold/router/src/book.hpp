#ifndef fastmq_router_book_hpp
#define fastmq_router_book_hpp

#include <stack>
#include <vector>

template <class Item>
class book {

	std::vector<Item *> m_book;
	std::stack<std::size_t> m_hole;

	public:
		book() {
			m_book.reserve(10);
		}

		virtual ~book() {
			for (Item * item: m_book)
				delete item;
		}

		template<typename ... Args>
		Item * create(Args & ... args) {
			std::size_t idx = 0;
			if (!m_hole.empty()) {
				idx = m_hole.top();
				m_hole.pop();
				return new Item(idx, args ...);
			} else {
				idx = m_book.size();
				Item * item = new Item(idx, args ...);
				m_book.push_back(item);
				return item;
			}
		}

		void destroy(Item * item) {
			m_book.at(item->index()) = nullptr;
			m_hole.push(item->index());
			delete item;
		}

		Item * get(std::size_t idx) {
			return m_book.at(idx);
		}

		template <typename R, typename ... ArgsT>
		void for_each_call(R (Item::* mfunc)(ArgsT ...), ArgsT ... args) {
			for (Item * item: m_book) {
				(item->*mfunc)(args ...);
			}
		}
};

#endif
