//
//  PackagedTask.h
//  DataStructures
//
//  Created by James Landess on 6/11/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_PackagedTask_h
#define DataStructures_PackagedTask_h

#include "Memory/ElementReference.h"
//#include "Definitions/Common.hpp"
#include "Functor/FastDelegate.h"
#include "Async/Promise.h"
#include "Async/Commitment.h"
#include "Functor/Delegate.h"
#include "Primitives/General/mapboxoptional.hpp"
#include "TypeTraits/Decay.hpp"
#include "Algorithms/Invoke.hpp"

//#include <tuple>



namespace LD
{
    namespace Detail
    {
        /*
        template<typename F, typename ... Args,LD::UInteger ...Indices >
        auto InvokeFold(LD::IndexSequence<Indices...> sequence, F && function, const LD::Tuple<Args...> & tuple)
        {
            return function(LD::Get<Indices>(tuple)...);
        }
         */
    }

    /*
    //return Delegate(PDP::Get<S>(Arguements) ...);
    template<typename F, typename ... Args>
    auto Invoke(F && function, const LD::Tuple<Args...> & tuple)
    {
        return LD::Detail::InvokeFold(LD::MakeIndexSequence_T<sizeof...(Args)>{}, LD::Forward<F>(function), tuple);
    }
     */


    template<typename Executor,typename T>
    class PackagedTask;

    template<typename Executor, typename T, typename ... Args>
    class PackagedTask<Executor, T(Args...)>
    {
    private:
        Executor CurrentExecutor;
        LD::Tuple<Args...> Arguement;
        mapbox::util::variant<LD::Promise<T>,LD::Commitment<T>> CurrentPromise;
    public:

        inline PackagedTask()
        {

        }
        inline PackagedTask(const Executor & executor,
                            const LD::Tuple<Args...> & arguements,
                            const mapbox::util::variant<LD::Promise<T>,LD::Commitment<T>> & promise):CurrentExecutor(executor),Arguement(arguements),CurrentPromise(promise)
        {

        }

        void operator()()
        {
            mapbox::util::apply_visitor(*this, CurrentPromise);
        }

        void operator()(LD::Promise<T> & promise)
        {
            promise.SetValue(LD::Invoke(CurrentExecutor, Arguement));

        }

        void operator()(LD::Commitment<T> & promise)
        {
            promise.AddItem(LD::Invoke(CurrentExecutor, Arguement));
        }
    };



    template<typename Executor, typename ... Args>
    class PackagedTask<Executor, void(Args...)>
    {
    private:
        Executor CurrentExecutor;
        LD::Tuple<Args...> Arguement;
        mapbox::util::variant<LD::Promise<void>,LD::Commitment<void>> CurrentPromise;
    public:

        inline PackagedTask()
        {

        }
        inline PackagedTask(const Executor & executor,
                            const LD::Tuple<Args...> & arguements,
                            const mapbox::util::variant<LD::Promise<void>,LD::Commitment<void>> & promise):CurrentExecutor(executor),Arguement(arguements),CurrentPromise(promise)
        {

        }

        void operator()()
        {
            mapbox::util::apply_visitor(*this, CurrentPromise);
        }

        void operator()(LD::Promise<void> & promise)
        {

        }

        void operator()(LD::Commitment<void> & promise)
        {

        }
    };
}
namespace PDP
{
    class Task
    {
    private:


    public:

        virtual ~Task(){}

        virtual void Run()  = 0;
    };



    namespace helper
    {
        template <int... Is>
        struct index {};

        template <int N, int... Is>
        struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

        template <int... Is>
        struct gen_seq<0, Is...> : index<Is...> {};
    }



    template<int ...> struct seq {};

    template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

