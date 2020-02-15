#ifndef DLGT_DELEGATE_H_
#define DLGT_DELEGATE_H_

#if (__cplusplus > 199711L)
#  define DELEGATES_CPP11_SUPPORT
#endif
namespace PDP
{
#if defined (DELEGATES_CPP11_SUPPORT) && !defined(DELEGATES_TEST_PRE_CPP11)
// we have C++11 support...yeah!
/**
 * non specialized template declaration for delegate
 */
    template <typename T>
    class FastDelegate{};
    
    template <typename T>
    class FastDelegateCore;
    
    
    

/**
 * specialization for member functions
 *
 * \tparam T            class-type of the object who's member function to call
 * \tparam R            return type of the function that gets captured
 * \tparam params       variadic template list for possible arguments
 *                      of the captured function
 */
template <typename T, typename R, typename... Params>
class FastDelegate<R (T::*)(Params...)>
{
public:
    typedef R (T::*func_type)(Params...);

    FastDelegate(func_type func, T& callee)
        : callee_(callee)
        , func_(func)
    {}

    R operator()(Params... args) const
    {
        return (callee_.*func_)(args...);
    }

    bool operator==(const FastDelegate& other) const
    {
        return (&callee_ == &other.callee_) && (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !((*this) == other);
    }
    
    
    
    T * GetCallee() const
    {
        return &this->callee_;
    }
    
    func_type GetFunction() const
    {
        return this->func_;
    }
    
    
    //explicit operator FastDelegateCore<R(Params...)> const { return nullptr; }

private:
    T& callee_;
    func_type func_;
};

/**
 * specialization for const member functions
 */
template <typename T, typename R, typename... Params>
class FastDelegate<R (T::*)(Params...) const>
{
public:
    typedef R (T::*func_type)(Params...) const;

    typedef T ObjectType;
    
    
    FastDelegate(func_type func, const T& callee)
        : callee_(callee)
        , func_(func)
    {}

    R operator()(Params... args) const
    {
        return (callee_.*func_)(args...);
    }

    bool operator==(const FastDelegate& other) const
    {
        return (&callee_ == &other.callee_) && (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !(*this == other);
    }

    
private:
    const T& callee_;
    func_type func_;
};

/**
 * specialization for free functions
 *
 * \tparam R            return type of the function that gets captured
 * \tparam params       variadic template list for possible arguments
 *                      of the captured function
 */
template <typename R, typename... Params>
class FastDelegate<R (*)(Params...)>
{
public:
    typedef R (*func_type)(Params...);

    FastDelegate(func_type func)
        : func_(func)
    {}

    R operator()(Params... args) const
    {
        return (*func_)(args...);
    }

    bool operator==(const FastDelegate& other) const
    {
        return func_ == other.func_;
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !((*this) == other);
    }
    

private:
    func_type func_;
};

/**
 * function to deduce template parameters from call-context
 */
template <typename F, typename T>
FastDelegate<F> make_delegate(F func, T& obj)
{
    return FastDelegate<F>(func, obj);
}

template <typename T>
FastDelegate<T> make_delegate(T && func)
{
    return FastDelegate<T>(LD::Forward<T>(func));
}
    
    typedef void(*FunctionEvilCast);
    struct FastDelegateCoreHelper
    {
        
        void * Object;
        void * Function;
        
    };
    template<typename T, typename ... Args>
    class FastDelegateCore<T(Args...)>
    {
    private:
        typedef T(*CallableFunction)(const FastDelegateCoreHelper &, Args...);
        CallableFunction CurrentCallableFunction;
        FastDelegateCoreHelper CurrentHelper;
    public:
        FastDelegateCore():CurrentCallableFunction(nullptr){}
        FastDelegateCore(const FastDelegateCoreHelper & currentHelper,const CallableFunction & function):CurrentCallableFunction(function),CurrentHelper(currentHelper){}
        
        //template <typename T, typename R, typename... Params>
        //class FastDelegate<R (T::*)(Params...)>
        
        template<typename Q>
        FastDelegateCore(const FastDelegate<T (Q::*)(Args...)> & object)
        {
            (*this) = object;
        }
        
        template<typename Q>
        FastDelegateCore<T(Args...)> & operator = (const FastDelegate<T (Q::*)(Args...)> & callableDelegate)
        {
            Q * currentObjectpqz = callableDelegate.GetCallee();
            
            typename FastDelegate<T (Q::*)(Args...)>::func_type currentFunction = callableDelegate.GetFunction();
            
            //typename FastDelegate<T (Q::*)(Args...)>::func_type * funcPointer = &currentFunction;
            
            CurrentHelper.Function =  (void *&)currentFunction;
            CurrentHelper.Object = currentObjectpqz;
            
            
            
            
            return (*this);
        }
        
        T operator()(Args... args) const
        {
            return CurrentCallableFunction(CurrentHelper,args...);
        }
        
        
    };

// a little backward compatilbility layer
#define DELEGATE make_delegate
#define DELEGATE_CONST make_delegate
#define DELEGATE_FREE make_delegate

#else // hmmm...no C++11 support...yuk!
template <typename T>
class FastDelegate;

template <typename R>
class FastDelegate<R (*)()>
{
public:
    typedef R (*func_type)();

    FastDelegate(func_type func)
        : func_(func)
    {}

    R operator()() const
    {
        return (*func_)();
    }

