//
// Created by James Landess on 2/12/20.
//

#ifndef LANDESSDEVCORE_FORMATTEDFLOATINGPOINT_HPP
#define LANDESSDEVCORE_FORMATTEDFLOATINGPOINT_HPP

#include "Definitions/Common.hpp"
#include "RenderableConcept.hpp"
#include "TermBoxRenderingContext.hpp"
namespace LD
{
    //template<typename T, class sfinae = void>
    //class FormattedFloatingPoint;

    template<typename T>
    class FormattedFloatingPoint
     {
     private:
         T mCurrentFloatingPointNumber;
         LD::UInteger mPrecision;
     public:
         inline constexpr explicit FormattedFloatingPoint(const T & value, const LD::UInteger & precison = 0) noexcept :mCurrentFloatingPointNumber(value),mPrecision(precison){}
         inline constexpr FormattedFloatingPoint() noexcept : mCurrentFloatingPointNumber(T()) {}
         inline constexpr const T & Value() const noexcept {return this->mCurrentFloatingPointNumber;}
         inline constexpr T & Value() noexcept {return this->mCurrentFloatingPointNumber;}
         inline constexpr const T & Precision() const noexcept {return this->mPrecision;}
         inline constexpr T & Precision() noexcept {return this->mPrecision;}

         const LD::TermBoxRenderContext & operator()(const LD::TermBoxRenderContext & context,const LD::Detail::tVec2<LD::Integer> & translation) const noexcept
        {
             context.Render(LD::ToImmutableString(this->mCurrentFloatingPointNumber,this->mPrecision),translation);
             return context;
        }


        inline   LD::Detail::tVec2<LD::UInteger > GetRenderableDimensions() const  noexcept
        {
            auto immutableString = LD::ToImmutableString(this->mCurrentFloatingPointNumber,this->mPrecision);
            return LD::Detail::tVec2<LD::UInteger >{immutableString.GetSize(),1};
        }

     };


    template<typename T>
    inline   LD::Detail::tVec2<LD::UInteger > GetRenderableDimensions(const LD::FormattedFloatingPoint<T> & object)  noexcept
    {
        auto immutableString = LD::ToImmutableString(object.Value(),object.Precision());
        return LD::Detail::tVec2<LD::UInteger >{immutableString.GetSize(),1};
    }

    template<typename T>
    inline const TermBoxRenderContext & Render(const TermBoxRenderContext & context,const LD::Detail::tVec2<LD::Integer> & translation, const LD::FormattedFloatingPoint<T> & formattedFloatingPointNumber)  noexcept
    {
        return context.Render(LD::ToImmutableString(formattedFloatingPointNumber.Value(),formattedFloatingPointNumber.Precision()),translation);
    }
}
#endif //LANDESSDEVCORE_FORMATTEDFLOATINGPOINT_HPP
