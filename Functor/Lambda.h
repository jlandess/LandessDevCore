//
//  LambdaFunction.h
//  StaticReflection
//
//  Created by James Landess on 8/15/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef __StaticReflection__LambdaFunction__
#define __StaticReflection__LambdaFunction__

namespace PDP
{
    namespace DataStructures
    {
        template<typename T> class LambdaExecutor {};

        template <typename Out, typename... In> class LambdaExecutor<Out(In...)> {
        public:
            Out operator()(In ... in) const
            {
                return executeLambda(lambda, in...);
            }

        protected:
            LambdaExecutor(void *&lambda) : lambda(lambda) {}

            ~LambdaExecutor() {}

            template <typename T> void generateExecutor(T const &lambda)
            {
                executeLambda = [](void *lambda, In... arguments) -> Out
                {
                    return ((T *)lambda)->operator()(arguments...);
                };
            }

            void receiveExecutor(LambdaExecutor<Out(In...)> const &other)
            {
                executeLambda = other.executeLambda;
            }

        private:
            void *&lambda;
            Out (*executeLambda)(void *, In...);
        };

        template <typename... In> class LambdaExecutor<void(In...)> {
        public:
            void operator()(In ... input) const
            {
                //assert(lambda != nullptr);
                executeLambda(lambda, input...);
            }

        protected:
            LambdaExecutor(void *&lambda) : lambda(lambda) {}

            ~LambdaExecutor() {}

            template <typename T> void generateExecutor(T const &lambda)
            {
                executeLambda = [](void *lambda, In... arguments)
                {
                    return ((T *)lambda)->operator()(arguments...);
                };
            }

            void receiveExecutor(LambdaExecutor<void(In...)> const &other)
            {
                executeLambda = other.executeLambda;
            }

        private:
            void *&lambda;
            void (*executeLambda)(void *, In...);
        };

        // Lambda contains most of the lambda management code and can be used
        // directly in external code.
        template <typename T> class Lambda {};

        template <typename Out, typename ...In> class Lambda<Out(In...)> : public LambdaExecutor<Out(In...)>
        {
        public:

            Lambda() : LambdaExecutor<Out(In...)>(lambda),
                       lambda(nullptr), deleteLambda(nullptr), copyLambda(nullptr)
            {
            }

            Lambda(Lambda<Out(In...)> const &other) : LambdaExecutor<Out(In...)>(lambda),
                                                      lambda(other.copyLambda ? other.copyLambda(other.lambda) : nullptr),
                                                      deleteLambda(other.deleteLambda), copyLambda(other.copyLambda)
            {
                this->receiveExecutor(other);
            }

            template<typename T>
            Lambda(T const &lambda) : LambdaExecutor<Out(In...)>(this->lambda), lambda(nullptr)
            {
                // Copy should set all variables
                copy(lambda);
            }





            ~Lambda()
            {
                if (deleteLambda != nullptr) deleteLambda(lambda);
            }

            Lambda<Out(In...)> &operator =(Lambda<Out(In...)> const &other)
            {
                this->lambda = other.copyLambda ? other.copyLambda(other.lambda) : nullptr;
                this->receiveExecutor(other);
                this->deleteLambda = other.deleteLambda;
                this->copyLambda = other.copyLambda;
                return *this;
            }

            template<typename T> Lambda<Out(In...)> &operator =(T const &lambda)
            {
                copy(lambda);
                return *this;
            }

            operator bool()
            {
                return lambda != nullptr;
            }

        private:
            template<typename T>
            void copy(T const &lambda)
            {
                if (this->lambda != nullptr) deleteLambda(this->lambda);
                this->lambda = new T(lambda);

                this->generateExecutor(lambda);

                deleteLambda = [](void *lambda)
                {
                    delete (T *)lambda;
                };

                copyLambda = [](void *lambda) -> void *
                {
                    return lambda ? new T(*(T *)lambda) : nullptr;
                };
            }

            void *lambda;
            void (*deleteLambda)(void *);
            void *(*copyLambda)(void *);
        };

        template<typename T>
        class PackagedTask
        {

        };

        template<typename T,class...Args>
        class PackagedTask<T(Args...)>
        {
        private:
            PDP::DataStructures::Lambda<T(Args...)> Function;
        public:
            inline PackagedTask();
            inline PackagedTask(const PDP::DataStructures::Lambda<T(Args...)> & function);
            inline void SetFunction(const PDP::DataStructures::Lambda<T(Args...)> & function);
            inline PDP::DataStructures::Lambda<void()> GetCallBack(Args && ... arguements) const;
        };

        template<typename T, class ...Args>
        PackagedTask<T(Args...)>::PackagedTask()
        {

        }

        template<typename T, class ...Args>
        PackagedTask<T(Args...)>::PackagedTask(const PDP::DataStructures::Lambda<T(Args...)> & function):Function(function)
        {

        }

        template<typename T, class ...Args>
        void PackagedTask<T(Args...)>::SetFunction(const PDP::DataStructures::Lambda<T (Args...)> &function)
        {
            this->Function = function;
        }

        template<typename T, class ...Args>
        PDP::DataStructures::Lambda<void()> PackagedTask<T(Args...)>::GetCallBack(Args && ... arguements) const
        {
            PDP::DataStructures::Lambda<void()> callback([&]()
                                                         {
                                                             this->Function(arguements...);
                                                         });

            return callback;
        }
    }
}

#endif /* defined(__StaticReflection__LambdaFunction__) */
