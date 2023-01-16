#pragma once

namespace udon {

	namespace std {

		template<class Ty>
		class shared_ptr
		{

			Ty* p;           // 管理するポインタ
			long* counter;   // 所有権共有数

		public:

			constexpr shared_ptr()
				: p()
				, counter()
			{}

			constexpr shared_ptr(Ty* p)
				: p(p)
				, counter(new long(1))
			{}

			constexpr shared_ptr(const shared_ptr& rhs) noexcept
				: p(rhs.p)
				, counter(rhs.counter)
			{
				++(*counter);
			}

			~shared_ptr() noexcept
			{
				if (--(*counter));
				else
				{
					delete p;
					delete counter;
				}
			}

			constexpr explicit operator bool() const noexcept
			{
				return static_cast<bool>(p);
			}

			constexpr shared_ptr& operator=(const shared_ptr& rhs) noexcept
			{
				if (this == &rhs);
				else
				{
					p = rhs.p;
					counter = rhs.counter;
					++(*counter);
				}
				return *this;
			}

			constexpr long use_count() const noexcept
			{
				return *counter;
			}

			constexpr Ty& operator*() noexcept
			{
				return *p;
			}
			constexpr const Ty& operator*() const noexcept
			{
				return *p;
			}

			constexpr Ty* operator->() noexcept
			{
				return p;
			}
			constexpr const Ty* operator->() const noexcept
			{
				return p;
			}

		};
	}

}

#ifdef ENABLE_STL

namespace std {

	template <class Ty>
	inline void swap(udon::stl::shared_ptr<Ty>& lhs, udon::stl::shared_ptr<Ty>& rhs) noexcept {
		auto&& temp = rhs;
		rhs = lhs;
		lhs = temp;
	}

}

#endif