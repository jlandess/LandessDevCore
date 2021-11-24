//
// Copyright (c) 2014-2016 Pavel Medvedev. All rights reserved.
//
// Distributed under the MIT software license, see the accompanying
// file LICENSE

#ifndef FIXED_SIZE_FUNCTION_HPP_INCLUDED
#define FIXED_SIZE_FUNCTION_HPP_INCLUDED


#include "TypeTraits/IsSame.hpp"
#include "Algorithms/Forward.hpp"
#include "Definitions/Integer.hpp"
namespace LD
{
    enum class construct_type
    {
        none,
        copy,
        move,
        copy_and_move,
    };


    struct NoConstructionStrategy
    {
        static const construct_type Strategy = construct_type::none;
    };

    struct CopyConstructionStrategy
    {
        static const construct_type Strategy = construct_type::copy;
    };

    struct MoveConstructionStrategy
    {
        static const construct_type Strategy = construct_type::move;
    };

    struct CopyAndMoveConstructionStrategy
    {
        static const construct_type Strategy = construct_type::copy_and_move;
    };

    template<LD::UInteger Size>
    struct BufferSize
    {
        static const LD::UInteger MaxSize = Size;
    };
    namespace details
    {

        // V-table implementation
        template<typename Ret, typename ...Args>
        struct fixed_function_vtable_base
        {
            Ret  (*call)(void*, Args&& ...) = nullptr;
            void (*destroy)(void*) = nullptr;
        };

        template<construct_type ConstructStrategy, typename Ret, typename ...Args>
        struct fixed_function_vtable;

        template<typename Ret, typename ...Args>
        struct fixed_function_vtable<construct_type::none, Ret, Args...> : fixed_function_vtable_base<Ret, Args...>
        {
        };

        template<typename Ret, typename ...Args>
        struct fixed_function_vtable<construct_type::copy, Ret, Args...> : fixed_function_vtable_base<Ret, Args...>
        {
            void (*copy)(const void*, void*) = nullptr;
        };

        template<typename Ret, typename ...Args>
        struct fixed_function_vtable<construct_type::move, Ret, Args...>: fixed_function_vtable_base<Ret, Args...>
        {
            void (*move)(void*, void*) = nullptr;
        };

        template<typename Ret, typename ...Args>
        struct fixed_function_vtable<construct_type::copy_and_move, Ret, Args...>: fixed_function_vtable_base<Ret, Args...>
        {
            void (*copy)(const void*, void*) = nullptr;
            void (*move)(void*, void*) = nullptr;
        };

    } // namespace details

    template<typename Function, class MaxSize = LD::BufferSize<128>, class ConstructStrategy = CopyAndMoveConstructionStrategy>
    class fixed_size_function;

    template<typename Ret, typename ...Args, class MaxSize, class ConstructStrategy>
    class fixed_size_function<Ret (Args...), MaxSize, ConstructStrategy>
    {
    public:
        // Compile-time information

        constexpr static bool is_copyable = LD::Detail::IntegralConstant<bool,LD::Detail::IsSame<ConstructStrategy,CopyConstructionStrategy>::value ||
                                                                              LD::Detail::IsSame<ConstructStrategy,CopyAndMoveConstructionStrategy>::value>::value;

        constexpr static bool is_movable = LD::Detail::IntegralConstant<bool,LD::Detail::IsSame<ConstructStrategy,MoveConstructionStrategy>::value ||
                                                                             LD::Detail::IsSame<ConstructStrategy,CopyAndMoveConstructionStrategy>::value>::value;


        using is_copyable_type = LD::Detail::IntegralConstant<bool,LD::Detail::IsSame<ConstructStrategy,CopyConstructionStrategy>::value ||
                                                                   LD::Detail::IsSame<ConstructStrategy,CopyAndMoveConstructionStrategy>::value>;

        using is_movable_type = LD::Detail::IntegralConstant<bool,LD::Detail::IsSame<ConstructStrategy,MoveConstructionStrategy>::value ||
                                                                  LD::Detail::IsSame<ConstructStrategy,CopyAndMoveConstructionStrategy>::value>;

        using result_type = Ret;

        static const std::size_t arity = sizeof...(Args);

        template <std::size_t N>
        struct argument
        {
            static_assert(N < arity, "invalid argument index");
            using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
        };

    public:
        template<typename F, class S, class C> fixed_size_function(fixed_size_function<F, S, C> const&) = delete;
        template<typename F, class S, class C> fixed_size_function(fixed_size_function<F, S, C>&) = delete;
        template<typename F, class S, class C> fixed_size_function(fixed_size_function<F, S, C>&&) = delete;
        template<typename F, class S, class C> fixed_size_function& operator=(fixed_size_function<F, S, C> const&) = delete;
        template<typename F, class S, class C> fixed_size_function& operator=(fixed_size_function<F, S, C>&) = delete;
        template<typename F, class S, class C> fixed_size_function& operator=(fixed_size_function<F, S, C>&&) = delete;
        template<typename F, class S, class C> void assign(fixed_size_function<F, S, C> const&) = delete;
        template<typename F, class S, class C> void assign(fixed_size_function<F, S, C>&) = delete;
        template<typename F, class S, class C> void assign(fixed_size_function<F, S, C>&&) = delete;

        fixed_size_function() {}

        ~fixed_size_function()
        {
            reset();
        }

        fixed_size_function(std::nullptr_t) {}

        fixed_size_function& operator=(std::nullptr_t)
        {
            reset();
            return *this;
        }

        fixed_size_function(fixed_size_function const& src)
        {
            copy(src);
        }

        fixed_size_function& operator=(fixed_size_function const& src)
        {
            assign(src);
            return *this;
        }

