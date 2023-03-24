#pragma once

// #include <algorithm>  // std::forward std::swap

namespace udon
{

    namespace std
    {

        // class bad_function_call : public exception {
        // 	[[nodiscard]]
        // 	virtual const char* what() const noexcept override {
        // 		return "bad function call";
        // 	}
        // };

        template <class>
        class function;

        template <class R, class... Args>
        class function<R(Args...)>
        {

            struct VTable
            {
                R(*call)
                (void*, Args&&...);
                void (*destroy)(void*);
            };

            VTable* vTable;

            void* functor;

            long* counter;    // memory admin counter

        public:
            /// @brief default constructor
            function() noexcept
                : vTable()
                , functor()
                , counter(new long(0))
            {
            }

            /// @brief converting constructor
            /// @param r functor
            template <class Functor>
            function(const Functor& r)
                : vTable()
                , functor(new Functor(r))
                , counter(new long(1))
            {
                // construct virtual function table
                static VTable table = {
                    [](void* functor, Args&&... args) -> R
                    {
                        //                        return (*reinterpret_cast<Functor *>(functor))(std::forward<Args>(args)...);
                        return (*reinterpret_cast<Functor*>(functor))(args...);
                    },
                    [](void* functor)
                    {
                        delete reinterpret_cast<Functor*>(functor);
                    }
                };
                vTable = &table;
            }

            /// @brief copy construtor
            /// @param r functor
            function(const function& r) noexcept
                : vTable(r.vTable)
                , functor(r.functor)
                , counter(r.counter)
            {
                ++(*counter);
            }

            /// @brief move constructor
            /// @param r functor
            function(function&& r) noexcept
                : vTable(r.vTable)
                , functor(r.functor)
                , counter(r.counter)
            {
                r.vTable  = nullptr;
                r.counter = nullptr;
                r.functor = nullptr;
            }

            ~function() noexcept
            {

                if (counter)
                {
                    if (--(*counter) > 0)
                        ;
                    else
                    {
                        delete counter;
                    }
                }
                if (*this)
                {
                    vTable->destroy(functor);
                }
            }

            /// @brief move asingment operaotr
            function& operator=(function&& r) noexcept
            {
                if (this == &r)
                    ;
                else
                {
                    if (counter)
                    {
                        const auto count = *counter + *r.counter;
                        delete counter;
                    }
                    vTable  = r.vTable;
                    functor = r.functor;
                    counter = r.counter;

                    r.vTable  = nullptr;
                    r.functor = nullptr;
                    r.counter = nullptr;
                }
                return *this;
            }

            /// @brief copy asingment operaotr
            function& operator=(const function& r) noexcept
            {
                if (this == &r)
                    ;
                else
                {
                    const auto count = *counter + *r.counter;
                    delete counter;
                    counter  = r.counter;
                    *counter = count;
                    ++(*counter);

                    vTable  = r.vTable;
                    functor = r.functor;
                }
                return *this;
            }

            operator bool() const noexcept
            {
                return functor && vTable;
            }

            // void swap(function &r) {
            //     auto &&temp = std::move(r);
            //     r = std::move(*this);
            //     *this = std::move(temp);
            // }

            /// @brief call functor
            R operator()(Args&&... args)
            {
                if (*this)
                {
                    // return vTable->call(functor, std::forward<Args>(args)...);
                    return vTable->call(functor, args...);
                }
                else
                {
                    // throw bad_function_call{};
                }
            }
        };

        // template <class>
        // void swap() {}

        // template <class R, class... Args>
        // void swap(function<R(Args...)> &lhs, function<R(Args...)> &rhs) {
        //     lhs.swap(rhs);
        // }

    }    // namespace std

}    // namespace udon
