//
// Created by phoenixflower on 10/4/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_FUNCTIONVIEW_H
#define LANDESSDEVDATASTRUCTURES_FUNCTIONVIEW_H

//#include "Definitions/Common.hpp"
/*
namespace LD
{
    template <typename TSignature>
    class FunctionView;

    template <typename TReturn, typename... TArgs>
    class FunctionView<TReturn(TArgs...)> final
    {
    private:
        using signature_type = TReturn(void*, TArgs...);

        void* _ptr;
        void * memberfunction;
        TReturn (*_erased_fn)(void*, void* ,TArgs...);

    public:


        FunctionView(){}
        template <typename T, typename = LD::Enable_If_T <
                LD::ConvertiblyCallable<T,TReturn(TArgs...)>::Value()&&
                !LD::Detail::IsSame<LD::Decay_T <T>, FunctionView>{}>>
        FunctionView(T&& x) noexcept : _ptr{(void*)std::addressof(x)}
        {
            _erased_fn = [](void* ptr, void * memberFunction ,TArgs... xs) -> TReturn{

                return (*reinterpret_cast<std::add_pointer_t<T>>(ptr))(LD::Forward<TArgs>(xs)...);
            };
        }



        template<typename Y, typename X>
        FunctionView(const Y & pthis, TReturn (X::* function_to_bind)(TArgs...) const) noexcept : _ptr((void*)&pthis)
        {


            _erased_fn = [](void* ptr, void * memberFunction ,TArgs... xs) -> TReturn{

                typedef TReturn (X::* Function)(TArgs...) const;

                const Y * instance = (const Y*)ptr;

                const Y & deferencedInstance = *instance;

                const Function &  function = (const Function&)memberFunction;

                return (deferencedInstance.*(function))(LD::Forward(xs)...);
            };


        };






        template<typename Y, typename X>
        FunctionView(const Y * pthis, TReturn (X::* function_to_bind)(TArgs...) const) noexcept : _ptr((void*)pthis),memberfunction(reinterpret_cast<void *&>(function_to_bind))
        {

            _erased_fn = [](void* ptr, void * memberFunction ,TArgs... xs) -> TReturn{

                typedef TReturn (X::* Function)(TArgs...) const;
                //return (obj.*getNumber)();

                Y * instance = (Y*)ptr;

                Y & deferencedInstance = *instance;

                Function &  function = (Function&)memberFunction;

                return (deferencedInstance.*(function))(LD::Forward(xs)...);
            };
        }


        template<typename Y, typename X>
        FunctionView(Y & pthis, TReturn (X::* function_to_bind)(TArgs...)) noexcept  :_ptr(&pthis),memberfunction(reinterpret_cast<void *&>(function_to_bind))
        {


            _erased_fn = [](void* ptr, void * memberFunction ,TArgs... xs) -> TReturn{

                typedef TReturn (X::* Function)(TArgs...) const;
                //return (obj.*getNumber)();

                Y * instance = (Y*)ptr;

                Y & deferencedInstance = *instance;

                Function &  function = (Function&)memberFunction;

                return (deferencedInstance.*(function))(LD::Forward(xs)...);
            };

        }

        template<typename Y, typename X>
        FunctionView(Y * pthis, TReturn (X::* function_to_bind)(TArgs...)) noexcept  :_ptr(pthis),memberfunction(reinterpret_cast<void *&>(function_to_bind))
        {


            _erased_fn = [](void* ptr, void * memberFunction ,TArgs... xs) -> TReturn{

                typedef TReturn (X::* Function)(TArgs...) const;

                Y * instance = (Y*)ptr;

                Y & deferencedInstance = *instance;

                Function  & function = (Function&)memberFunction;

                return (deferencedInstance.*(function))(LD::Forward(xs)...);
            };
        }

        decltype(auto) operator()(TArgs... xs) const noexcept(noexcept(_erased_fn(_ptr,memberfunction ,LD::Forward<TArgs>(xs)...)))
        {
            return _erased_fn(_ptr, memberfunction ,std::forward<TArgs>(xs)...);
        }



    };
}
*/
#include "LightWeightDelegateBase.h"
//#include "Definitions/Common.hpp"
#include "Primitives/General/Tuple.hpp"
namespace LD
{
    template<class R=fastdelegate::detail::DefaultVoid, class ...P>
    class fast_delegate_base {
    private:
        typedef typename fastdelegate::detail::DefaultVoidToVoid<R>::type desired_ret_t;
        typedef desired_ret_t (*static_func_ptr)(P...);
        typedef R (*unvoid_static_func_ptr)(P...);
        typedef R (fastdelegate::detail::GenericClass::*generic_mem_fn)(P...);
        typedef fastdelegate::detail::ClosurePtr<generic_mem_fn, static_func_ptr, unvoid_static_func_ptr> closure_t;
        closure_t closure_;
    public:
        // Typedefs to aid generic programming
        typedef fast_delegate_base type;

        // Construction and comparison functions
        fast_delegate_base() { clear(); }

        fast_delegate_base(const fast_delegate_base &x)
        {
            closure_.CopyFrom(this, x.closure_);
        }

        void operator = (const fast_delegate_base &x)
        {
            closure_.CopyFrom(this, x.closure_);
        }
        bool operator ==(const fast_delegate_base &x) const
        {
            return closure_.IsEqual(x.closure_);
        }
        bool operator !=(const fast_delegate_base &x) const
        {
            return !closure_.IsEqual(x.closure_);
        }
        bool operator <(const fast_delegate_base &x) const
        {
            return closure_.IsLess(x.closure_);
        }
        bool operator >(const fast_delegate_base &x) const
        {
            return x.closure_.IsLess(closure_);
        }

