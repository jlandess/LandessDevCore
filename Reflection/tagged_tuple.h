#ifndef REFLECTABLE_TAGGED_TUPLE_H_INCLUDED
#define REFLECTABLE_TAGGED_TUPLE_H_INCLUDED


#include <Definitions/Common.h>
namespace LD
{
    // ************************************************************************************************
    // Code stolen from: http://stackoverflow.com/a/13066078
    
    template<typename... Ts> struct typelist{
        template<typename T> using prepend = typelist<T, Ts...>;
    };
    
    template<typename T, typename... Ts> struct index;
    template<typename T, typename... Ts> struct index<T, T, Ts...>:
    LD::Detail::IntegralConstant<int, 0> {};
    template<typename T, typename U, typename... Ts> struct index<T, U, Ts...>:
            LD::Detail::IntegralConstant<int, index<T, Ts...>::value + 1> {};
    
    template<int n, typename... Ts> struct nth_impl;
    template<typename T, typename... Ts> struct nth_impl<0, T, Ts...> {
        using type = T; };
    template<int n, typename T, typename... Ts> struct nth_impl<n, T, Ts...> {
        using type = typename nth_impl<n - 1, Ts...>::type; };
    template<int n, typename... Ts> using nth = typename nth_impl<n, Ts...>::type;
    
    template<int n, int m, typename... Ts> struct extract_impl;
    template<int n, int m, typename T, typename... Ts>
    struct extract_impl<n, m, T, Ts...>: extract_impl<n, m - 1, Ts...> {};
    template<int n, typename T, typename... Ts>
    struct extract_impl<n, 0, T, Ts...> { using types = typename
        extract_impl<n, n - 1, Ts...>::types::template prepend<T>; };
    template<int n, int m> struct extract_impl<n, m> {
        using types = typelist<>; };
    template<int n, int m, typename... Ts> using extract = typename
    extract_impl<n, m, Ts...>::types;
    
    template<typename S, typename T> struct tt_impl;
    template<typename... Ss, typename... Ts>
    struct tt_impl<typelist<Ss...>, typelist<Ts...>>: public LD::Tuple<Ts...>
    {
        template<typename... Args> tt_impl(Args &&...args):
        LD::Tuple<Ts...>(LD::Forward<Args>(args)...) {}
        template<typename S>
        constexpr nth<index<S, Ss...>::value, Ts...>& Get()
        {
            return LD::Get<index<S, Ss...>::value>(*this);
            
        }

         template<typename S> constexpr const nth<index<S, Ss...>::value, Ts...>& Get() const
        {
            return LD::Get<index<S, Ss...>::value>(*this);

        }



        template<LD::UInteger Index>
        constexpr auto Get()
        {
            return LD::Get<Index>(*this);
        }

        template<LD::UInteger Index>
        constexpr auto Get() const
        {
            return LD::Get<Index>(*this);
        }
        
    };
    template<typename... Ts> struct tagged_tuple:
    tt_impl<extract<2, 0, Ts...>, extract<2, 1, Ts...>>
    {
        template<typename... Args> tagged_tuple(Args &&...args):
        tt_impl<extract<2, 0, Ts...>, extract<2, 1, Ts...>>(
                                                            LD::Forward<Args>(args)...) {}
    };
}


#endif // REFLECTABLE_TAGGED_TUPLE_H_INCLUDED
