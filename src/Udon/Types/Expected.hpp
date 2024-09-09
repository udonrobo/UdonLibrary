#pragma once

#include <utility>
#include <type_traits>
#include <new>

namespace Udon
{

#if UDON_PLATFORM_ENABLE_EXCEPTION
    struct BadExpectedAccess : public std::exception
    {
        const char* what() const noexcept override
        {
            return "Bad Expected Access";
        }
    };
    inline void ThrowBadExpectedAccess(bool throwException)
    {
        if (throwException)
        {
            throw BadExpectedAccess{};
        }
    }
#else
    inline void ThrowBadExpectedAccess(bool throwException)
    {
    }
#endif


    template <typename E>
    class Unexpected
    {
    public:
        constexpr Unexpected(const E& error) noexcept(std::is_nothrow_copy_constructible<E>::value)
            : error(error)
        {
        }

        constexpr Unexpected(E&& error) noexcept(std::is_nothrow_move_constructible<E>::value)
            : error(std::move(error))
        {
        }

        constexpr E& value() noexcept { return error; }

        constexpr const E& value() const noexcept { return error; }

    private:
        E error;
    };


    template <typename T, typename E>
    class Expected
    {
        static_assert(std::is_nothrow_destructible<T>::value, "T must be nothrow destructible");

    public:
        constexpr Expected() noexcept
            : hasValue(false)
        {
        }


        constexpr Expected(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value)
            : hasValue(true)
        {
            setValue(value);
        }

