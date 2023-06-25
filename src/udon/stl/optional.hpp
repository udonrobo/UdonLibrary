//-----------------------------------------------
//
//	UdonLibrary
// 
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  optional 
//
//-----------------------------------------------


#pragma once

namespace udon
{

    struct nullopt_t
    {
    };

    constexpr nullopt_t nullopt{};

    template <typename T>
    class optional
    {

        T    m_value;
        bool m_hasValue;

    public:
        constexpr optional()
            : m_value()
            , m_hasValue()
        {
        }

        constexpr optional(nullopt_t)
            : m_value()
            , m_hasValue()
        {
        }

        constexpr optional(const T& value)
            : m_value(value)
            , m_hasValue(true)
        {
        }

        constexpr optional(T&& value)
            : m_value(std::move(value))
            , m_hasValue(true)
        {
        }

        constexpr optional(const optional& other)
            : m_value(other.m_value)
            , m_hasValue(other.m_hasValue)
        {
        }

        constexpr optional(optional&& other) noexcept
            : m_value(std::move(other.m_value))
            , m_hasValue(other.m_hasValue)
        {
        }

        optional& operator=(nullopt_t)
        {
            m_hasValue = false;
            return *this;
        }
        optional& operator=(const optional& other)
        {
            m_value    = other.m_value;
            m_hasValue = other.m_hasValue;
            return *this;
        }
        optional& operator=(optional&& other)
        {
            m_value    = std::move(other.m_value);
            m_hasValue = other.m_hasValue;
            return *this;
        }

		constexpr explicit operator bool() const
		{
			return m_hasValue;
		}

		constexpr const T& operator*() const
		{
			return m_value;
		}

		T& operator*()
		{
			return m_value;
		}

		constexpr const T* operator->() const
		{
			return &m_value;
		}

		T* operator->()
		{
			return &m_value;
		}

		constexpr bool has_value() const
		{
			return m_hasValue;
		}

		constexpr const T& value() const
		{
			return m_value;
		}

		T& value()
		{
			return m_value;
		}

		constexpr const T& value_or(const T& defaultValue) const
		{
			return m_hasValue ? m_value : defaultValue;
		}

		T& value_or(T& defaultValue)
		{
			return m_hasValue ? m_value : defaultValue;
		}

	};

}
