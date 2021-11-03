//
// Created by phoenixflower on 9/10/21.
//

#ifndef LANDESSDEVCORE_SYNCWAIT_HPP
#define LANDESSDEVCORE_SYNCWAIT_HPP
#include <mutex>
#include <condition_variable>
#include <memory>
#include "SumTypes/MPark/variant.hpp"
#include <exception_ptr.hpp>
#include "Then.hpp"
#include "WhenAllPredicate.hpp"
#include "WhenOnlyPredicate.hpp"
#include "Algorithms/Visitation.hpp"
namespace LD
{
    namespace Async
    {
        namespace Detail
        {
            template<typename T>
            struct _state
            {
                std::mutex mtx;
                std::condition_variable cv;
                mpark::variant<LD::NullClass,std::exception_ptr, T> data;
            };

            template<>
            struct _state<void>
            {
                std::mutex mtx;
                std::condition_variable cv;
                mpark::variant<LD::NullClass,std::exception_ptr,LD::UInteger> data;
            };





            template<typename T>
            struct _promise
            {

                static constexpr LD::UInteger Number = 0;
                _state<T> * pst;

                _promise(_state<T> * state) noexcept: pst{state}{}
                template<int I, typename ... A> void _set(A ... xs) noexcept
                {
                    auto lk = std::unique_lock{pst->mtx};
                    pst->data.template emplace<I>(LD::Move(xs)...);
                    pst->cv.notify_one();
                }

                template<typename ... A>
                void set_value(A ... vs) noexcept{_set<2>(LD::Move(vs)...);}

                template<typename  A>
                void set_exception(A e) noexcept
                {
                    _set<1>(e);
                }
            };


            template<>
            struct _promise<void>
            {
                static constexpr LD::UInteger Number = 1;
                _state<void> * pst;

                _promise(_state<void> * state) noexcept: pst{state}{}


                template<int I, typename ... A>
                void _set(A ... xs) noexcept
                {
                    auto lk = std::unique_lock{pst->mtx};
                    pst->data.template emplace<I>(xs...);
                    pst->data = LD::UInteger {1};
                    pst->cv.notify_one();
                }

                //template<typename ... A>
                void set_value() noexcept{
                    _set<2>(LD::UInteger{1});
                }

                template<typename  A>
                void set_exception(A e) noexcept
                {
                    _set<1>(e);
                }
            };

            template<typename T>
            struct _AtomicState
            {
                std::mutex mtx;
                typedef T AtomicStateType;
                LD::Atomic<LD::UInteger> mStatus;
                mpark::variant<LD::NullClass,std::exception_ptr, T> data;
                _AtomicState() noexcept
                {
                    this->mStatus.store(false,LD::AcquireRelease);
                }
            };
            template<typename T>
            struct _AtomicPromise
            {

                //_AtomicState<T> * pst;
                LD::Atomic<_AtomicState<T> *> mAtomicPST;
                _AtomicPromise() noexcept
                {
                    mAtomicPST.load(nullptr,LD::AcquireRelease);
                }
                _AtomicPromise(_AtomicState<T> * state) noexcept//: pst{state}
                {
                    mAtomicPST.store(state,LD::AcquireRelease);
                }
                //_AtomicPromise(LD::Atomic<_AtomicState<T> *> * state) noexcept: mAtomicPST{state}{}
                template<int I, typename ... A> void _set(A ... xs) noexcept
                {
                    _AtomicState<T> * ptr = mAtomicPST.load(LD::AcquireRelease);
                    if (ptr != nullptr)
                    {

                    }
                    //if (pst != nullptr)
                    {
                        //pst->mStatus.store(true,LD::AcquireRelease);
                        //pst->data.template emplace<I>(xs...);
                    }

                }

                template<typename ... A>
                void set_value(A ... vs) noexcept {_set<2>(vs...);}

                template<typename  A>
                void set_exception(A e) noexcept
                {
                    _set<1>(e);
                }

                void Reset()
                {
                    mAtomicPST.store(nullptr,LD::AcquireRelease);
                    //this->mAtomicPST = nullptr;
                }
            };
        }