    template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };


    template<typename Ret, typename Object>
    class TypedTask: public Task
    {
    private:
    public:




    };


    template<typename Ret, typename Object, typename ... Args>
    class TypedTask<Ret(Args...),Object>: public Task
    {
    public:
        typedef Ret (Object::*MemberPointer)(Args...);
    private:


        LD::Promise<Ret> CurrentPromise;


        Object CurrentObject;


        MemberPointer CurrentMemberPointer;

        std::tuple<Args...> Arguements;


        template<int ...S, typename ...A>
        Ret callFunc(seq<S...>)
        {
            PDP::LightWeightDelegate<Ret(Args...)> currentDelegate(CurrentObject,CurrentMemberPointer);
            return currentDelegate(std::get<S>(Arguements) ...);
        }
    public:

        TypedTask(const LD::Promise<Ret> & promise, const Object & object, const MemberPointer & memberPointer ,Args  && ...arguements):Arguements(LD::Forward<Args>(arguements)...),CurrentObject(object),CurrentPromise(promise),CurrentMemberPointer(memberPointer)
        {

        }


        virtual void Run()
        {

            Ret returnValue = LD::Move(callFunc(typename gens<sizeof...(Args)>::type()));

            CurrentPromise.SetValue(LD::Move(returnValue));
        }

    };


    template<typename Ret, typename Object>
    class ConstTypedTask
    {

    };

    template<typename Ret, typename Object, typename ... Args>
    class ConstTypedTask<Ret(Args...),Object>: public Task
    {
    public:
        typedef Ret (Object::*MemberPointer)(Args...) const;
    private:
        LD::Promise<Ret> CurrentPromise;
        const Object CurrentObject;
        MemberPointer CurrentMemberPointer;

        std::tuple<Args...> Arguements;


        template<int ...S>
        Ret callFunc(seq<S...>)
        {
            PDP::LightWeightDelegate<Ret(Args...)> currentDelegate(CurrentObject,CurrentMemberPointer);
            return currentDelegate(std::get<S>(Arguements) ...);
        }
    public:


        ConstTypedTask(const LD::Promise<Ret> & promise, const Object & object, const MemberPointer & memberPointer ,Args  && ...arguements):Arguements(LD::Forward<Args>(arguements)...),CurrentObject(object),CurrentPromise(promise),CurrentMemberPointer(memberPointer)
        {

        }


        virtual void Run()
        {

            Ret returnValue = LD::Move(callFunc(typename gens<sizeof...(Args)>::type()));

            CurrentPromise.SetValue(LD::Move(returnValue));
        }
    };


    template<typename T>
    class DelegatedPackageTask
    {

    };


    template<typename T, typename ... Args>
    class DelegatedPackageTask<T(Args...)>: public Task
    {
    private:
        LD::Promise<T> CurrentPromise;
        PDP::Delegate<T(Args...)> Delegate;
        std::tuple<Args...> Arguements;
        template<int ...S>
        T callFunc(seq<S...>)
        {
            return Delegate(std::get<S>(Arguements) ...);
        }

    public:

        DelegatedPackageTask(const LD::Promise<T> & promise,const PDP::Delegate<T(Args...)> & delegate, Args  && ...arguements):Arguements(LD::Forward<Args>(arguements)...),Delegate(delegate),CurrentPromise(promise)
        {

        }


        virtual void Run()
        {
            T returnValue =  LD::Move(callFunc(typename gens<sizeof...(Args)>::type()));
            CurrentPromise.SetValue(LD::Move(returnValue));

        }

    };
    template<typename T>
    class FastDelegatedPackageTask
    {

    };


    template<typename T, typename ... Args>
    class FastDelegatedPackageTask<T(Args...)>: public Task
    {
    private:
        LD::Promise<T> CurrentPromise;
        PDP::FastDelegate<T(Args...)> Delegate;
        std::tuple<Args...> Arguements;
        template<int ...S>
        T callFunc(seq<S...>)
        {
            return Delegate(std::get<S>(Arguements) ...);
        }
    public:

        FastDelegatedPackageTask(const LD::Promise<T> & promise,const PDP::FastDelegate<T(Args...)> & delegate, Args  && ...arguements):Arguements(LD::Forward<Args>(arguements)...),Delegate(delegate),CurrentPromise(promise)
        {

        }


        virtual void Run()
        {
            T returnValue =  LD::Move(callFunc(typename gens<sizeof...(Args)>::type()));
            CurrentPromise.SetValue(LD::Move(returnValue));

        }
    };

    template<typename T>
    class LightWeightCommittedLightWeightPackagedTask
    {
    private:

    };


    template<typename T, typename ... Args>
    class LightWeightCommittedLightWeightPackagedTask<T(Args...)>
    {
    private:
        PDP::LightWeightDelegate<T(Args...)> Delegate;
        LD::Commitment<T> CurrentCommittment;
        std::tuple<Args...> Arguements;


        template<int ...S>
        T callFunc(seq<S...>) const
        {
            return Delegate(std::get<S>(Arguements) ...);
        }
    public:
        LightWeightCommittedLightWeightPackagedTask(const LD::Commitment<T> & promise,const PDP::LightWeightDelegate<T(Args...)> & delegate, Args && ... arguements):Arguements(LD::Forward<Args>(arguements)...),Delegate(delegate),CurrentCommittment(promise)
        {

        }

        virtual void Run()
        {

            T returnValue =  LD::Move(callFunc(typename gens<sizeof...(Args)>::type()));
            CurrentCommittment.AddValue(LD::Move(returnValue));
        }
    };

    template<typename T>
    class LightWeightDelegatedPackageTask
    {

    };

    template<typename T, typename ... Args>
    class LightWeightDelegatedPackageTask<T(Args...)>: public Task
    {
    private:
        LD::Promise<T> CurrentPromise;
        PDP::LightWeightDelegate<T(Args...)> Delegate;
        std::tuple<Args...> Arguements;


        template<int ...S>
        T callFunc(seq<S...>) const
        {
            return Delegate(std::get<S>(Arguements) ...);
        }
    public:
        LightWeightDelegatedPackageTask(const LD::Promise<T> & promise,const PDP::LightWeightDelegate<T(Args...)> & delegate, Args && ... arguements):Arguements(LD::Forward<Args>(arguements)...),Delegate(delegate),CurrentPromise(promise)
        {

        }
        virtual void Run()
        {

            T returnValue =  LD::Move(callFunc(typename gens<sizeof...(Args)>::type()));
            CurrentPromise.SetValue(LD::Move(returnValue));
            //CurrentPromise.SetFinishedState(true);
        }

    };


    template<typename T, template<typename> class Functor = PDP::LightWeightDelegate,class = void>
    class PackagedTask;


    /**
     @tparam T - The expected return type
     @tparam Functor - The type of functor you would like to utilize.  For instance std::function, PDP::Lamba, PDP::LightWeightDelegate are all valid functor class types
     @tparam Args - infered arguements from the exact type of functor passed in
     @brief Serves as the primary implementation of PDP::PackagedTask where you have a non-void return type and do not expected to utilized PDP::Commitment to stream data
     */
    template<typename T, template<typename> class Functor, typename ... Args>
    class PackagedTask<T(Args...),Functor,LD::Enable_If_T<!LD::Detail::IsSame<LD::Detail::Decay_T<T>, void>::value>>: public PDP::Task
    {
    public:
        typedef Functor<T(Args...)> FunctorType;
    private:
        LD::Promise<T> CurrentPromise;
        Functor<T(Args...)> Delegate;
        LD::Tuple<LD::Detail::Decay_T<Args>...> Arguements;


        template<int ...S>
        T callFunc(PDP::seq<S...>) const
        {
            //return Delegate(std::get<S>(Arguements) ...);

            return Delegate(LD::Get<S>(Arguements) ...);
        }
    public:
        PackagedTask()
        {

        }
        PackagedTask(const LD::Promise<T> & promise,const Functor<T(Args...)> & delegate, const LD::Detail::Decay_T<Args> & ... arguements):Arguements(LD::Forward<Args>(arguements)...),Delegate(delegate),CurrentPromise(promise)
        {

        }

        inline void SetPromise(const LD::Promise<T> & currentPromise)
        {
            this->CurrentPromise = currentPromise;
        }

        inline void SetArguements( const LD::Detail::Decay_T<Args> & ... arguements)
        {
            this->Arguements = LD::Tuple<LD::Detail::Decay_T<Args>...>(LD::MakeTuple(arguements...));
        }

        inline void SetDelegate(const Functor<T(Args...)> & delegate)
        {
            this->Delegate = delegate;
        }
        virtual void Run()
        {
            try
            {
                T returnValue =  LD::Move(callFunc(typename PDP::gens<sizeof...(Args)>::type()));
                CurrentPromise.SetValue(LD::Move(returnValue));

            }catch(...)
            {
                CurrentPromise.ExceptionWasThrown();
                //indicate that the promise was broken at this point
                //in the future this should actually get the exception and help forward it along
            }
        }

    };

    /*
     It was an experiment that I'm leaving in as a reminder.  This is an implementation of Packaged Task which does not have any direct way utilizing it's API to communicate the concept of a PDP::Promise or PDP::Commitment
     //back to the caller thread

    template<typename T, template<typename> class Functor, typename ... Args>
    class PackagedTask<T(Args...),Functor,PDP::Enable_If_T<PDP::IsSame<PDP::Decay_T<T>, void>::value>>: public PDP::Task
    {
    private:
        Functor<T(Args...)> Delegate;
        PDP::Tuple<PDP::Decay_T<Args>...> Arguements;
        template<int ...S>
        T callFunc(PDP::seq<S...>) const
        {
            //return Delegate(std::get<S>(Arguements) ...);

            return Delegate(PDP::get<S>(Arguements) ...);
        }
    public:

        PackagedTask()
        {

        }
        PackagedTask(const Functor<T(Args...)> & delegate, const PDP::Decay_T<Args> & ... arguements):Arguements(PDP::Forward<Args>(arguements)...),Delegate(delegate)
        {
        }

        inline void SetArguements( const PDP::Decay_T<Args> & ... arguements)
        {
            this->Arguements = PDP::Tuple<PDP::Decay_T<Args>...>(arguements...);
        }

        inline void SetDelegate(const Functor<T(Args...)> & delegate)
        {
            this->Delegate = delegate;
        }

        virtual void Run()
        {
            try
            {
                callFunc(typename PDP::gens<sizeof...(Args)>::type());

            }catch(...)
            {
                //indicate that the commitment was broken at this point
            }

            //CurrentPromise.SetFinishedState(true);
        }
    };
     */

    /**
     @tparam T - The expected type to be streamed by the PDP::Commitment
     @tparam Functor - The expected functor type to be utilized.  std::function, PDP::Lambda, and PDP::LightWeightDelegate are examples of well accepted classes for this
     @tparam Args - The inferred arguements which will be inferred from the instance of the functor
     @brief - This class's expected uses is when you wish to dispatch a function which has a return type of void and simply want to monitor progress or get data streamed back to you from the thread
     */
    template<typename T, template<typename> class Functor, typename ... Args>
    class PackagedTask<T(LD::Commitment<T>,Args...),Functor,LD::Enable_If_T<LD::Detail::IsSame<LD::Detail::Decay_T<T>, void>::value>>: public PDP::Task
    {
    private:


        Functor<T(LD::Commitment<T>,Args...)> Delegate;


        LD::Tuple<LD::Commitment<T>,LD::Detail::Decay_T<Args>...> Arguements;


        //PDP::Commitment<T> CurrentCommitment;
        template<int ...S>
        T callFunc(PDP::seq<S...>) const
        {
            //return Delegate(std::get<S>(Arguements) ...);

            return Delegate(LD::Get<S>(Arguements) ...);

        }
    public:

        PackagedTask()
        {

        }
        PackagedTask(const LD::Commitment<T> &currentCommitment ,const Functor<T(LD::Commitment<T> &,Args...)> & delegate, const LD::Detail::Decay_T<Args> & ... arguements):Arguements(LD::Forward<Args>(arguements)...),Delegate(delegate)
        {
            LD::Get<0>(Arguements) = currentCommitment;
        }

        inline void SetArguements(const LD::Commitment<T> &currentCommitment, const LD::Detail::Decay_T<Args> & ... arguements)
        {
            this->Arguements = LD::Tuple<LD::Detail::Decay_T<Args>...>(currentCommitment,arguements...);
        }


        inline void SetCommitment(const LD::Commitment<T> & currentCommitment)
        {
            LD::Get<0>(Arguements) = currentCommitment;
            //this->CurrentCommitment = currentCommitment;
        }


        inline void SetDelegate(const Functor<T(LD::Commitment<T> , Args...)> & delegate)
        {
            this->Delegate = delegate;
        }

        virtual void Run()
        {
            try
            {
                callFunc(typename PDP::gens<sizeof...(Args)+1>::type());//it's +1 because the Tuple has a tparam which is not specified by the size of the compile time variadic arguement list

            }catch(...)
            {
                LD::Get<0>(Arguements).SetExceptionOccured();//the Commitment is the first arguement in the arguement compile time variadic list, and indicate something has gone wrong
                //in the future this should actually get the exception and help forward it along
            }

        }
    };

    /**
     @tparam T - The return type of the PDP::Promise which will provide a value sometime in the future after the function is launced on the thread.
     @tparam U - The type which will be streamed utilizing the PDP::Commitment
     @tparam Functor - The expected functor type to be utilized.  std::function, PDP::Lambda, and PDP::LightWeightDelegate are examples of well accepted classes for this
     @tparam Args - The inferred arguements which will be inferred from the instance of the functor
     @brief - This implementation of PDP::PackagedTask is exptected to be used when you want to stream data (such as a progress indicators) and want a final value at the end.  An example of this is when you download a file.  You would like to get progress indicators while the file downloads and then you want the file at the end
     */
    template<typename T, typename U, template<typename> class Functor, typename ... Args>
    class PackagedTask<T(LD::Commitment<U> ,Args...),Functor,LD::Enable_If_T<!LD::Detail::IsSame<LD::Detail::Decay_T<T>, void>::value>>: public PDP::Task
    {
    private:
        Functor<T(LD::Commitment<U>,Args...)> Delegate;
        LD::Tuple<LD::Commitment<U>,LD::Detail::Decay_T<Args>...> Arguements;
        LD::Promise<T> CurrentPromise;



        template<int ...S>
        T callFunc(PDP::seq<S...>) const
        {
            //return Delegate(std::get<S>(Arguements) ...);

            return Delegate(LD::Get<S>(Arguements) ...);

            //return T();
        }
    public:

        PackagedTask()
        {

        }
        PackagedTask(const LD::Promise<T> & currentPromise,const LD::Commitment<U> &currentCommitment ,const Functor<T(LD::Commitment<T> &,Args...)> & delegate, const LD::Detail::Decay_T<Args> & ... arguements):Arguements(LD::Forward<Args>(arguements)...),Delegate(delegate),CurrentPromise(currentPromise)
        {

            LD::Get<0>(Arguements) = currentCommitment;
        }


        inline void SetPromise(const LD::Promise<T> & currentPromise)
        {
            this->CurrentPromise = currentPromise;
        }

        inline void SetCommitment(const LD::Commitment<U> & currentCommitment)
        {
            //this->CurrentCommitment = currentCommitment;
            LD::Get<0>(Arguements) = currentCommitment;
        }


        inline void SetArguements(const LD::Commitment<U> & commitment, const LD::Detail::Decay_T<Args> & ... arguements)
        {
            this->Arguements = LD::Tuple<LD::Detail::Decay_T<Args>...>(commitment,arguements...);
        }

        inline void SetDelegate(const Functor<T(LD::Commitment<U> , Args...)> & delegate)
        {
            this->Delegate = delegate;
        }


        virtual void Run()
        {
            try
            {
                T returnValue =  LD::Move(callFunc(typename PDP::gens<sizeof...(Args)+1>::type()));//it's +1 because the Tuple has a tparam which is not specified by the size of the compile time variadic arguement list
                CurrentPromise.SetValue(LD::Move(returnValue));


            }catch(...)
            {
                LD::Get<0>(Arguements).SetExceptionOccured();
                //the Commitment is the first arguement in the arguement compile time variadic list, and indicate something has gone wrong
                //in the future this should actually get the exception and help forward it along
                CurrentPromise.ExceptionWasThrown();
            }
        }
    };
}
#endif