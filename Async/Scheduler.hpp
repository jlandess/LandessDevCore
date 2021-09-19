//
// Created by phoenixflower on 9/9/21.
//

#ifndef LANDESSDEVCORE_SCHEDULER_HPP
#define LANDESSDEVCORE_SCHEDULER_HPP
#include "Definitions/Integer.hpp"
#include <vector>
#include "Functor/fixed_size_function.hpp"
#include "Atomic/Atomic.h"
#include "Channel.hpp"
#include "Thread.h"
namespace LD
{
    namespace Async
    {


        class Scheduler
        {
        private:
            LD::Channel<LD::fixed_size_function<void()>> mFunctions;
            LD::Atomic<LD::UInteger> mNumberOfThreads;
            LD::Atomic<LD::UInteger> mStatus;
            std::vector<LD::CUTThread> mThreads;
            static void * InternalThreadFunction(void * data)
            {
                Scheduler * instance = (Scheduler*)data;


                while (instance->mStatus.load(LD::AcquireRelease) == true)
                {
                    instance->mFunctions >> [](LD::fixed_size_function<void()> function)
                    {
                        function();
                    };
                }

                return nullptr;
            }
            inline void InitThreads()
            {
                LD::UInteger numberOfInitialThreads = this->mNumberOfThreads.load(LD::AcquireRelease);
                for(LD::UInteger threadIndex = 0;threadIndex<numberOfInitialThreads;++threadIndex)
                {
                    LD::CUTThread thread = LD::CutStartThread(&Scheduler::InternalThreadFunction, this);
                    this->mThreads.push_back(thread);
                }
            }
        public:
            Scheduler(LD::UInteger numberOfThreads) noexcept
            {
                this->mNumberOfThreads.store(numberOfThreads,LD::AcquireRelease);
                this->mStatus.store(true,LD::AcquireRelease);
                InitThreads();
            }
            ~Scheduler()
            {
                this->mStatus.store(false,LD::AcquireRelease);
                LD::UInteger numberOfThreads = this->mThreads.size();
                for(LD::UInteger n = 0 ;n<numberOfThreads;++n)
                {
                    LD::CutEndThread(this->mThreads[n]);
                }
            }

            template<typename F>
            void Enqueue(F && function) noexcept
            {
                this->mFunctions << LD::fixed_size_function<void()>{LD::Forward<F>(function)};
            }

            template<typename F>
            Scheduler & operator << (F && function) noexcept
            {
                this->mFunctions << LD::fixed_size_function<void()>{LD::Forward<F>(function)};
                return (*this);
            }

            template<typename F>
            void Post(F && function)
            {
                this->mFunctions << LD::fixed_size_function<void()>{LD::Forward<F>(function)};
            }

            template<typename F>
            void Execute(F && function)
            {
                this->mFunctions << LD::fixed_size_function<void()>{LD::Forward<F>(function)};
            }

            template<typename F>
            auto AsyncExecute(F && function)
            {
                this->mFunctions << LD::fixed_size_function<void()>{LD::Forward<F>(function)};
            }

            template<typename F>
            void Defer(F && function)
            {
                this->mFunctions << LD::fixed_size_function<void()>{LD::Forward<F>(function)};
            }

            template<typename F>
            void SyncExecute(F && function)
            {
                this->mFunctions << LD::fixed_size_function<void()>{LD::Forward<F>(function)};
            }
        };
    }
}
#endif //LANDESSDEVCORE_SCHEDULER_HPP
