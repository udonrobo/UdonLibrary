//
//    Optional
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <Udon/Utility/Platform.hpp>
#include <Udon/Utility/Show.hpp>
#include <type_traits>
#include <new>

#if __has_include(<new.h>)
#    include <new.h>
#endif

namespace Udon
{

#if UDON_PLATFORM_ENABLE_EXCEPTION
    struct BadOptionalAccess : public std::exception
    {
        const char* what() const noexcept override
        {
            return "Bad Optional Access";
        }
    };
    inline void ThrowBadOptionalAccess(bool throwException)
    {
        if (throwException)
        {
            throw BadOptionalAccess{};
        }
    }
#else
    inline void ThrowBadOptionalAccess(bool /* throwException */)
    {
    }
#endif

    /**
     * @brief 無効値型
     */
    struct NulloptT
    {
    };

    /**
     * @brief 無効値
     */
    constexpr NulloptT nullopt{};


    /**
     * @brief オプショナル型
     * @note C++17のstd::optionalの模倣
     * @tparam T 保持する値の型
     */
    template <typename T>
    class Optional
    {

        static_assert(std::is_nothrow_destructible<T>::value, "T must be nothrow destructible");

        static_assert(not std::is_same<typename std::remove_cv<T>::type, NulloptT>::value, "T must not be the same as NulloptT");

        static_assert(not std::is_array<T>::value, "T must not be an array");

        template <typename>
        friend class Optional;

    public:
        using ValueType = T;

        /**
         * @brief デフォルトコンストラクタ
         */
        constexpr Optional() noexcept
            : mStorage()
            , mHasValue()
        {
        }


        /**
         * @brief 無効値として構築
         * @param
         */
        constexpr Optional(NulloptT)
            : mStorage()
            , mHasValue()
        {
        }


        /**
         * @brief 値から構築
         * @param value
         */
        template <typename U, typename std::enable_if<std::is_constructible<ValueType, U>::value, std::nullptr_t>::type = nullptr>
        Optional(U&& value) noexcept(std::is_nothrow_constructible<ValueType>::value)
            : mStorage()
            , mHasValue(true)
        {
            ConstructValue(std::forward<U>(value));
        }


        /**
         * @brief コピーコンストラクタ
         * @note 同じ型を持つOptionalから構築する場合、このコンストラクタは呼ばれず、暗黙定義のコピーコンストラクタが呼ばれる
         * @param other
         */
        template <typename U, typename std::enable_if<std::is_constructible<ValueType, const U&>::value, std::nullptr_t>::type = nullptr>
        Optional(const Optional<U>& other) noexcept(std::is_nothrow_constructible<ValueType, U>::value)
            : mStorage()
            , mHasValue(other.mHasValue)
        {
            if (mHasValue)
            {
                ConstructValue(other.mStorage.value);
            }
        }


        /**
         * @brief ムーブコンストラクタ
         * @note 同じ型を持つOptionalから構築する場合、このコンストラクタは呼ばれず、暗黙定義のムーブコンストラクタが呼ばれる
         * @param other
         */
        template <typename U, typename std::enable_if<std::is_constructible<ValueType, U&&>::value, std::nullptr_t>::type = nullptr>
        Optional(Optional<U>&& other) noexcept(std::is_nothrow_constructible<ValueType, U>::value)
            : mStorage()
            , mHasValue(other.mHasValue)
        {
            if (mHasValue)
            {
                ConstructValue(std::move(other.mStorage.value));
            }
        }


        /**
         * @brief デストラクタ
         */
        ~Optional() noexcept
        {
            reset();
        }


        /**
         * @brief 無効値代入演算子
         * @param
         * @return
         */
        Optional& operator=(NulloptT) noexcept
        {
            reset();
            return *this;
        }


        /**
         * @brief 値代入演算子
         * @param other
         * @return *this
         */
        // template <typename U>
        template <typename U, typename std::enable_if<std::is_constructible<ValueType, U>::value and std::is_assignable<ValueType, U>::value, std::nullptr_t>::type = nullptr>
        Optional& operator=(U&& value) noexcept(std::is_nothrow_constructible<ValueType, U>::value and std::is_nothrow_assignable<ValueType, U>::value)
        {
            AssignmentValue(std::forward<U>(value));
            return *this;
        }


        /**
         * @brief コピー代入演算子
         * @param other
         */
        template <typename U, typename std::enable_if<std::is_constructible<ValueType, const U&>::value and std::is_assignable<ValueType, const U&>::value, std::nullptr_t>::type = nullptr>
        Optional& operator=(const Optional<U>& other) noexcept(std::is_nothrow_constructible<ValueType, U>::value and std::is_nothrow_assignable<ValueType, U>::value)
        {
            if (other)
            {
                AssignmentValue(other.value());
            }
            else
            {
                reset();
            }
            return *this;
        }


