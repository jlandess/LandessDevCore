#ifndef REFLECTABLE_FOR_EACH_H_INCLUDED
#define REFLECTABLE_FOR_EACH_H_INCLUDED

#include <Definitions/Common.hpp>
namespace LD
{
    // ************************************************************************************************
    // Code stolen from: http://stackoverflow.com/a/6894436
    
    template<std::size_t I = 0, typename FuncT, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    for_each(LD::Tuple<Tp...> &, FuncT) // Unused arguments are given no names.
    {
    }
    
    template<PDP::UInteger I = 0, typename FuncT, typename... Tp>
    inline typename PDP::EnableIf<I < sizeof...(Tp), void>::type
    for_each(LD::Tuple<Tp...>& t, FuncT && f)
    {
        f(LD::Get<I>(t));
        for_each<I + 1, FuncT, Tp...>(t, f);
    }
}


#endif // REFLECTABLE_FOR_EACH_H_INCLUDED
