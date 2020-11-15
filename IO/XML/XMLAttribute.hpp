//
// Created by phoenixflower on 10/27/20.
//

#ifndef LANDESSDEVCORE_XMLATTRIBUTE_HPP
#define LANDESSDEVCORE_XMLATTRIBUTE_HPP
namespace LD
{
    template<typename T>
    class XMLAttribute
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
        struct IsXMLAttribute
        {
            static constexpr bool value = false;
        };

        template<typename T>
        class IsXMLAttribute<LD::XMLAttribute<T>>
        {
            static constexpr bool value = true;
        };
    }
    template<typename T>
    constexpr bool IsXMLAttribute = LD::Detail::IsXMLAttribute<T>::value;
}
#endif //LANDESSDEVCORE_XMLATTRIBUTE_HPP
