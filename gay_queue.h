#include "gay_deque.h"

namespace uvwxyz
{
	namespace gay_stl
	{
		template <class T, class Sequence = gay_deque<T> >
		class gay_queue
		{
		public:
			typedef typename Sequence::value_type value_type;
			typedef typename Sequence::size_type size_type;
			typedef typename Sequence::reference reference;
			typedef typename Sequence::const_reference const_reference;

		protected:
			Sequence c;

		public:
			bool empty() const { return c.empty(); }
			size_type size() const { return c.size(); }
			const_reference front() const { return c.front(); }
			reference front() { return c.front(); }
			const_reference back() const { return c.back(); }
			reference back() { return c.back(); }
			void push(const_reference x) { c.push_back(x); }
			void pop() { c.pop_front(); }

		private:
			template <class _T, class _Sequence>
            friend bool operator==
            (const gay_queue<_T, _Sequence>& lhs, const gay_queue<_T, _Sequence>& rhs);
            template <class _T, class _Sequence>
            friend bool operator<
            (const gay_queue<_T, _Sequence>& lhs, const gay_queue<_T, _Sequence>& rhs);

		};

		template <class _T, class _Sequence>
		bool operator==(const gay_queue<_T, _Sequence>& lhs, const gay_queue<_T, _Sequence>& rhs)
		{
			return lhs.c == rhs.c;
		}
		template <class _T, class _Sequence>
		bool operator<(const gay_queue<_T, _Sequence>& lhs, const gay_queue<_T, _Sequence>& rhs)
		{
			return lhs.c < rhs.c;
		}
	}
}
