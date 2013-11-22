#ifndef smpp_esme_session_manager
#define smpp_esme_session_manager

namespace smpp { namespace esme {

	template <class SessionPtrT>
	class session_manager {
		public:
			session_manager(){}
			virtual ~session_manager(){}

			virtual void add(SessionPtrT sptr) = 0;
			virtual void remove(SessionPtrT sptr) = 0;
	};

} }

#endif
