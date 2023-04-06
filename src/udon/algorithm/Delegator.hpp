#pragma once

#include <vector>

/// @brief 単一のメンバ関数を管理するデリゲータ
namespace udon
{

	template<class> class Delegator;  // 前方宣言

	/// @brief 単一のメンバ関数を管理するデリゲータ
	template<class R, class... Args>
	class Delegator<R(Args...)>
	{

		static Delegator* p;

	public:

		template<class T = R>
		static auto Execute(Args... args) -> typename std::enable_if<std::is_same<T, void>::value, R>::type
		{
			if (p)
			{
				p->callback(std::forward<Args>(args)...);
			}
		}

		template<class T = R>
		static auto Execute(Args... args) -> typename std::enable_if<!std::is_same<T, void>::value, R>::type
		{
			if (p)
			{
				return p->callback(std::forward<Args>(args)...);
			}
			else
			{
				return {};
			}
		}

	protected:

		Delegator()
		{
			p = this;
		}

		virtual R callback(Args&&...) = 0;

	};

	template<class R, class... Args>
	Delegator<R(Args...)>* Delegator<R(Args...)>::p = nullptr;

}



/// @brief 複数のメンバ関数を管理するデリゲータ
namespace udon
{

	template<class> class MultipleDelegator;  // 前方宣言

	/// @brief 複数のメンバ関数を管理するデリゲータ
	/// @tparam R 静的関数の戻り値
	/// @tparam ...Args 静的関数の引数
	template<class R, class... Args>
	class MultipleDelegator<R(Args...)>
	{

		static std::vector<MultipleDelegator*> pList;

	public:

		template<class T = R>
		static auto Execute(Args... args) -> typename std::enable_if<std::is_same<T, void>::value, R>::type
		{
			if (pList.empty())
			{
				return;
			}
			for (auto&& it : pList)
			{
				it->callback(std::forward<Args>(args)...);
			}
		}

		template<class T = R>
		static auto Execute(Args... args) -> typename std::enable_if<!std::is_same<T, void>::value, R>::type
		{
			if (pList.empty())
			{
				return {};
			}
			R retval;
			for (auto&& it : pList)
			{
				retval = it->callback(std::forward<Args>(args)...);
			}
			return retval;
		}

	protected:

		MultipleDelegator()
		{
			pList.push_back(this);
		}

		virtual R callback(Args&&...) = 0;

	};

	template<class R, class... Args>
	std::vector<MultipleDelegator<R(Args...)>*> MultipleDelegator<R(Args...)>::pList;


}
