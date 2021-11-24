//
//  LightWeightDelegate.h
//  DataStructures
//
//  Created by James Landess on 6/7/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_LightWeightDelegate_h
#define DataStructures_LightWeightDelegate_h

#include "LightWeightDelegateBase.h"
#include "Algorithms/Move.hpp"
#include "Algorithms/Forward.hpp"
//#include "Definitions/Common.hpp"
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
        fast_delegate_base() noexcept{ clear(); }
        
        fast_delegate_base(const fast_delegate_base &x) noexcept
        {
            closure_.CopyFrom(this, x.closure_);
        }
        
        void operator = (const fast_delegate_base &x) noexcept
        {
            closure_.CopyFrom(this, x.closure_);
        }
        bool operator ==(const fast_delegate_base &x) const noexcept
        {
            return closure_.IsEqual(x.closure_);
        }
        bool operator !=(const fast_delegate_base &x) const noexcept
        {
            return !closure_.IsEqual(x.closure_);
        }
        bool operator <(const fast_delegate_base &x) const noexcept
        {
            return closure_.IsLess(x.closure_);
        }
        bool operator >(const fast_delegate_base &x) const noexcept
        {
            return x.closure_.IsLess(closure_);
        }
        
        // Binding to non-const member functions
        template<class X, class Y>
        fast_delegate_base(Y *pthis, desired_ret_t (X::* function_to_bind)(P...) ) noexcept
        {
            closure_.bindmemfunc(fastdelegate::detail::implicit_cast<X*>(pthis), function_to_bind);
        }
        
        template<class X, class Y>
        inline void bind(Y *pthis, desired_ret_t (X::* function_to_bind)(P...)) noexcept
        {
            closure_.bindmemfunc(fastdelegate::detail::implicit_cast<X*>(pthis), function_to_bind);
        }
        
        // Binding to const member functions.
        template<class X, class Y>
        fast_delegate_base(const Y *pthis, desired_ret_t (X::* function_to_bind)(P...) const) noexcept
        {
            closure_.bindconstmemfunc(fastdelegate::detail::implicit_cast<const X*>(pthis), function_to_bind);
        }
        
        template<class X, class Y>
        inline void bind(const Y *pthis, desired_ret_t (X::* function_to_bind)(P...) const) noexcept
        {
            closure_.bindconstmemfunc(fastdelegate::detail::implicit_cast<const X *>(pthis), function_to_bind);
        }
        
        // Static functions. We convert them into a member function call.
        // This constructor also provides implicit conversion
        fast_delegate_base(desired_ret_t (*function_to_bind)(P...) ) noexcept
        {
            bind(function_to_bind);
        }
        
        // for efficiency, prevent creation of a temporary
        void operator = (desired_ret_t (*function_to_bind)(P...) ) noexcept
        {
            bind(function_to_bind);
        }
        
        inline void bind(desired_ret_t (*function_to_bind)(P...)) noexcept
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
        operator unspecified_bool_type() const noexcept { return empty()? 0: &safe_bool_struct::m_nonzero; }
        // necessary to allow ==0 to work despite the safe_bool idiom
        inline bool operator==(static_func_ptr funcptr) const noexcept { return closure_.IsEqualToStaticFuncPtr(funcptr); }
        inline bool operator!=(static_func_ptr funcptr) const noexcept { return !closure_.IsEqualToStaticFuncPtr(funcptr); }
        // Is it bound to anything?
        inline bool operator ! () const noexcept { return !closure_; }
        inline bool empty() const noexcept { return !closure_; }
        void clear() noexcept { closure_.clear();}
        // Conversion to and from the DelegateMemento storage class
        const fastdelegate::DelegateMemento & GetMemento() const noexcept { return closure_; }
        void SetMemento(const fastdelegate::DelegateMemento &any) noexcept { closure_.CopyFrom(this, any); }
        
    private:
        // Invoker for static functions
        R invoke_static_func(P... args) const noexcept
        {
            // TODO: extra value copy takes place here!
            return (*(closure_.GetStaticFunction()))(args...);
        }
    };
    
    // fast_delegate<> is similar to std::function, but it has comparison operators.
    template<typename _Signature, bool ExceptStatus = false>
    class LightWeightDelegate;
    
    template<typename R, bool ExceptSTatus,typename ...P>
    class LightWeightDelegate<R(P...),ExceptSTatus> : public fast_delegate_base<R, P...>
    {
    public:
        typedef fast_delegate_base<R, P...> BaseType;
        //typedef  PDP::Tuple<P...> ParamaterPack;
        LightWeightDelegate() noexcept: BaseType() { }
        
        template<class X, class Y>
        LightWeightDelegate(Y * pthis, R (X::* function_to_bind)(P...))
        noexcept: BaseType(pthis, function_to_bind)
        { }

        template<class X, class Y>
        LightWeightDelegate(Y & pthis, R (X::* function_to_bind)(P...))
        noexcept: BaseType(&pthis, function_to_bind)
        { }
        
        template<class X, class Y>
        LightWeightDelegate(const Y *pthis, R (X::* function_to_bind)(P...) const)
        noexcept: BaseType(pthis, function_to_bind)
        { }


        
        LightWeightDelegate(R (*function_to_bind)(P...))
        noexcept: BaseType(function_to_bind)
        { }
        
        void operator = (const BaseType &x)
        {
            *static_cast<BaseType*>(this) = x;
        }
        
        
        
        template<class X, class Y>
        static LightWeightDelegate<R(P...)> Bind(const X *pthis, R (Y::* function_to_bind)(P...) const) noexcept
        {
            return LightWeightDelegate<R(P...)>(pthis,function_to_bind);
        }

        template<class X, class Y>
        static LightWeightDelegate<R(P...)> Bind(const X *pthis, R (Y::* function_to_bind)(P...) const noexcept) noexcept
        {
            return LightWeightDelegate<R(P...),true>(pthis,function_to_bind);
        }

        template<class X, class Y>
        static LightWeightDelegate<R(P...)> Bind(const X  & pthis, R (Y::* function_to_bind)(P...) const) noexcept
        {
            return LightWeightDelegate<R(P...)>(&pthis,function_to_bind);
        }

        template<class X, class Y>
        static LightWeightDelegate<R(P...)> Bind(const X  & pthis, R (Y::* function_to_bind)(P...) const noexcept) noexcept
        {
            return LightWeightDelegate<R(P...), true>(&pthis,function_to_bind);
        }
        
        template<class X, class Y>
        static LightWeightDelegate<R(P...)> Bind(X *pthis, R (Y::* function_to_bind)(P...)) noexcept
        {
            return LightWeightDelegate<R(P...)>(pthis,function_to_bind);
        }

        template<class X, class Y>
        static LightWeightDelegate<R(P...)> Bind(X *pthis, R (Y::* function_to_bind)(P...) noexcept) noexcept
        {
            return LightWeightDelegate<R(P...),true>(pthis,function_to_bind);
        }

        template<class X, class Y>
        static LightWeightDelegate<R(P...)> Bind(X & pthis, R (Y::* function_to_bind)(P...)) noexcept
        {
            return LightWeightDelegate<R(P...)>(&pthis,function_to_bind);
        }

        template<class X, class Y>
        static LightWeightDelegate<R(P...)> Bind(X & pthis, R (Y::* function_to_bind)(P...) noexcept) noexcept
        {
            return LightWeightDelegate<R(P...),true>(&pthis,function_to_bind);
        }
        
        
        
        static LightWeightDelegate<R(P...)> Bind(R (*function_to_bind)(P...)) noexcept
        {
            return LightWeightDelegate<R(P...)>(function_to_bind);
        }

        static LightWeightDelegate<R(P...)> Bind(R (*function_to_bind)(P...) noexcept) noexcept
        {
            return LightWeightDelegate<R(P...),true>(function_to_bind);
        }
        
        //template<class X, class Y>
        //static LightWeightDelegate<R(P...)> Bind(const Y *pthis, R (X::* function_to_bind)(P...) const)
        //{
            //return LightWeightDelegate<R(P...)>(pthis,function_to_bind);
        //}
        
        
       // template<class X, class Y>
        //static LightWeightDelegate<R(P...)> Bind(Y *pthis, R (X::* function_to_bind)(P...))
        //{
          //  return LightWeightDelegate<R(P...)>(pthis,function_to_bind);
        //}
        
        
        
        //static LightWeightDelegate<R(P...)> bind(R (*function_to_bind)(P...))
        //{
            //return LightWeightDelegate<R(P...)>(function_to_bind);
        //}
    };
}



#endif
