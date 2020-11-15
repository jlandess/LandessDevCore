//
// Created by phoenixflower on 11/4/20.
//

#ifndef LANDESSDEVCORE_CSSATTRIBUTES_HPP
#define LANDESSDEVCORE_CSSATTRIBUTES_HPP
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/Detection.hpp"
#include "Primitives/General/Vec4.hpp"
namespace LD
{
    namespace SPA
    {
        enum AlignContentTag{AlignContentStart = 0,AlignContentEnd,AlignFlexStart,AlignFlexEnd,AlignContentCenter ,AlignNormal ,AlignBaseline,AlignFirstBaseline,AlignLastBaseline,AlignContentSpaceBetween,AlignContentSpaceAround,AlignEvenly,AlignStretch};
        enum AlignItemsTag{AlignItemsNormal=0,AlignItemsFlexStart,AlignItemsFlexEnd,AlignItemsCenter,AlignItemsStart,AlignItemsEnd,AlignItemsSelfStart,AlignItemsSelfEnd,AlignItemsBaseline,AlignItemsFirstBaseline,AlignItemsLastBaseline,AlignItemsStretch};
        enum AlignSelfTag{AlignSelfAuto,AlignSelfNormal,AlighSelfSelfStart,AlignSelfSelfEnd,AlignSelfFlexStart,AlignSelfFlexEnd,AlignSelfCenter,AlignSelfBaseline,AlignSelfFirstBaseline,AlignSelfLastBaseline,AlignSelfStretch};
        enum AngleTag{AngleDegree=0,AngleRad,AngleGrad,AngleTurn};
        enum ColorAdjustTag{ColorAdjustEconomy=0,ColorAdjustExact};
        enum ColumnFillTag{ColumnFillBalance,ColumnFillBalanceAll};
        enum ColumnRuleStyleTag{ColumnRuleStyleDotted,ColumnRuleStyleSolid,ColumnStyleRuleDouble,ColumnStyleRidge};
        enum ColumnRuleWidthTag{ColumnRuleThin,ColumnRuleMedium,ColumnRuleThick};
        enum ColumnSpanTag{ColumnSpanNone,ColumnSpanAll};
        enum FlexBoxTag{FlexBoxStart=0,FlexBoxEnd};
    }
}

namespace LD
{
    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<LD::Detail::IsSame<T,LD::SPA::AlignContentTag>::value>,LD::ImmutableString<13>> ToImmutableString(const T & typeOfInput) noexcept
    {
        LD::StaticArray<LD::ImmutableString<13>,4> results;


        LD::ImmutableString<13> start;
        start[0] = 's';
        start[1] = 't';
        start[2] = 'a';
        start[3] = 'r';
        start[4] = 't';

        LD::ImmutableString<13> center;
        center[0] = 'c';
        center[1] = 'e';
        center[2] = 'n';
        center[3] = 't';
        center[4] = 'e';
        center[5] = 'r';

        LD::ImmutableString<10> spaceBetween;
        spaceBetween[0] = 's';
        spaceBetween[1] = 'p';
        spaceBetween[2] = 'a';
        spaceBetween[3] = 'c';
        spaceBetween[4] = 'e';
        spaceBetween[5] = '-';
        spaceBetween[6] = 'b';
        spaceBetween[7] = 'e';
        spaceBetween[8] = 't';
        spaceBetween[9] = 'w';
        spaceBetween[10] = 'e';
        spaceBetween[11] = 'e';
        spaceBetween[12] = 'n';

        LD::ImmutableString<10> spaceAround;
        spaceAround[0] = 's';
        spaceAround[1] = 'p';
        spaceAround[2] = 'a';
        spaceAround[3] = 'c';
        spaceAround[4] = 'e';
        spaceAround[5] = '-';
        spaceAround[6] = 'a';
        spaceAround[7] = 'r';
        spaceAround[8] = 'o';
        spaceAround[9] = 'u';
        spaceAround[10] = 'n';
        spaceAround[11] = 'd';




        results[0] = start;
        results[1] = center;
        results[2] = spaceBetween;
        results[3] = spaceAround;
        return results[typeOfInput];
    }
}

namespace LD
{
    namespace SPA
    {
        class AlignContent
        {
        private:
            AlignContentTag mContent;
        public:
        };

        class AlignItems
        {

        };

        class AlignSelf
        {

        };

        class All
        {

        };

        class Angle
        {

        };

        //--------------------------------------------------------Beginning of Animation--------------------------------------------------
        class Animation
        {

        };

        class AnimationDelay
        {

        };

        class AnimationDirection
        {

        };

        class AnimationDuration
        {

        };

        class AnimationFillMode
        {

        };

        class AnimationIterationCount
        {

        };

        class AnimationName
        {

        };

        class AnimationPlayState
        {

        };

        class AnimationTimingFunction
        {

        };
        //--------------------------------------------------------End of Animation--------------------------------------------------

        class BackdropFilter
        {

        };

        class BackfaceVisibility
        {

        };

        class Background
        {

        };

