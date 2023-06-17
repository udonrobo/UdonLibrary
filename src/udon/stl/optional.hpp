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
        optional()
            : m_value()
            , m_hasValue()
        {
        }

        optional(nullopt_t)
            : m_value()
            , m_hasValue()
        {
        }

        optional(const T& value)
            : m_value(value)
            , m_hasValue(true)
        {
        }

        optional(T&& value)
            : m_value(std::move(value))
            , m_hasValue(true)
        {
        }

        optional(const optional& other)
            : m_value(other.m_value)
            , m_hasValue(other.m_hasValue)
        {
        }

        optional(optional&& other)
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

        operator bool() const
        {
            return m_hasValue;
        }

        const T& operator*() const
        {
            return m_value;
        }

        T& operator*()
        {
            return m_value;
        }

        const T* operator->() const
        {
            return &m_value;
        }

        T* operator->()
        {
            return &m_value;
        }

        bool has_value() const
        {
            return m_hasValue;
        }

        const T& value() const
        {
            return m_value;
        }

        T& value()
        {
            return m_value;
        }

        const T& value_or(const T& defaultValue) const
        {
            return m_hasValue ? m_value : defaultValue;
        }

        T& value_or(T& defaultValue)
        {
            return m_hasValue ? m_value : defaultValue;
        }
    };

}    // namespace udon