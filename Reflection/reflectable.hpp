#ifndef REFLECTABLE_REFLECTABLE_H_INCLUDED
#define REFLECTABLE_REFLECTABLE_H_INCLUDED

#include "tagged_tuple.hpp"
#include "Algorithms/CompileTimeControlFlow.hpp"
#include "Primitives/General/typestring.hpp"
#include "TypeTraits/SearchableVariadicPack.hpp"
#include "ConstexprFor.hpp"
#include "TypeTraits/IsMemberPointer.hpp"
#include "Primitives/General/mapboxvariant.hpp"
namespace LD
{
#define RecflectableStringification( x ) #x
    struct ReflectableWarrant
    {

        constexpr  static bool Value = {true};

        constexpr const bool GetWarrant() const
        {
            return Value;
        }
    };


    namespace Detail
    {
        template <class T>
        using ReflectableWarrantCheck = decltype(LD::Declval<T>().GetReflectionWarrant());



        template <class T>
        class HasReflectableWarrant
        {
            template<class U, class = typename std::enable_if<!LD::IsMemberPointer<decltype(&U::ReflectionWarrant)>>::type>
            static constexpr LD::TrueType check(int);
            template <class>
            static constexpr LD::FalseType check(...);
        public:
            static constexpr bool value = decltype(check<T>(0))::value;
        };
    }






    template<typename T>
    constexpr bool IsReflectable = LD::Exists<LD::Detail::ReflectableWarrantCheck,T>;


    namespace Detail
    {
        template<typename ... A>
        struct IsNamedPack
        {

            using SearchablePack = LD::SearchableVariadicPack<A...>;



            constexpr static const bool IsNamed()
            {
                if constexpr (sizeof...(A) > 0 && sizeof...(A)%2 == 0)//make sure the number of arguements are even and a positive number
                {
                    bool isReady = true;

                    for_constexpr<for_bounds<0, sizeof...(A)>>([&](auto I)
                    {
                        if constexpr (I %2 == 0)
                        {

                            //make sure all of the even indices are typestrings

                           isReady = isReady && LD::IsTypeString< LD::Extract<I,SearchablePack> >;
                        }
                    });

                    return isReady;
                }


                return false;
            }

        };
    }


    template<typename ... A>
    constexpr bool IsNamedPack = LD::Detail::IsNamedPack<A...>::IsNamed();


    template<typename T, class = void>
    class Reflectable;

    template <typename T,typename ...Args>
    class Reflectable<T(Args...),LD::Enable_If_T<
            LD::Require<
                    LD::IsTypeString<T>

                    >>> : public LD::tagged_tuple<decltype("ClassName"_ts),const char *,Args...>
    {
    public:

        using ValueTypeList = typename LD::tagged_tuple<decltype("ClassName"_ts),const char *,Args...>::ValueTypeList;
        using KeyTypeList = typename LD::tagged_tuple<decltype("ClassName"_ts),const char *,Args...>::KeyTypeList;
        typedef  LD::SearchableVariadicPack<decltype("ClassName"_ts),const char *,Args...> SearchableReflectionList;

        constexpr static LD::UInteger NumberOfMembers = {(sizeof...(Args)/2)+1};

        constexpr static ReflectableWarrant ReflectionWarrant = {};


        constexpr static T GetClassNameTypeString()
        {
            return T{};
        }

        constexpr static const char * GetClassName()
        {
            return T::data();
        }
        inline Reflectable()
        {
            (*this)["ClassName"_ts] = T::data();
        }
        template <typename FuncT>
        void iterate(const FuncT& f)
        {
            for_each(*this, f);
        }
        
        
        template<char ... Characters>
        constexpr auto & operator[](const LD::TypeString<Characters...> & characters)
        {
            return (*this).template Get<LD::TypeString<Characters...>>();
        }
        template<char ... Characters>
        constexpr const auto & operator[](const LD::TypeString<Characters...> & characters) const
        {
            return (*this).template Get<LD::TypeString<Characters...>>();
        }

        template<LD::UInteger Index>
        constexpr auto  operator[](const LD::CompileTimeIndex<Index> & compiletimeIndex) const
        {
            return (*this).template GetMemberName<Index>();
        }



        template<LD::UInteger Index>
        constexpr auto GetMemberName() const
        {

            using extracted =  LD::Extract<(2*Index), SearchableReflectionList>;

            //return extracted::data();

            return extracted{};
        }


        constexpr ReflectableWarrant GetReflectionWarrant() const
        {
            return ReflectionWarrant;
        }



    };




    namespace Detail
    {
        template<typename T, class = void>
        struct GetTypeString;


        template<typename T>
        struct GetTypeString<T,LD::Enable_If_T<LD::Require<LD::IsReflectable<T>>>>{

            static decltype(T::GetClassNameTypeString()) Value()
            {
                return T::GetClassNameTypeString();
            }


        };

        template<typename T>
        struct GetPrimitiveTypeString;


        template<>
        struct GetPrimitiveTypeString<bool>
        {
            static auto Value()
            {
                return "bool"_ts;
            }
        };


        template<>
        struct GetPrimitiveTypeString<char>{

            static auto Value()
            {
                return "char"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<short>{

            static auto Value()
            {
                return "short"_ts;
            }


        };
        template<>
        struct GetPrimitiveTypeString<int>{

            static auto Value()
            {
                return "int"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<long>{

            static auto Value()
            {
                return "long"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<float>{

            static auto Value()
            {
                return "float"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<double>{

            static auto Value()
            {
                return "double"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<unsigned  char>{

            static auto Value()
            {
                return "uchar"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<unsigned short>{

            static auto Value()
            {
                return "ushort"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<unsigned int>{

            static auto Value()
            {
                return "uint"_ts;
            }


        };

        template<>
        struct GetPrimitiveTypeString<unsigned long>{

            static auto Value()
            {
                return "ulong"_ts;
            }


        };

        template<typename T>
        struct GetTypeString<T,LD::Enable_If_T<LD::Require<LD::IsPrimitive<T>>>>{

            static auto Value()
            {
                return  LD::Detail::GetPrimitiveTypeString<T>::Value();
            }


        };

        template<typename T>
        struct GetTypeString<T,LD::Enable_If_T <LD::FallBack<LD::IsPrimitive<T>,LD::IsReflectable<T>>>>{


            static_assert(LD::Negate<LD::IsPrimitive<T>>,"You can only get a typestring for a primitive type or a reflective type");
            static_assert(LD::Negate<LD::IsReflectable<T>>,"You can only get a typestring for a primitive type or a reflective type");
        };




    }



}


#endif // REFLECTABLE_REFLECTABLE_H_INCLUDED