        class BackgroundAttachment
        {

        };

        class BackgroundBlendMode
        {

        };

        class BackgroundClip
        {

        };

        class BackgroundColor
        {

        };

        class BackgroundImage
        {

        };

        class BackgroundOrigin
        {

        };

        class BackgroundPosition
        {

        };

        class BackgroundRepeat
        {

        };

        class BackgroundSize
        {

        };

        class BlockSize
        {

        };

        class Border
        {

        };

        class BorderBlock
        {

        };

        class BorderBlockColor
        {

        };

        class BorderBlockEnd
        {

        };

        class BorderBlockEndColor
        {

        };

        class BorderBlockEndStyle
        {

        };

        class BorderBlockEndWidth
        {

        };

        class BorderBlockStart
        {

        };

        class BorderBlockStartColor
        {

        };

        class BorderBlockStartStyle
        {

        };

        class BorderBlockStyleWidth
        {

        };

        class BorderBlockStyle
        {

        };

        class BorderBlockWidth
        {

        };

        class BorderBottom
        {

        };

        class BorderBottomColor
        {

        };

        class BorderBottomLeftRadius
        {

        };

        class BorderBottomRightRadius
        {

        };

        class BorderBottomStyle
        {

        };

        class BorderBottomWidth
        {

        };

        class BorderCollapse
        {

        };

        class BorderColor
        {

        };

        class BorderEndEndRadius
        {

        };

        class BorderEndStartRadius
        {

        };

        class BorderImage
        {

        };

        class BorderImageOutset
        {

        };

        class BorderImageRepeat
        {

        };

        class BorderImageSlice
        {

        };

        class BorderImageSource
        {

        };

        class BorderImageWidth
        {

        };

        class BorderInline
        {

        };

        class BorderInlineColor
        {

        };

        class BorderInlineEnd
        {

        };

        class BorderInlineEndColor
        {

        };

        class BorderInlineEndStyle
        {

        };

        class BorderInlineEndWidth
        {

        };

        class BorderInlineStart
        {

        };

        class BorderInlineStartColor
        {

        };

        class BorderInlineStartStyle
        {

        };


        class BorderInlineStartWidth
        {

        };

        class BorderInlineStyle
        {

        };

        class BorderInlineWidth
        {

        };

        class BorderLeft
        {

        };

        class BorderLeftColor
        {

        };

        class BorderLeftStyle
        {

        };

        class BorderLeftWidth
        {

        };


        class BorderRadius
        {

        };

        class BorderRight
        {

        };

        class BorderRightColor
        {

        };

        class BorderRightStyle
        {

        };

        class BorderRightWidth
        {

        };

        class BorderSpacing
        {

        };

        class BorderStartEndRadius
        {

        };

        class BorderStartStartRadius
        {

        };

        class BorderStyle
        {

        };

        class BorderTop
        {

        };

        class BorderTopColor
        {

        };

        class BorderTopLeftRadius
        {

        };

        class BorderTopRightRadius
        {

        };

        class BorderTopStyle
        {

        };

        class BorderTopWidth
        {

        };

        class BorderWidth
        {

        };

        class Bottom
        {

        };

        class BoxDecorationBreak
        {

        };

        class BoxShadow
        {

        };

        class BoxSizing
        {

        };

        class BreakAfter
        {

        };

        class BreakBefore
        {

        };

        class BreakInside
        {

        };

        class CaptionSide
        {

        };

        class CaretColor
        {

        };

        class Color
        {
        private:
            LD::BasicVec4D<LD::Float> mColor;
        public:
        };


        class ColorAdjust
        {
        private:
            ColorAdjustTag mValue;
        public:
        };

        class ColumnCount
        {
        private:
            LD::UInteger mValue;
        public:
        };

        class ColumnFill
        {
        private:
            ColumnFillTag mValue;
        public:
        };

        class ColumnRuleColor
        {
        private:
            LD::BasicVec4D<LD::Float> mColor;
        public:
        };

        class ColumnRuleStyle
        {
        private:
            ColumnRuleStyleTag mValue;
        public:
        };

        class ColumnRuleWidth
        {
        private:
            ColumnRuleWidthTag mValue;
        public:
        };

        class ColumnSpan
        {
        private:
            ColumnSpanTag mValue;
        public:
        };



        class FontSizeAdjust
        {
        private:
            LD::Float mAmount;
        public:
        };


        //--------------------------------------------------------Beginning of Flex Properties---------------------------------------------
        class Display
        {

        };

        class FlexDirection
        {

        };

        class FlexWrap
        {

        };

        class FlexBasis
        {

        };

        class FlexJustifyContent
        {

        };

        class FlexAlignSelf
        {

        };

        class FlexAlignItems
        {

        };

        class FlexAlignContent
        {

        };

        class FlexGrow
        {

        };

        class FlexShrink
        {

        };

        class Flex
        {

        };

        class Order
        {

        };
    }
}
#endif //LANDESSDEVCORE_CSSATTRIBUTES_HPP