        // Binding to non-const member functions
        template<class X, class Y>
        fast_delegate_base(Y *pthis, desired_ret_t (X::* function_to_bind)(P...) )
        {
            closure_.bindmemfunc(fastdelegate::detail::implicit_cast<X*>(pthis), function_to_bind);
        }

        template<class X, class Y>
        inline void bind(Y *pthis, desired_ret_t (X::* function_to_bind)(P...))
        {
            closure_.bindmemfunc(fastdelegate::detail::implicit_cast<X*>(pthis), function_to_bind);
        }

        // Binding to const member functions.
        template<class X, class Y>
        fast_delegate_base(const Y *pthis, desired_ret_t (X::* function_to_bind)(P...) const)
        {
            closure_.bindconstmemfunc(fastdelegate::detail::implicit_cast<const X*>(pthis), function_to_bind);
        }

        template<class X, class Y>
        inline void bind(const Y *pthis, desired_ret_t (X::* function_to_bind)(P...) const)
        {
            closure_.bindconstmemfunc(fastdelegate::detail::implicit_cast<const X *>(pthis), function_to_bind);
        }

        // Static functions. We convert them into a member function call.
        // This constructor also provides implicit conversion
        fast_delegate_base(desired_ret_t (*function_to_bind)(P...) )
        {
            bind(function_to_bind);
        }

        // for efficiency, prevent creation of a temporary
        void operator = (desired_ret_t (*function_to_bind)(P...) )
        {
            bind(function_to_bind);
        }

        inline void bind(desired_ret_t (*function_to_bind)(P...))
        {
            closure_.bindstaticfunc(this, &fast_delegate_base::invoke_static_func, function_to_bind);
        }

        // Invoke the delegate
        template<typename ...A>
        R operator()(A&&... args) const
        {
            return (closure_.GetClosureThis()->*(closure_.GetClosureMemPtr()))(LD::Forward<A>(args)...);
        }

    private:
        typedef struct safe_bool_struct
        {
            int a_data_pointer_to_this_is_0_on_buggy_compilers;
            static_func_ptr m_nonzero;
        } useless_typedef;
        typedef static_func_ptr safe_bool_struct::*unspecified_bool_type;
    public:
        //operator unspecified_bool_type() const { return empty()? 0: &safe_bool_struct::m_nonzero; }
        // necessary to allow ==0 to work despite the safe_bool idiom
        inline bool operator==(static_func_ptr funcptr) { return closure_.IsEqualToStaticFuncPtr(funcptr); }
        inline bool operator!=(static_func_ptr funcptr) { return !closure_.IsEqualToStaticFuncPtr(funcptr); }
        // Is it bound to anything?
        inline bool operator ! () const { return !closure_; }
        inline bool empty() const { return !closure_; }
        void clear() { closure_.clear();}
        // Conversion to and from the DelegateMemento storage class
        const fastdelegate::DelegateMemento & GetMemento() { return closure_; }
        void SetMemento(const fastdelegate::DelegateMemento &any) { closure_.CopyFrom(this, any); }

    private:
        // Invoker for static functions
        R invoke_static_func(P... args) const
        {
            // TODO: extra value copy takes place here!
            return (*(closure_.GetStaticFunction()))(args...);
        }
    };

    // fast_delegate<> is similar to std::function, but it has comparison operators.
    template<typename _Signature>
    class FunctionView;

    template<typename R, typename ...P>
    class FunctionView<R(P...)> : public fast_delegate_base<R, P...>
    {
    public:
        typedef fast_delegate_base<R, P...> BaseType;
        //typedef  PDP::Tuple<P...> ParamaterPack;
        FunctionView() : BaseType() { }

        template<class X, class Y>
        FunctionView(Y * pthis, R (X::* function_to_bind)(P...))
                : BaseType(pthis, function_to_bind)
        { }

        template<class X, class Y>
        FunctionView(const Y *pthis, R (X::* function_to_bind)(P...) const)
                : BaseType(pthis, function_to_bind)
        { }

        FunctionView(R (*function_to_bind)(P...))
                : BaseType(function_to_bind)
        { }

        void operator = (const BaseType &x)
        {
            *static_cast<BaseType*>(this) = x;
        }



        template<class X, class Y>
        static FunctionView<R(P...)> Bind(const X *pthis, R (Y::* function_to_bind)(P...) const)
        {
            return FunctionView<R(P...)>(pthis,function_to_bind);
        }


        template<class X, class Y>
        static FunctionView<R(P...)> Bind(X *pthis, R (Y::* function_to_bind)(P...))
        {
            return FunctionView<R(P...)>(pthis,function_to_bind);
        }



        static FunctionView<R(P...)> Bind(R (*function_to_bind)(P...))
        {
            return FunctionView<R(P...)>(function_to_bind);
        }


        template<class X, class Y>
        static FunctionView<R(P...)> bind(const Y *pthis, R (X::* function_to_bind)(P...) const)
        {
            return FunctionView<R(P...)>(pthis,function_to_bind);
        }


        template<class X, class Y>
        static FunctionView<R(P...)> bind(Y *pthis, R (X::* function_to_bind)(P...))
        {
            return FunctionView<R(P...)>(pthis,function_to_bind);
        }



        static FunctionView<R(P...)> bind(R (*function_to_bind)(P...))
        {
            return FunctionView<R(P...)>(function_to_bind);
        }
    };
}

#endif //LANDESSDEVDATASTRUCTURES_FUNCTIONVIEW_H
