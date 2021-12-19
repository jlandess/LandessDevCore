//
//  OptionalReference.h
//  DataStructures
//
//  Created by James Landess on 3/20/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_OptionalReference_h
#define DataStructures_OptionalReference_h

#include "Optional.h"
#include "ElementReference.h"

namespace LD
{
    template<typename T>
    class Optional<T&>
    {
    private:
        LD::ElementReference<T> mReference;
    public:
        Optional() noexcept:mReference{nullptr}{}
        Optional(T * object) noexcept:mReference{object}{}
        Optional(T & object) noexcept:mReference{object}{}

        T & operator*() noexcept{ return (*this->Pointer());}

        const T & operator*() const noexcept{ return *mReference.GetPointer();}

        operator bool() const noexcept
        {
            return this->mReference.GetPointer() != nullptr;
        }
        bool HasValue() const noexcept
        {
            return this->mReference.GetPointer() != nullptr;
        }
        T & GetValue() noexcept
        {
            return *mReference.GetPointer();
        }
        const T & GetValue() const noexcept
        {
            return *mReference.GetPointer();
        }
        T * Pointer() noexcept{return this->mReference.GetPointer();}

        T * operator->() noexcept{return this->Pointer();}

        template<typename X,typename R, typename ... P, typename V = T>
        LD::Enable_If_T<
                LD::Require<
                        LD::Detail::IsClass<X>::value,
                        LD::Either<LD::CT::IsSame(LD::Type<V>{},LD::Type<X>{}),LD::Detail::IsBaseOf_V<X,V>>
                >,
                LD::optional_lite::Optional<R>> Map(R (X::* function_to_bind)(P...) const, P && ... parameters) const
        {
            if (this->HasValue())
            {
                const T & object = this->GetValue();
                const T * ptr = (const T*)&object;
                LD::LightWeightDelegate<R(P...)> delegate{ptr,function_to_bind};
                return LD::optional_lite::Optional<R>{delegate(LD::Forward<P>(parameters)...)};
            }
            return LD::optional_lite::Optional<R>{};
        };

        template<typename X,typename R, typename ... P, typename V = T>
        LD::Enable_If_T<
                LD::Require<
                        LD::Detail::IsClass<X>::value,
                        LD::Either<LD::CT::IsSame(LD::Type<V>{},LD::Type<X>{}),LD::Detail::IsBaseOf_V<X,V>>
                >,
                LD::optional_lite::Optional<R>> Map(R (X::* function_to_bind)(P...) const noexcept, P && ... parameters) const noexcept
        {
            if (this->HasValue())
            {
                const T & object = this->GetValue();
                const T * ptr = (const T*)&object;
                LD::LightWeightDelegate<R(P...)> delegate{ptr,function_to_bind};
                return LD::optional_lite::Optional<R>{delegate(LD::Forward<P>(parameters)...)};
            }
            return LD::optional_lite::Optional<R>{};
        };

        template<typename X,typename R, typename ... P, typename V = T>
        LD::Enable_If_T<
                LD::Require<
                        LD::Detail::IsClass<X>::value,
                        LD::Either<LD::CT::IsSame(LD::Type<V>{},LD::Type<X>{}),LD::Detail::IsBaseOf_V<X,V>>
                >,
                LD::optional_lite::Optional<R>> Map(R (X::* function_to_bind)(P...)  , P && ... parameters)
        {
            if (this->HasValue())
            {
                const T & object = this->GetValue();
                const T * ptr = (const T*)&object;
                LD::LightWeightDelegate<R(P...)> delegate{ptr,function_to_bind};
                return LD::optional_lite::Optional<R>{delegate(LD::Forward<P>(parameters)...)};
            }
            return LD::optional_lite::Optional<R>{};
        };

