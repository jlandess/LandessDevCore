//
// Created by phoenixflower on 9/17/21.
//

#ifndef LANDESSDEVCORE_EXECUTE_HPP
#define LANDESSDEVCORE_EXECUTE_HPP
#include "SyncWait.hpp"
namespace LD
{
    namespace Async
    {
        namespace Detail
        {
            template<typename T>
            struct _empty_promise
            {

                //_state<T> * pst;

                //_empty_promise(_state<T> * state) noexcept: pst{state}{}
                template<int I, typename ... A> void _set(A ... xs) noexcept
                {
                    //auto lk = std::unique_lock{pst->mtx};
                    //pst->data.template emplace<I>(xs...);
                    //pst->cv.notify_one();
                }

                template<typename ... A>
                void set_value(A ... vs) noexcept{
                    //_set<2>(vs...);
                }

                template<typename  A>
                void set_exception(A e) noexcept
                {
                    //_set<1>(e);
                }
            };
        }
        template<typename Task, typename  T = LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<Task>{}))>>
        LD::Enable_If_T<LD::Require<LD::CT::IsSame(LD::Type<void>{},LD::Type<T>{})>,void> Execute(Task  task) noexcept
        {

            //task.mRunnable(LD::Async::Detail::_empty_promise<int>());
        }
    }
}
#endif //LANDESSDEVCORE_EXECUTE_HPP