    bool operator==(const FastDelegate& other) const
    {
        return (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !(*this == other);
    }

private:
    func_type func_;
};
template <typename R, typename P>
class FastDelegate<R (*)(P)>
{
public:
    typedef R (*func_type)(P);

    FastDelegate(func_type func)
        : func_(func)
    {}

    R operator()(P x) const
    {
        return (*func_)(x);
    }

    bool operator==(const FastDelegate& other) const
    {
        return (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !((*this) == other);
    }

private:
    func_type func_;
};
/**
 * specialization for member functions with 0 parmeter
 *
 * \tparam T            class-type of the object who's member function to call
 * \tparam R            return type of the function that gets captured
 */
template <typename T, typename R>
class FastDelegate<R (T::*)()>
{
public:
    typedef R (T::*func_type)();

    FastDelegate(func_type func, T& callee)
        : callee_(callee)
        , func_(func)
    {}

    R operator()() const
    {
        return (callee_.*func_)();
    }

    bool operator==(const FastDelegate& other) const
    {
        return (&callee_ == &other.callee_) && (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !((*this) == other);
    }

private:
    T& callee_;
    func_type func_;
};
template <typename T, typename R>
class FastDelegate<R (T::*)() const>
{
public:
    typedef R (T::*func_type)() const;

    FastDelegate(func_type func, T& callee)
        : callee_(callee)
        , func_(func)
    {}

    R operator()() const
    {
        return (callee_.*func_)();
    }

    bool operator==(const FastDelegate& other) const
    {
        return (&callee_ == &other.callee_) && (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !((*this) == other);
    }

private:
    T& callee_;
    func_type func_;
};
/**
 * specialization for member functions with 1 parmeter
 *
 * \tparam T            class-type of the object who's member function to call
 * \tparam R            return type of the function that gets captured
 * \tparam P            type of first parameter of the captured function
 */
template <typename T, typename R, typename P>
class FastDelegate<R (T::*)(P)>
{
public:
    typedef R (T::*func_type)(P);

    FastDelegate(func_type func, T& callee)
        : callee_(callee)
        , func_(func)
    {}

    R operator()(P arg) const
    {
        return (callee_.*func_)(arg);
    }

    bool operator==(const FastDelegate& other) const
    {
        return (&callee_ == &other.callee_) && (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !((*this) == other);
    }

private:
    T& callee_;
    func_type func_;
};
template <typename T, typename R, typename P>
class FastDelegate<R (T::*)(P) const>
{
public:
    typedef R (T::*func_type)(P) const;

    FastDelegate(func_type func, T& callee)
        : callee_(callee)
        , func_(func)
    {}

    R operator()(P arg) const
    {
        return (callee_.*func_)(arg);
    }

    bool operator==(const FastDelegate& other) const
    {
        return (&callee_ == &other.callee_) && (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !((*this) == other);
    }

private:
    T& callee_;
    func_type func_;
};
template <typename T, typename R, typename P1, typename P2>
class FastDelegate<R (T::*)(P1, P2)>
{
public:
    typedef R (T::*func_type)(P1, P2);

    FastDelegate(func_type func, T& callee)
        : callee_(callee)
        , func_(func)
    {}

    R operator()(P1 arg, P2 arg2) const
    {
        return (callee_.*func_)(arg, arg2);
    }

    bool operator==(const FastDelegate& other) const
    {
        return (&callee_ == &other.callee_) && (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !((*this) == other);
    }

private:
    T& callee_;
    func_type func_;
};
template <typename T, typename R, typename P1, typename P2, typename P3>
class FastDelegate<R (T::*)(P1, P2, P3)>
{
public:
    typedef R (T::*func_type)(P1, P2, P3);

    FastDelegate(func_type func, T& callee)
        : callee_(callee)
        , func_(func)
    {}

    R operator()(P1 arg, P2 arg2, P3 arg3) const
    {
        return (callee_.*func_)(arg, arg2, arg3);
    }

    bool operator==(const FastDelegate& other) const
    {
        return (&callee_ == &other.callee_) && (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !((*this) == other);
    }

private:
    T& callee_;
    func_type func_;
};
template <typename T, typename R, typename P1, typename P2, typename P3, typename P4>
class FastDelegate<R (T::*)(P1, P2, P3, P4)>
{
public:
    typedef R (T::*func_type)(P1, P2, P3, P4);

    FastDelegate(func_type func, T& callee)
        : callee_(callee)
        , func_(func)
    {}

    R operator()(P1 arg, P2 arg2, P3 arg3, P4 arg4) const
    {
        return (callee_.*func_)(arg, arg2, arg3, arg4);
    }

    bool operator==(const FastDelegate& other) const
    {
        return (&callee_ == &other.callee_) && (func_ == other.func_);
    }
    bool operator!= (const FastDelegate& other) const
    {
        return !((*this) == other);
    }

private:
    T& callee_;
    func_type func_;
};
/**
 * function to deduce template parameters from call-context
 */
template <typename F, typename T>
FastDelegate<F> make_delegate(F func, T& obj)
{
    return FastDelegate<F>(func, obj);
}

template <typename T>
FastDelegate<T> make_delegate(T func)
{
    return FastDelegate<T>(func);
}

#endif // DELEGATES_CPP11_SUPPORT

} // namespace delegate

#endif /* end of include guard */
