//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_CONDITIONAL_HPP
#define LANDESSDEVCORE_CONDITIONAL_HPP
namespace LD
{
    namespace Detail
    {
        template <bool _Bp, class _If, class _Then>
        struct  Conditonal {typedef _If type;};
        template <class _If, class _Then>
        struct  Conditonal<false, _If, _Then> {typedef _Then type;};


//#if _LIBCPP_STD_VER > 11
        template <bool _Bp, class _If, class _Then> using Conditional_T = typename Conditonal<_Bp, _If, _Then>::type;
//#endif
    }

    template<bool value, typename If, typename Then>
    using Conditonal = typename LD::Detail::Conditonal<value,If,Then>::type ;
}
#endif //LANDESSDEVCORE_CONDITIONAL_HPP