        template<typename X,typename R, typename ... P, typename V = T>
        LD::Enable_If_T<
                LD::Require<
                        LD::Detail::IsClass<X>::value,
                        LD::Either<LD::CT::IsSame(LD::Type<V>{},LD::Type<X>{}),LD::Detail::IsBaseOf_V<X,V>>
                >,
                LD::optional_lite::Optional<R>> Map(R (X::* function_to_bind)(P...)  noexcept, P && ... parameters)  noexcept
        {
            if (this->HasValue())
            {
                const T & object = this->GetValue();
                const T * ptr = (const T*)&object;
                LD::LightWeightDelegate<R(P...)> delegate{ptr,function_to_bind};
                return LD::optional_lite::Optional<R>{delegate(LD::Forward<P>(parameters)...)};
            }
            return LD::optional_lite::Optional<R>{};
        };

        template<typename X,typename R, typename ... P, typename V = T>
        LD::optional_lite::Optional<R> Map(R (*function_to_bind)(const V &,P...) , P && ... parameters) const
        {
            if (this->HasValue())
            {
                const T & object = this->GetValue();
                //const T * ptr = (const T*)&object;
                //LD::LightWeightDelegate<R(P...)> delegate{function_to_bind};

                return LD::optional_lite::Optional<R>{function_to_bind(object,LD::Forward<P>(parameters)...)};
            }
            return LD::optional_lite::Optional<R>{};
        };

        template<typename X,typename R, typename ... P, typename V = T>
        LD::optional_lite::Optional<R> Map(R (*function_to_bind)(const V &,P...) noexcept , P && ... parameters) const noexcept
        {
            if (this->HasValue())
            {
                const T & object = this->GetValue();
                //const T * ptr = (const T*)&object;
                //LD::LightWeightDelegate<R(P...)> delegate{function_to_bind};

                return LD::optional_lite::Optional<R>{function_to_bind(object,LD::Forward<P>(parameters)...)};
            }
            return LD::optional_lite::Optional<R>{};
        };

        template<typename X,typename R, typename ... P, typename V = T>
        LD::optional_lite::Optional<R> Map(R (*function_to_bind)( V &,P...) , P && ... parameters)
        {
            if (this->HasValue())
            {
                T & object = this->GetValue();
                //const T * ptr = (const T*)&object;
                //LD::LightWeightDelegate<R(P...)> delegate{function_to_bind};

                return LD::optional_lite::Optional<R>{function_to_bind(object,LD::Forward<P>(parameters)...)};
            }
            return LD::optional_lite::Optional<R>{};
        };

        template<typename X,typename R, typename ... P, typename V = T>
        LD::optional_lite::Optional<R> Map(R (*function_to_bind)( V &,P...) noexcept , P && ... parameters) noexcept
        {
            if (this->HasValue())
            {
                T & object = this->GetValue();
                //const T * ptr = (const T*)&object;
                //LD::LightWeightDelegate<R(P...)> delegate{function_to_bind};

                return LD::optional_lite::Optional<R>{function_to_bind(object,LD::Forward<P>(parameters)...)};
            }
            return LD::optional_lite::Optional<R>{};
        };

        template<typename ... P,typename F ,typename V = T,
                typename Ret = decltype(LD::Declval<F>()(LD::Declval<V&>(),LD::Declval<P>()...))>
        auto Map(F && function, P && ... parameters) noexcept -> LD::Detail::Conditional_T<LD::CT::IsSame(LD::Type<Ret>{},LD::Type<void>{}),void,LD::optional_lite::Optional<Ret>>
        {

            if (this->HasValue())
            {
                T & object = this->GetValue();
                //const T * ptr = (const T*)&object;
                //LD::LightWeightDelegate<R(P...)> delegate{function_to_bind};

                if constexpr(LD::CT::IsSame(LD::Type<Ret>{},LD::Type<void>{}))
                {
                    function(object,LD::Forward<P>(parameters)...);

                }else
                {
                    return LD::optional_lite::Optional<Ret>{function(object,LD::Forward<P>(parameters)...)};
                }

            }
            if constexpr(!LD::CT::IsSame(LD::Type<Ret>{},LD::Type<void>{}))
            {
                return LD::optional_lite::Optional<Ret>{};
            }
        }