        /**
         * @brief ムーブ代入演算子
         * @param other
         */
        template <typename U, typename std::enable_if<std::is_constructible<ValueType, U&&>::value and std::is_assignable<ValueType, U&&>::value, std::nullptr_t>::type = nullptr>
        Optional& operator=(Optional<U>&& other) noexcept(std::is_nothrow_constructible<ValueType, U>::value and std::is_nothrow_assignable<ValueType, U>::value)
        {
            if (other)
            {
                AssignmentValue(std::move(other.value()));
            }
            else
            {
                reset();
            }
            return *this;
        }


        /**
         * @brief 等価演算子
         * @param lhs
         * @param rhs
         */
        friend bool operator==(const Optional& lhs, const Optional& rhs) noexcept
        {
            if (lhs and rhs)
            {
                return *lhs == *rhs;
            }
            if (not lhs and not rhs)
            {
                return true;    // nullopt == nullopt
            }
            return false;
        }

        /**
         * @brief 非等価演算子
         * @param lhs
         * @param rhs
         */
        friend bool operator!=(const Optional& lhs, const Optional& rhs) noexcept
        {
            return not(lhs == rhs);
        }

        /**
         * @brief 比較演算子
         * @param lhs
         * @param rhs
         */
        friend bool operator<(const Optional& lhs, const Optional& rhs) noexcept
        {
            if (lhs and rhs)
            {
                return *lhs < *rhs;
            }
            if (not rhs)
            {
                return false;
            }
            return not lhs;
        }

        /**
         * @brief 比較演算子
         * @param lhs
         * @param rhs
         */
        friend bool operator>(const Optional& lhs, const Optional& rhs) noexcept
        {
            return rhs < lhs;
        }

        /**
         * @brief 比較演算子
         * @param lhs
         * @param rhs
         */
        friend bool operator>=(const Optional& lhs, const Optional& rhs) noexcept
        {
            return not(lhs < rhs);
        }

        /**
         * @brief 比較演算子
         * @param lhs
         * @param rhs
         */
        friend bool operator<=(const Optional& lhs, const Optional& rhs) noexcept
        {
            return not(rhs < lhs);
        }


        /**
         * @brief 値が有効か
         * @return 有効ならtrue
         */
        explicit constexpr operator bool() const noexcept
        {
            return mHasValue;
        }


        /**
         * @brief 値が有効か
         * @return 有効ならtrue
         */
        constexpr bool hasValue() const noexcept
        {
            return mHasValue;
        }


        /**
         * @brief 値を取得
         * @note 値が無効な場合は例外を投げる
         * @return 値
         */
        ValueType& value() &
        {
            ThrowBadOptionalAccess(not mHasValue);
            return mStorage.value;
        }

        /**
         * @brief 値を取得
         * @note 値が無効な場合は例外を投げる
         * @return 値
         */
        const ValueType& value() const&
        {
            ThrowBadOptionalAccess(not mHasValue);
            return mStorage.value;
        }

        /**
         * @brief 値を取得
         * @note 値が無効な場合は例外を投げる
         * @return 値
         */
        ValueType&& value() &&
        {
            ThrowBadOptionalAccess(not mHasValue);
            return std::move(mStorage.value);
        }

        /**
         * @brief 値を取得
         * @note 値が無効な場合は例外を投げる
         * @return 値
         */
        const ValueType&& value() const&&
        {
            ThrowBadOptionalAccess(not mHasValue);
            return std::move(mStorage.value);
        }

        /**
         * @brief 値を取得
         * @note 値が無効な場合は例外を投げる
         * @return 値
         */
        ValueType& operator*() &
        {
            return value();
        }

        /**
         * @brief 値を取得
         * @note 値が無効な場合は例外を投げる
         * @return 値
         */
        const ValueType& operator*() const&
        {
            return value();
        }

        /**
         * @brief 値を取得
         * @note 値が無効な場合は例外を投げる
         * @return 値
         */
        ValueType&& operator*() &&
        {
            return std::move(*this).value();
        }

        /**
         * @brief 値を取得
         * @note 値が無効な場合は例外を投げる
         * @return 値
         */
        const ValueType&& operator*() const&&
        {
            return std::move(*this).value();
        }

        ValueType* operator->()
        {
            return &value();
        }

        const ValueType* operator->() const
        {
            return &value();
        }

        /**
         * @brief 有効であれば自身を、無効であればデフォルト値を取得する
         * @param defaultValue デフォルト値
         * @return
         */
        template <typename U>
        constexpr ValueType valueOr(U&& defaultValue) const&
        {
            return mHasValue ? **this : static_cast<ValueType>(std::forward<U>(defaultValue));
        }

        /**
         * @brief 有効であれば自身を、無効であればデフォルト値を取得する
         * @param defaultValue デフォルト値
         * @return
         */
        template <typename U>
        constexpr ValueType valueOr(U&& defaultValue) &&
        {
            return mHasValue ? std::move(**this) : static_cast<ValueType>(std::forward<U>(defaultValue));
        }


