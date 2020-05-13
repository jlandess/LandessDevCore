cmake_minimum_required(VERSION 3.15)
include_directories(".")
project(LandessDevCore)
set(CMAKE_CXX_STANDARD 17)

add_executable(LandessDevCore
        main.cpp
        LandessDevCore.h
        Core/HeteregenousTuple.h
        Chrono/Timer.cpp Chrono/Timer.h
        Primitives/General/typestring.h
        Primitives/General/mapboxoptional.h
        Primitives/General/mapboxrecursive_wrapper.h
        Primitives/General/mapboxvariant_cast.h
        Primitives/General/mapboxvariant_io.h
        Primitives/General/mapboxvariantvisitor.h
        Primitives/General/mapboxvariant.h
        Primitives/General/StaticArray.h
        Functor/Lambda.h
        Memory/ElementReference.h
        Algorithms/Sampler.h
        Algorithms/fsm.h Memory/Optional.h
        Primitives/General/Vec2.h
        Algorithms/Exponential.h Algorithms/vdt/asin.h
        Algorithms/vdt/atan.h
        Algorithms/vdt/atan2.h Algorithms/vdt/cos.h
        Algorithms/vdt/exp.h Algorithms/vdt/identity.h
        Algorithms/vdt/inv.h Algorithms/vdt/log.h
        Algorithms/vdt/sin.h Algorithms/vdt/sincos.h
        Algorithms/vdt/sqrt.h Algorithms/vdt/tan.h
        Algorithms/vdt/vdtcore_common.h
        Algorithms/vdt/vdtmath.cpp
        Algorithms/vdt/vdtmath.h
        Primitives/General/ImproperFraction.h
        Primitives/General/Immutable.h
        Algorithms/CompileTimeControlFlow.h
        Primitives/General/Hash.h
        MVC/TermBoxApplication.h
        MVC/TermBoxApplication.cpp
        MVC/RenderingContext.h
        MVC/TermBoxEnumerations.h
        TermBox/termbox.c
        TermBox/utf8.c
        Definitions/TimeExtension.h
        MVC/TermBoxMenu.h
        MVC/TermBoxImmutableLabel.h
        MVC/TermBoxImmutableTextArea.h
        Primitives/General/Vec3.h
        Primitives/General/Vec4.h
        Primitives/General/Mat2x2.h
        Primitives/General/Mat2x3.h
        Primitives/General/Mat2x4.h
        Primitives/General/Mat3x2.h
        Primitives/General/Mat3x3.h
        Primitives/General/Mat3x4.h
        Primitives/General/Mat4x2.h
        Primitives/General/Mat4x3.h
        Primitives/General/Mat4x4.h
        Primitives/General/Quaternion.h
        Algorithms/Periodic.h
        Primitives/General/Bitmap.h
        MVC/TermBoxCursorNavigator.h
        Primitives/General/Observer.h
        Primitives/General/ctre.hpp
        TypeTraits/VariadicPack.h
        TypeTraits/Decay.h
        TypeTraits/Conditional.h
        TypeTraits/IsArray.h
        TypeTraits/RemoveExtent.h
        TypeTraits/AddPointer.h
        TypeTraits/RemoveCV.h
        TypeTraits/RemoveReference.h
        TypeTraits/IntegralConstant.h
        TypeTraits/IsUnion.h
        TypeTraits/IsVoid.h
        TypeTraits/IsReference.h
        TypeTraits/IsNullPtr.h
        TypeTraits/IsIntegralType.h
        TypeTraits/IsClassType.h
        TypeTraits/IsFunction.h
        TypeTraits/IsSame.h
        TypeTraits/__two.h
        TypeTraits/IsConst.h
        TypeTraits/TypeList.h
        TypeTraits/IntegerSequence.h
        TypeTraits/IsConstructible.h
        TypeTraits/IsAssignable.h
        TypeTraits/Cast.h
        TypeTraits/ResultOf.h
        TypeTraits/Limit.h
        TypeTraits/Calleable.h
        TypeTraits/Detection.h
        TypeTraits/IsConvertible.h
        TypeTraits/Declval.h
        TypeTraits/RemoveConst.h
        TypeTraits/IsDestructible.h
        TypeTraits/RemoveAllExtents.h
        TypeTraits/IsVolatile.h
        TypeTraits/AddLValueReference.h
        TypeTraits/AddRValueReference.h
        TypeTraits/Invoke.h TypeTraits/IsKindOf.h
        TypeTraits/IsBaseOf.h
        Primitives/General/Tuple.h
        MVC/SchedulingQuittingPredicate.h
        MVC/FormattedFloatingPoint.h
        MVC/RenderableConcept.h
        MVC/Renderable.h Primitives/General/StringView.h MVC/LinearAlgebraRenderable.h
        Process/clock.posix.c
        Process/drain.c
        Process/error.posix.c
        Process/handle.posix.c
        Process/init.posix.c
        Process/pipe.posix.c
        Process/process.posix.c
        Process/redirect.c
        Process/redirect.posix.c
        Process/reproc.c
        Process/run.c
        Process/options.c)