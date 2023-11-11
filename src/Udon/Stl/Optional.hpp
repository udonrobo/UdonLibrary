//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    Optional
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Common/Show.hpp>

namespace Udon
{

    struct Nullopt_t
    {
    };

    constexpr Nullopt_t nullopt{};

    template <typename T>
    class Optional
    {

        T    m_value;
        bool m_hasValue;

    public:
        constexpr Optional()
            : m_value()
            , m_hasValue()
        {
        }

        constexpr Optional(Nullopt_t)
            : m_value()
            , m_hasValue()
        {
        }

        constexpr Optional(const T& value)
            : m_value(value)
            , m_hasValue(true)
        {
        }

        constexpr Optional(T&& value)
            : m_value(std::move(value))
            , m_hasValue(true)
        {
        }

        constexpr Optional(const Optional& other)
            : m_value(other.m_value)
            , m_hasValue(other.m_hasValue)
        {
        }

        constexpr Optional(Optional&& other) noexcept
            : m_value(std::move(other.m_value))
            , m_hasValue(other.m_hasValue)
        {
        }

        /// @brief Optional<T> から Optional<OtherT> へのキャスト(左辺値)
        template <typename OtherT>
        constexpr operator Optional<OtherT>() const&
        {
            return m_hasValue ? Optional<OtherT>{ static_cast<OtherT>(m_value) }
                              : nullopt;
        }

        /// @brief Optional<T> から Optional<OtherT> へのキャスト(右辺値)
        template <typename OtherT>
        constexpr operator Optional<OtherT>() const&&
        {
            return m_hasValue ? Optional<OtherT>{ std::move(static_cast<OtherT>(m_value)) }
                              : nullopt;
        }

        /// @brief Optional<OtherT> を Optional<T> に代入(左辺値)
        template <typename OtherT>
        Optional& operator=(const Optional<OtherT>& other)
        {
            m_value    = static_cast<T>(other.m_value);
            m_hasValue = other.m_hasValue;
            return *this;
        }

        /// @brief Optional<OtherT> を Optional<T> に代入(右辺値)
        template <typename OtherT>
        Optional& operator=(Optional<OtherT>&& other) noexcept
        {
            m_value    = std::move(static_cast<T>(other.m_value));
            m_hasValue = other.m_hasValue;
            return *this;
        }

        Optional& operator=(Nullopt_t)
        {
            m_hasValue = false;
            return *this;
        }
        Optional& operator=(const Optional& other)
        {
            m_value    = other.m_value;
            m_hasValue = other.m_hasValue;
            return *this;
        }
        Optional& operator=(Optional&& other) noexcept
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

        void reset()
        {
            m_hasValue = false;
        }

        void show() const
        {
            if (m_hasValue)
            {
                Udon::Show(m_value);
            }
            else
            {
                Udon::Show("nullopt");
            }
        }
    };

}    // namespace Udon