        template<typename Task, typename  T = LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<Task>{}))>>
        LD::Enable_If_T<LD::Negate<LD::Require<LD::CT::IsSame(LD::Type<void>{},LD::Type<T>{})>>,T> SyncWait(Task task) noexcept
        {
            LD::Async::Detail::_state<T> state;


            task.mRunnable(LD::Async::Detail::_promise<T>(&state));
            {
                auto lk = std::unique_lock{state.mtx};

                state.cv.wait(lk,[&state]
                {
                    return state.data.index() != 0;
                });
            }


            if (state.data.index() == 1)
            {
                std::rethrow_exception(mpark::get<1>(state.data));
            }


            if constexpr(LD::CT::IsSame(LD::Type<void>{},LD::Type<T>{}))
            {
                return;
            }else
            {
                return LD::Move(mpark::get<2>(state.data));;
            }

        }

        template<typename Task, typename  T = LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<Task>{}))>>
        LD::Enable_If_T<LD::Require<LD::CT::IsSame(LD::Type<void>{},LD::Type<T>{})>,T> SyncWait(Task task) noexcept
        {
            LD::Async::Detail::_state<void> state;

            task.mRunnable(LD::Async::Detail::_promise<void>(&state));
            {
                auto lk = std::unique_lock{state.mtx};

                state.cv.wait(lk,[&state]
                {
                    return state.data.index() != 0;
                });
            }



            if (state.data.index() == 1)
            {
                std::rethrow_exception(mpark::get<1>(state.data));
            }
        }

        template<typename ... A, typename T = LD::Tuple<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<A>{}))> ...>>
        T SyncWait(LD::Tuple<A...> senders) noexcept
        {

            constexpr LD::UInteger NumberOfSenders = LD::CT::TupleSize(LD::Type<LD::Tuple<A...>>{});
            T ret;
            LD::For<NumberOfSenders>([&](
                    auto I) noexcept
            {
                LD::Get(LD::Get<I>(ret)) = LD::Async::SyncWait(LD::Get(LD::Get<I>(senders)));

                return true;
            });

            return ret;
        }

        template<typename ... A, typename T = LD::Tuple<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<A>{}))> ...>>
        T SyncWait(const LD::Async::WhenAllPredicate<A...>  input) noexcept
        {
            //LD::CT::DebugTemplate<T>{};
            //using WhenPredicateType = typename LD::Async::WhenAllPredicate<T>::PredicateType ;
            //constexpr LD::UInteger NumberOfSenders = LD::CT::TupleSize(LD::Type<T>{});
            //T ret;
            /*
            LD::For<NumberOfSenders>([&](
                    auto I)
            {

                LD::Get(LD::Get<I>(ret)) = LD::Async::SyncWait(LD::Get(LD::Get<I>(senders.Value())));

                return true;
            });

             */
            auto senders = input.Value();
            constexpr LD::UInteger NumberOfSenders = LD::CT::TupleSize(LD::Type<LD::Tuple<A...>>{});
            T ret;
            LD::For<NumberOfSenders>([&](
                    auto I) noexcept
            {

                LD::Get(LD::Get<I>(ret)) = LD::Async::SyncWait(LD::Get(LD::Get<I>(senders)));

                return true;
            });

            return ret;
        }
        /*
        template<typename ... A, typename TL = LD::CT::DeDuplicateTypeList<LD::CT::TypeList<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<A>{}))> ...>>>
        LD::Enable_If_T<LD::Require<(LD::CT::TypeListSize(LD::Type<TL>{}) > 1)>,LD::CT::RebindList<TL,LD::Variant>> SyncWait(const LD::Async::WhenAnyPredicate<A...>  input) noexcept
        {
            LD::CT::TypeListSize(LD::Type<TL>{});
            //LD::StaticArray<LD::Variant<A...>,sizeof...(A)> variant;
            LD::StaticArray<LD::Tuple<LD::Variant<A...>,LD::Variant<LD::Async::Detail::_AtomicState<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<A>{}))>> ...>>,sizeof...(A)> runnables;

            LD::BackInserter inserter{runnables};
            auto senders = input.Value();
            LD::For<sizeof...(A)>([](
                    auto I,
                    auto senders,
                    auto backInserter)
            {
                LD::CT::RemoveQualifiers(LD::Type<decltype(LD::Get(LD::Get<I>(senders)))>{});
                using CurrentRet = LD::GetType<decltype(LD::Async::CT::RunnableType(LD::CT::RemoveQualifiers(LD::Type<decltype(LD::Get(LD::Get<I>(senders)))>{})))>;
                LD::Variant<A...> sender = LD::Get(LD::Get<I>(senders));
                LD::Variant<LD::Async::Detail::_AtomicState<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<A>{}))>> ...> state = LD::Async::Detail::_AtomicState<CurrentRet>{};

                backInserter = LD::Tuple<LD::Variant<A...>,LD::Variant<LD::Async::Detail::_AtomicState<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<A>{}))>> ...>>{sender,state};
                return true;
            },senders);


            for(LD::UInteger n = 0;n<sizeof...(A);++n)
            {
                auto & runner = LD::Get(LD::Get<0>(runnables[n]));
                auto & runnerState = LD::Get(LD::Get<1>(runnables[n]));

                auto function = [](auto & task, auto & runnerState) noexcept
                {
                    using StateType = LD::Detail::Decay_T<decltype(runnerState)>;
                    using StateUnderlyingType = typename StateType::AtomicStateType;
                    task.mRunnable(LD::Async::Detail::_AtomicPromise<StateUnderlyingType>{&runnerState});
                };
                LD::Visit(function,runner,runnerState);
            }
            bool waiting = true;



            while (waiting == true)
            {
                for(LD::UInteger n = 0;n<sizeof...(A);++n)
                {
                    auto & runner = LD::Get(LD::Get<0>(runnables[n]));
                    auto & runnerState = LD::Get(LD::Get<1>(runnables[n]));

                    auto function = [](auto & task, auto & runnerState) noexcept -> LD::Optional<LD::CT::RebindList<TL,LD::Variant>>
                    {
                        LD::UInteger state = runnerState.mStatus.load(LD::AcquireRelease);
                        if(state == true)
                        {
                            return LD::Optional<LD::CT::RebindList<TL,LD::Variant>>{mpark::get<2>(runnerState.state.data)};
                        }
                        return LD::Optional<LD::CT::RebindList<TL,LD::Variant>>{};
                    };
                    LD::Optional<LD::CT::RebindList<TL,LD::Variant>> runnableRet = LD::Visit(function,runner,runnerState);
                    if (runnableRet.HasValue())
                    {
                        waiting = false;
                        return *runnableRet;
                    }
                }
            }
            return LD::CT::RebindList<TL,LD::Variant>{};
        }
         */

        /*
        template<typename ... T,
                typename List = LD::CT::TypeList<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<T>{}))> ...>,
                typename TL = LD::CT::DeDuplicateTypeList<LD::CT::TypeList<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<T>{}))> ...>>,
                typename Ret = LD::Variant<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<T>{}))> ...>>
        Ret SyncWait(LD::Async::WhenAnyPredicate<T...> whenAnyPredicate) noexcept
        {
            Ret objectToReturn;

            using StatesTable = LD::Tuple<LD::Async::Detail::_AtomicState<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<T>{}))>>...>;
            using StatesTableReference = LD::Tuple<LD::Atomic<LD::Async::Detail::_AtomicState<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<T>{}))>>*>...>;
            auto senders = whenAnyPredicate.Value();

            StatesTable states;
            StatesTableReference stateReferences;
            LD::For<sizeof...(T)>([](
                    auto I,
                    auto senders,
                    StatesTable & states,
                    StatesTableReference & stateReferences) noexcept
            {
                using CurrentType = LD::CT::TypeAtIndex<I,List>;


                LD::Async::Detail::_AtomicState<CurrentType> * state = (LD::Async::Detail::_AtomicState<CurrentType> * )&LD::Get<I>(states);
                //LD::Atomic<LD::Async::Detail::_AtomicState<CurrentType> *> * atomic =  (LD::Atomic<LD::Async::Detail::_AtomicState<CurrentType> *> *)&LD::Get(LD::Get<I>(stateReferences));
                //atomic->store(state,LD::AcquireRelease);
                LD::Get(LD::Get<I>(senders)).mRunnable(
                        LD::Async::Detail::_AtomicPromise<CurrentType>{
                                state
                        }
                );

                return true;
            },senders,states,stateReferences);



           // while(true){}

            bool shouldRun = true;

            while (shouldRun == true)
            {
                LD::For<sizeof...(T)>([](
                        auto I,
                        bool & shouldRun,
                        StatesTable & table,
                        Ret & objectToReturn) noexcept
                {
                    LD::UInteger taskStatus = LD::Get(LD::Get<I>(table)).mStatus.load(LD::AcquireRelease);
                    if (taskStatus == true)
                    {
                        //mpark::get<2>(state.data);
                        //objectToReturn = mpark::get<2>(LD::Get(LD::Get<I>(table)).data);
                        shouldRun = false;
                        return false;
                    }
                    return true;
                },shouldRun,states,objectToReturn);
            }


            LD::For<sizeof...(T)>([](
                    auto I,
                    auto senders,
                    StatesTable & states,
                    StatesTableReference & stateReferences) noexcept
            {
                  using CurrentType = LD::CT::TypeAtIndex<I,List>;

                  LD::Get(LD::Get<I>(states)).Reset();

                  return true;
            },senders,states,stateReferences);

            return objectToReturn;
        }
         */

        /*
        template<typename ... A, typename TL = LD::CT::DeDuplicateTypeList<LD::CT::TypeList<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<A>{}))> ...>>>
        LD::Enable_If_T<LD::Require<(LD::CT::TypeListSize(LD::Type<TL>{}) == 1)>,typename LD::CT::TypeAtIndex<0,TL>::type> SyncWait(const LD::Async::WhenOnlyPredicate<A...>  input) noexcept
        {
            //LD::CT::TypeListSize(LD::Type<TL>{});
            //LD::Async::Detail::_AtomicState<int> state;
            LD::StaticArray<LD::Tuple<LD::Variant<A...>,LD::Variant<LD::Async::Detail::_AtomicState<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<A>{}))>> ...>>,sizeof...(A)> variant;
            return typename LD::CT::TypeAtIndex<0,TL>::type{};
        }
         */


    }
}
#endif //LANDESSDEVCORE_SYNCWAIT_HPP