        template<typename ... P,typename F ,typename V = T,
                typename Ret = decltype(LD::Declval<F>()(LD::Declval<const V&>(),LD::Declval<P>()...))>
        auto Map(F && function, P && ... parameters) const noexcept -> LD::optional_lite::Optional<Ret>
        {
            if (this->HasValue())
            {
                const T & object = this->GetValue();
                //const T * ptr = (const T*)&object;
                //LD::LightWeightDelegate<R(P...)> delegate{function_to_bind};

                return LD::optional_lite::Optional<Ret>{function(object,LD::Forward<P>(parameters)...)};
            }
            return LD::optional_lite::Optional<Ret>{};
        }

        template<typename X,typename R, typename ... P, typename V = T>
        LD::Enable_If_T<
                LD::Require<
                        LD::Detail::IsClass<X>::value,
                        LD::Either<LD::CT::IsSame(LD::Type<V>{},LD::Type<X>{}),LD::Detail::IsBaseOf_V<X,V>>
                >,
                R> MapOr(R (X::* function_to_bind)(P...) , R && value,P && ... parameters)
        {
            if (this->HasValue())
            {
                const T & object = this->GetValue();
                const T * ptr = (const T*)&object;
                LD::LightWeightDelegate<R(P...)> delegate{ptr,function_to_bind};
                return LD::optional_lite::Optional<R>{delegate(LD::Forward<P>(parameters)...)};
            }
            return value;
        }

        template<typename X,typename F,typename R, typename ... P, typename V = T>
        LD::Enable_If_T<
                LD::Require<
                        LD::Detail::IsClass<X>::value,
                        LD::Either<LD::CT::IsSame(LD::Type<V>{},LD::Type<X>{}),LD::Detail::IsBaseOf_V<X,V>>
                >,
                R> MapOr(R (X::* function_to_bind)(P...) , F && functor,P && ... parameters)
        {
            if (this->HasValue())
            {
                const T & object = this->GetValue();
                const T * ptr = (const T*)&object;
                LD::LightWeightDelegate<R(P...)> delegate{ptr,function_to_bind};
                return LD::optional_lite::Optional<R>{delegate(LD::Forward<P>(parameters)...)};
            }
            return functor(LD::Forward<P>(parameters)...);
        }

        template<class X,typename R, typename ... P>
        R AndThen(R (X::* function_to_bind)(P...) , P && ... parameters)
        {
            return LD::optional_lite::Optional<R>{};
        }

        template<class X,typename R, typename ... P>
        LD::optional_lite::Optional<R> AndThen(R (X::* function_to_bind)(P...)  const , P && ... parameters) noexcept
        {
            return LD::optional_lite::Optional<R>{};
        }

        template<typename F>
        void Else(F && function) noexcept
        {

        }

        template<typename F, typename F1>
        void MapOrElse(F && function, F1 &&) noexcept
        {

        }

        T DisJunction(T && value)
        {
            if (this->HasValue())
            {
                return (*this);
            }
            return LD::Forward<T>(value);
        }

        LD::optional_lite::Optional<T> Conjunction(T && value)
        {
            if(this->HasValue())
            {
                return LD::optional_lite::Optional<T>{};
            }
            return LD::Forward<T>(value);
        }

        LD::optional_lite::Optional<T> Take() noexcept
        {
            if(this->HasValue())
            {
                LD::optional_lite::Optional<T> optional{LD::Move((*this))};
                this->reset();
                return optional;
            }
            return LD::optional_lite::Optional<T>{};
        };
    };
    template<typename T>
    class OptionalReference: public LD::Optional<PDP::ElementReference<T>>
    {
    public:
        
        inline OptionalReference(): LD::Optional<PDP::ElementReference<T>>()
        {
            
        }
        
        inline OptionalReference(const PDP::ElementReference<T> & elementReference): LD::Optional<PDP::ElementReference<T>>(elementReference)
        {
            
        }
    };


}
#endif