        fixed_size_function(fixed_size_function& src)
        {
            copy(src);
        }

        fixed_size_function& operator=(fixed_size_function& src)
        {
            assign(src);
            return *this;
        }

        fixed_size_function(fixed_size_function&& src)
        {
            move(std::move(src), is_movable_type());
        }

        fixed_size_function& operator=(fixed_size_function&& src)
        {
            assign(std::move(src));
            return *this;
        }

        template<typename Functor>
        fixed_size_function(Functor&& f)
        {
            create(std::forward<Functor>(f));
        }

        template<typename Functor>
        fixed_size_function& operator=(Functor&& f)
        {
            assign(std::forward<Functor>(f));
            return *this;
        }

        void assign(fixed_size_function const& src)
        {
            reset();
            copy(src);
        }

        void assign(fixed_size_function& src)
        {
            reset();
            copy(src);
        }

        void assign(fixed_size_function&& src)
        {
            reset();
            move(std::move(src), is_movable_type());
        }

        template<typename Functor>
        void assign(Functor&& f)
        {
            reset();
            create(std::forward<Functor>(f));
        }

        void reset()
        {
            auto destroy = vtable_.destroy;
            if (destroy)
            {
                vtable_ = vtable();
                destroy(&storage_);
            }
        }

        explicit operator bool() const { return vtable_.call != nullptr; }

        Ret operator()(Args... args)
        {
            //return vtable_.call ? vtable_.call(&storage_, std::forward<Args>(args)...) : throw std::bad_function_call();
            return vtable_.call(&storage_, std::forward<Args>(args)...);
        }

        void swap(fixed_size_function& other)
        {
            fixed_size_function tmp = std::move(other);
            other = std::move(*this);
            *this = std::move(tmp);
        }

        friend void swap(fixed_size_function& lhs, fixed_size_function& rhs)
        {
            lhs.swap(rhs);
        }

        friend bool operator==(std::nullptr_t, fixed_size_function const& f)
        {
            return !f;
        }

        friend bool operator==(fixed_size_function const& f, std::nullptr_t)
        {
            return !f;
        }

        friend bool operator!=(std::nullptr_t, fixed_size_function const& f)
        {
            return f;
        }

        friend bool operator!=(fixed_size_function const& f, std::nullptr_t)
        {
            return f;
        }

    private:
        template<typename Functor>
        void create(Functor&& f)
        {
            using functor_type = typename std::decay<Functor>::type;
            static_assert(sizeof(functor_type) <= StorageSize, "Functor must be smaller than storage buffer");

            new (&storage_) functor_type(std::forward<Functor>(f));

            vtable_.call = &call_impl<functor_type>;
            vtable_.destroy = &destroy_impl<functor_type>;
            init_copy<functor_type>(is_copyable_type());
            init_move<functor_type>(is_movable_type());
        }

        void copy(fixed_size_function const& src)
        {
            if (src.vtable_.copy)
            {
                src.vtable_.copy(&src.storage_, &storage_);
                vtable_ = src.vtable_;
            }
        }

        void move(fixed_size_function&& src, LD::TrueType movable)
        {
            if (src.vtable_.move)
            {
                src.vtable_.move(&src.storage_, &storage_);
                vtable_ = src.vtable_;
                src.reset();
            }
        }

        void move(fixed_size_function const& src, LD::FalseType movable)
        {
            copy(src);
        }

    private:
        template<typename Functor>
        static Ret call_impl(void* functor, Args&& ... args)
        {
            return (*static_cast<Functor*>(functor))(std::forward<Args>(args)...);
        }

        template<typename Functor>
        static void destroy_impl(void* functor)
        {
            static_cast<Functor*>(functor)->~Functor();
        }

        template<typename Functor>
        static void copy_impl(void const* functor, void* dest)
        {
            new (dest) Functor(*static_cast<Functor const*>(functor));
        }

        template<typename Functor>
        static void move_impl(void* functor, void* dest)
        {
            new (dest) Functor(std::move(*static_cast<Functor*>(functor)));
        }

        template<typename Functor>
        void init_copy(LD::TrueType /*copyable*/) { vtable_.copy = &copy_impl<Functor>; }

        template<typename Functor>
        void init_copy(LD::FalseType /*copyable*/) {}

        template<typename Functor>
        void init_move(LD::TrueType /*movable*/) { vtable_.move = &move_impl<Functor>; }

        template<typename Functor>
        void init_move(LD::FalseType /*movable*/) {}

    private:
        using vtable = details::fixed_function_vtable<ConstructStrategy::Strategy, Ret, Args...>;
        static const size_t StorageSize = MaxSize::MaxSize - sizeof(vtable);
        using storage = typename std::aligned_storage<StorageSize>::type;

        vtable vtable_;
        storage storage_;
    };

    template<typename T>
    class SBOFunction;


    template<typename T, typename ... Args>
    class SBOFunction<T(Args...)>
    {
    private:
        fixed_size_function<T(Args...)> Default;
    public:
        SBOFunction()
        {

        }
        SBOFunction(const SBOFunction & function):Default(function.Default)
        {

        }

        template<typename F>
        SBOFunction(F && functor):Default(LD::Forward<F>(functor))
        {

        }


        SBOFunction & operator = (const SBOFunction & function)
        {
            //this->Default = function.Default;
            return (*this);
        }


        template<typename F>
        SBOFunction & operator = (F && functor)
        {
            this->Default.assign(LD::Forward<F>(functor));
            return (*this);
        }

        T operator()(Args... args)
        {
            return this->Default(args...);
        }
    };
}


#endif // FIXED_SIZE_FUNCTION_HPP_INCLUDED
