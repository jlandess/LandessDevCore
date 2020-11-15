//
// Created by phoenixflower on 10/27/20.
//

#ifndef LANDESSDEVCORE_XMLVALUE_HPP
#define LANDESSDEVCORE_XMLVALUE_HPP
namespace LD
{
    template<typename T>
    class XMLValue
    {
    private:
        T mValue;
    public:

        T & Value() noexcept
        {
            return this->mValue;
        }

        const T & Value() const noexcept
        {
            return this->mValue;
        }
    };

    namespace Detail
    {
        template<typename T>
        struct IsXMLValue
        {
            static constexpr bool value = false;
        };

        template<typename T>
        class IsXMLValue<LD::XMLValue<T>>
        {
            static constexpr bool value = true;
        };
    }
    template<typename T>
    constexpr bool IsXMLValue = LD::Detail::IsXMLValue<T>::value;
}
#endif //LANDESSDEVCORE_XMLVALUE_HPP
