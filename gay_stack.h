
#include "gay_deque.h"

namespace uvwxyz
{
	namespace gay_stl
	{

		template <class T, class Sequence = gay_deque<T> >
		class gay_stack
		{
		public:
			typedef typename Sequence::value_type value_type;
			typedef typename Sequence::reference reference;
			typedef typename Sequence::size_type size_type;
			typedef typename Sequence::const_reference const_reference;

		protected:
			Sequence c;

		public:
			gay_stack() {}
			bool empty() const { return c.empty(); }
			reference top() { return c.back(); }
			const_reference top() const { return c.back(); }
			size_type size() const { return c.size(); }
			void push(const_reference x) { c.push_back(x); }
			void pop() { c. pop_back(); }

			template <class _T, class _Sequence>
            friend bool operator==
            (const gay_stack<_T, _Sequence>& lhs, const gay_stack<_T, _Sequence>& rhs);
            template <class _T, class _Sequence>
            friend bool operator<
            (const gay_stack<_T, _Sequence>& lhs, const gay_stack<_T, _Sequence>& rhs);
		};

		template <class _T, class _Sequence>
		bool operator==(const gay_stack<_T, _Sequence>& lhs, const gay_stack<_T, _Sequence>& rhs)
		{
			return lhs.c == rhs.c;
		}
		template <class _T, class _Sequence>
		bool operator<(const gay_stack<_T, _Sequence>& lhs, const gay_stack<_T, _Sequence>& rhs)
		{
			return lhs.c < rhs.c;
		}
	}
}