        /**
         * @brief 値を変換
         */
        template <typename Visitor>
        constexpr auto transform(Visitor&& visitor) & -> Optional<Traits::RemoveCVRefT<typename std::result_of<Visitor(ValueType&)>::type>>
        {
            return *this ? Optional<Traits::RemoveCVRefT<typename std::result_of<Visitor(ValueType&)>::type>>{ visitor(**this) }
                         : nullopt;
        }


        /**
         * @brief 値を変換
         */
        template <typename Visitor>
        constexpr auto transform(Visitor&& visitor) const& -> Optional<Traits::RemoveCVRefT<typename std::result_of<Visitor(const ValueType&)>::type>>
        {
            return *this ? Optional<Traits::RemoveCVRefT<typename std::result_of<Visitor(const ValueType&)>::type>>{ visitor(**this) }
                         : nullopt;
        }


        /**
         * @brief 値を変換
         */
        template <typename Visitor>
        constexpr auto transform(Visitor&& visitor) && -> Optional<Traits::RemoveCVRefT<typename std::result_of<Visitor(ValueType)>::type>>
        {
            return *this ? Optional<Traits::RemoveCVRefT<typename std::result_of<Visitor(ValueType)>::type>>{ visitor(std::move(**this)) }
                         : nullopt;
        }


        /**
         * @brief 他のOptionalと値を交換する
         */
        void swap(Optional& other) noexcept(/* std::is_nothrow_swappable<ValueType>::value and*/ std::is_nothrow_move_assignable<ValueType>::value)
        {
            if (*this and other)
            {
                std::swap(**this, *other);
            }
            else if (not *this and other)
            {
                mStorage.value = std::move(*other);
                other.reset();
            }
            else if (*this and not other)
            {
                *other = std::move(**this);
                reset();
            }
        }


        /**
         * @brief 無効状態にする (トリビアルな型)
         * @note Tが非トリビアルな型で、値が有効な場合はデストラクタを呼ぶ
         */
        template <typename U = ValueType, typename std::enable_if<std::is_trivially_destructible<U>::value, std::nullptr_t>::type = nullptr>
        void reset() noexcept(std::is_nothrow_destructible<ValueType>::value)
        {
            if (mHasValue)
            {
                mHasValue = false;
            }
        }


        /**
         * @brief 無効状態にする (非トリビアルな型)
         * @note Tが非トリビアルな型で、値が有効な場合はデストラクタを呼ぶ
         */
        template <typename U = ValueType, typename std::enable_if<not std::is_trivially_destructible<U>::value, std::nullptr_t>::type = nullptr>
        void reset() noexcept(std::is_nothrow_destructible<ValueType>::value)
        {
            if (mHasValue)
            {
                mStorage.value.~ValueType();
                mHasValue = false;
            }
        }


        /**
         * @brief 値を表示
         */
        void show() const noexcept
        {
            if (mHasValue)
            {
                Udon::ShowRaw(mStorage.value);
            }
            else
            {
                Udon::ShowRaw("nullopt");
            }
        }

#if UDON_PLATFORM_OUTPUT_STREAM == UDON_PLATFORM_OUTPUT_CONSOLE

        /**
         * @brief std::basic_ostream への出力
         * @tparam CharType
         * @param os
         * @param opt
         * @return
         */
        template <typename CharType>
        friend std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, const Optional& opt)
        {
            if (opt)
            {
                return os << *opt;
            }
            else
            {
                return os << "nullopt";
            }
        }

#endif

    private:
        // 共用体を用いて初期化タイミングを遅延
        union Storage
        {
            struct Dummy
            {
                constexpr Dummy() noexcept {}
            } dummy;

            ValueType value;

            constexpr Storage() noexcept
                : dummy{}
            {
            }

            constexpr Storage(Storage&& other) noexcept(std::is_nothrow_move_constructible<ValueType>::value)
                : value{ std::move(other.value) }
            {
            }

            constexpr Storage(const Storage& other) noexcept(std::is_nothrow_move_constructible<ValueType>::value)
                : value{ other.value }
            {
            }

            ~Storage() noexcept {}

            Storage& operator=(const Storage& other)
            {
                value = other.value;
                return *this;
            }

            Storage& operator=(Storage&& other) noexcept(std::is_nothrow_assignable<ValueType, ValueType>::value)
            {
                value = std::move(other.value);
                return *this;
            }

        } mStorage{};

        bool mHasValue;

        template <typename U>
        void ConstructValue(U&& value) noexcept(std::is_nothrow_constructible<ValueType, U>::value)
        {
            new (&mStorage.value) ValueType(std::forward<U>(value));
        }

        template <typename U>
        void AssignmentValue(U&& value) noexcept(std::is_nothrow_constructible<ValueType, U>::value and std::is_nothrow_assignable<ValueType, U>::value)
        {
            if (mHasValue)
            {
                mStorage.value = std::forward<U>(value);
            }
            else
            {
                ConstructValue(std::forward<U>(value));
                mHasValue = true;
            }
        }
    };

}    // namespace Udon