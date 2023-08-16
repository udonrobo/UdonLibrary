#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

template <class T>
struct AlwaysFalse : std::false_type
{
};

namespace Udon
{

    namespace Details
    {

        class DummyAccessor;

        namespace AccessibleImpl
        {
            template <typename T>
            static auto test(T&&) -> decltype(std::declval<T>().accessor(std::declval<DummyAccessor&>()), std::true_type());
            template <typename T>
            static auto test(...) -> std::false_type;
        }    // namespace AccessibleImpl

        /// @brief T::accessor<Acc>(Acc&) が存在するか判定する
        template <typename T>
        struct Accessible : decltype(AccessibleImpl::test<T>(std::declval<T>()))
        {
        };

        namespace AccessorCallableImpl
        {
            template <typename T>
            static auto test(T&&) -> decltype(Accessor(std::declval<DummyAccessor&>(), std::declval<std::remove_const_t<T>&>()), std::true_type());
            template <typename T>
            static auto test(...) -> std::false_type;
        }    // namespace AccessorCallableImpl

        /// @brief グローバル関数 Accessor<Acc>(Acc&, T&) が呼び出せるか判定する
        template <typename T>
        struct AccessorCallable : decltype(AccessorCallableImpl::test<T>(std::declval<T>()))
        {
        };

    }    // namespace Details

    namespace Details
    {

        namespace HasMemberFunctionCapacityImpl
        {
            template <typename T>
            static auto test(T&&) -> decltype(std::declval<T>().capacity(), std::true_type());
            template <typename T>
            static auto test(...) -> std::false_type;
        }    // namespace HasMemberFunctionCapacityImpl

        /// @brief T::capacity を持つか判定する
        template <typename T>
        struct HasMemberFunctionCapacity : decltype(HasMemberFunctionCapacityImpl::test<T>(std::declval<T>()))
        {
        };

        namespace CapacityCallableImpl
        {
            template <typename T>
            static auto test(T&&) -> decltype(Capacity(std::declval<T>()), std::true_type());
            template <typename T>
            static auto test(...) -> std::false_type;
        }    // namespace CapacityCallableImpl

        /// @brief グローバル関数 Capacity(T) が呼び出せるか判定する
        template <typename T>
        struct CapacityCallable : decltype(CapacityCallableImpl::test<T>(std::declval<T>()))
        {
        };

    }    // namespace Details

}    // namespace Udon