        constexpr Expected(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
            : hasValue(true)
        {
            setValue(std::move(value));
        }


        constexpr Expected(const Unexpected<E>& error) noexcept(std::is_nothrow_copy_constructible<E>::value)
            : hasValue(false)
        {
            setError(error.value());
        }

        constexpr Expected(Unexpected<E>&& error) noexcept(std::is_nothrow_move_constructible<E>::value)
            : hasValue(false)
        {
            setError(std::move(error).value());
        }


        ~Expected() noexcept
        {
            reset();
        }


        template <typename U, typename W>
        constexpr Expected(const Expected<U, W>& other) noexcept(std::is_nothrow_copy_constructible<U>::value && std::is_nothrow_copy_constructible<W>::value)
            : hasValue(other.valueExists())
        {
            if (hasValue)
            {
                setValue(other.value());
            }
            else
            {
                setError(other.error());
            }
        }

        template <typename U, typename W>
        constexpr Expected(Expected<U, W>&& other) noexcept(std::is_nothrow_move_constructible<U>::value && std::is_nothrow_move_constructible<W>::value)
            : hasValue(std::move(other).valueExists())
        {
            if (hasValue)
            {
                setValue(std::move(other).value());
            }
            else
            {
                setError(std::move(other).error());
            }
        }


        template <typename U, typename W>
        constexpr Expected& operator=(const Expected<U, W>& other) noexcept(std::is_nothrow_copy_constructible<U>::value && std::is_nothrow_copy_constructible<W>::value)
        {
            if (this != &other)
            {
                reset();
                hasValue = other.hasValue;
                if (hasValue)
                {
                    setValue(other.value());
                }
                else
                {
                    setError(other.error());
                }
            }
            return *this;
        }

        template <typename U, typename W>
        constexpr Expected& operator=(Expected<U, W>&& other) noexcept(std::is_nothrow_move_constructible<U>::value && std::is_nothrow_move_constructible<W>::value)
        {
            if (this != &other)
            {
                reset();
                hasValue = std::move(other).hasValue;
                if (hasValue)
                {
                    setValue(std::move(other).value());
                }
                else
                {
                    setError(std::move(other).error());
                }
            }
            return *this;
        }


        constexpr Expected& operator=(const T& value)
        {
            static_assert(!std::is_same<T, E>::value, "Ambiguous call to operator=()");

            hasValue = true;
            setValue(value);

            return *this;
        }

        constexpr Expected& operator=(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
        {
            static_assert(!std::is_same<T, E>::value, "Ambiguous call to operator=()");

            hasValue = true;
            setValue(std::move(value));

            return *this;
        }


        constexpr Expected& operator=(const E& error) noexcept(std::is_nothrow_copy_constructible<E>::value)
        {
            static_assert(!std::is_same<T, E>::value, "Ambiguous call to operator=()");

            hasValue = false;
            setError(error);

            return *this;
        }

        constexpr Expected& operator=(E&& error) noexcept(std::is_nothrow_move_constructible<E>::value)
        {
            static_assert(!std::is_same<T, E>::value, "Ambiguous call to operator=()");

            hasValue = false;
            setError(std::move(error));

            return *this;
        }


        constexpr bool valueExists() const noexcept { return hasValue; }


        constexpr T& value()
        {
            ThrowBadExpectedAccess(!hasValue);
            return storage.value;
        }

        constexpr const T& value() const
        {
            ThrowBadExpectedAccess(!hasValue);
            return storage.value;
        }


        constexpr E& error()
        {
            ThrowBadExpectedAccess(hasValue);
            return storage.error;
        }

        constexpr const E& error() const
        {
            ThrowBadExpectedAccess(hasValue);
            return storage.error;
        }


        constexpr T value_or(T&& default_value) const noexcept(std::is_nothrow_move_constructible<T>::value)
        {
            return hasValue ? storage.value : std::move(default_value);
        }

        constexpr E error_or(E&& default_error) const noexcept(std::is_nothrow_move_constructible<E>::value)
        {
            return hasValue ? std::move(default_error) : storage.error;
        }


        constexpr T* operator->()
        {
            ThrowBadExpectedAccess(!hasValue);
            return &storage.value;
        }

        constexpr const T* operator->() const
        {
            ThrowBadExpectedAccess(!hasValue);
            return &storage.value;
        }


        constexpr T& operator*()
        {
            ThrowBadExpectedAccess(!hasValue);
            return storage.value;
        }

        constexpr const T& operator*() const
        {
            ThrowBadExpectedAccess(!hasValue);
            return storage.value;
        }


        constexpr operator bool() const noexcept
        {
            return hasValue;
        }


        constexpr friend void swap(Expected& first, Expected& second) noexcept
        {
            if (first.hasValue)
            {
                if (second.hasValue)
                {
                    std::swap(first.storage.value, second.storage.value);
                }
                else
                {
                    E temp = std::move(second.storage.error);
                    new (&second.storage.value) T(std::move(first.storage.value));
                    new (&first.storage.error) E(std::move(temp));
                }
            }
            else
            {
                if (second.hasValue)
                {
                    T temp = std::move(second.storage.value);
                    new (&second.storage.error) E(std::move(first.storage.error));
                    new (&first.storage.value) T(std::move(temp));
                }
                else
                {
                    std::swap(first.storage.error, second.storage.error);
                }
            }
            std::swap(first.hasValue, second.hasValue);
        }


        template <typename F>
        constexpr auto and_then(F&& f) -> decltype(f(std::declval<T>()))
        {
            static_assert(std::is_same<decltype(f(std::declval<T>())), Expected<T, E>>::value, "The return value of the function argument must be `Expected<T,E>`.");

            if (hasValue)
            {
                return f(value());
            }
            else
            {
                return Unexpected<E>(error());
            }
        }

        template <typename F>
        constexpr auto or_else(F&& f) -> decltype(f(std::declval<E>()))
        {
            static_assert(std::is_same<decltype(f(std::declval<E>())), Expected<T, E>>::value, "The return value of the function argument must be `Expected<T,E>`.");

            if (hasValue)
            {
                return value();
            }
            else
            {
                return f(error());
            }
        }

        template <typename F>
        constexpr auto transform(F&& f) -> Expected<decltype(f(std::declval<T>())), E>
        {
            if (hasValue)
            {
                return Expected<decltype(f(std::declval<T>())), E>{ f(value()) };
            }
            else
            {
                return Unexpected<E>{ error() };
            }
        }

        template <typename F>
        constexpr auto transform_error(F&& f) -> Expected<T, decltype(f(std::declval<E>()))>
        {
            if (hasValue)
            {
                return value();
            }
            else
            {
                return Unexpected<decltype(f(std::declval<E>()))>{ f(error()) };
            }
        }


    private:
        bool hasValue;

        union Storage
        {
            T value;
            E error;
            constexpr Storage() noexcept {}
            ~Storage() noexcept {}
        } storage;

        constexpr void reset() noexcept
        {
            if (hasValue)
            {
                storage.value.~T();
            }
            else
            {
                storage.error.~E();
            }
            hasValue = false;
        }

        template <typename U>
        constexpr void setValue(U&& value) noexcept
        {
            new (&storage.value) T(static_cast<T>(std::forward<U>(value)));
        }

        template <typename U>
        constexpr void setError(U&& error) noexcept
        {
            new (&storage.error) E(static_cast<E>(std::forward<U>(error)));
        }
    };


    template <typename T, typename E>
    Expected<T, E> make_expected(const T& value)
    {
        return Expected<T, E>(value);
    }

    template <typename T, typename E>
    Expected<T, E> make_expected(T&& value)
    {
        return Expected<T, E>(std::move(value));
    }

    template <typename T, typename E>
    Expected<T, E> make_Unexpected(const E& error)
    {
        return Expected<T, E>(Unexpected<E>(error));
    }

    template <typename T, typename E>
    Expected<T, E> make_Unexpected(E&& error)
    {
        return Expected<T, E>(Unexpected<E>(std::move(error)));
    }
}    // namespace Udon