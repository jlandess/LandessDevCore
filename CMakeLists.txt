cmake_minimum_required(VERSION 3.15)
include_directories(".")
project(LandessDevCore)
set(CMAKE_CXX_STANDARD 17)

add_executable(LandessDevCore
        main.cpp
        LandessDevCore.hpp
        Core/HeteregenousTuple.hpp
        Chrono/Timer.cpp Chrono/Timer.h
        Primitives/General/typestring.hpp
        Primitives/General/mapboxoptional.hpp
        Primitives/General/mapboxrecursive_wrapper.hpp
        Primitives/General/mapboxvariant_cast.hpp
        Primitives/General/mapboxvariant_io.hpp
        Primitives/General/mapboxvariantvisitor.h
        Primitives/General/mapboxvariant.hpp
        Primitives/General/StaticArray.hpp
        Functor/Lambda.h
        Memory/ElementReference.h
        Algorithms/Sampler.hpp
        Algorithms/fsm.hpp Memory/Optional.h
        Primitives/General/Vec2.hpp
        Algorithms/Exponential.hpp Algorithms/vdt/asin.h
        Algorithms/vdt/atan.h
        Algorithms/vdt/atan2.h Algorithms/vdt/cos.h
        Algorithms/vdt/exp.h Algorithms/vdt/identity.h
        Algorithms/vdt/inv.h Algorithms/vdt/log.h
        Algorithms/vdt/sin.h Algorithms/vdt/sincos.h
        Algorithms/vdt/sqrt.h Algorithms/vdt/tan.h
        Algorithms/vdt/vdtcore_common.h
        Algorithms/vdt/vdtmath.cpp
        Algorithms/vdt/vdtmath.h
        Primitives/General/ImproperFraction.hpp
        Primitives/General/Immutable.hpp
        Algorithms/CompileTimeControlFlow.hpp
        Primitives/General/Hash.hpp
        MVC/TermBoxApplication.hpp
        MVC/TermBoxApplication.cpp
        MVC/RenderingContext.hpp
        MVC/TermBoxEnumerations.hpp
        TermBox/termbox.c
        TermBox/utf8.c
        Definitions/TimeExtension.hpp
        MVC/TermBoxMenu.hpp
        MVC/TermBoxImmutableLabel.hpp
        MVC/TermBoxImmutableTextArea.hpp
        Primitives/General/Vec3.hpp
        Primitives/General/Vec4.hpp
        Primitives/General/Mat2x2.hpp
        Primitives/General/Mat2x3.hpp
        Primitives/General/Mat2x4.hpp
        Primitives/General/Mat3x2.hpp
        Primitives/General/Mat3x3.hpp
        Primitives/General/Mat3x4.hpp
        Primitives/General/Mat4x2.hpp
        Primitives/General/Mat4x3.hpp
        Primitives/General/Mat4x4.hpp
        Primitives/General/Quaternion.hpp
        Algorithms/Periodic.hpp
        Primitives/General/Bitmap.hpp
        MVC/TermBoxCursorNavigator.hpp
        Primitives/General/Observer.hpp
        Primitives/General/ctre.hpp
        TypeTraits/VariadicPack.hpp
        TypeTraits/Decay.hpp
        TypeTraits/Conditional.hpp
        TypeTraits/IsArray.hpp
        TypeTraits/RemoveExtent.hpp
        TypeTraits/AddPointer.hpp
        TypeTraits/RemoveCV.hpp
        TypeTraits/RemoveReference.hpp
        TypeTraits/IntegralConstant.hpp
        TypeTraits/IsUnion.hpp
        TypeTraits/IsVoid.hpp
        TypeTraits/IsReference.hpp
        TypeTraits/IsNullPtr.hpp
        TypeTraits/IsIntegralType.hpp
        TypeTraits/IsClassType.hpp
        TypeTraits/IsFunction.hpp
        TypeTraits/IsSame.hpp
        TypeTraits/__two.hpp
        TypeTraits/IsConst.hpp
        TypeTraits/TypeList.hpp
        TypeTraits/IntegerSequence.hpp
        TypeTraits/IsConstructible.hpp
        TypeTraits/IsAssignable.hpp
        TypeTraits/Cast.hpp
        TypeTraits/ResultOf.hpp
        TypeTraits/Limit.hpp
        TypeTraits/Calleable.hpp
        TypeTraits/Detection.hpp
        TypeTraits/IsConvertible.hpp
        TypeTraits/Declval.hpp
        TypeTraits/RemoveConst.hpp
        TypeTraits/IsDestructible.hpp
        TypeTraits/RemoveAllExtents.hpp
        TypeTraits/IsVolatile.hpp
        TypeTraits/AddLValueReference.hpp
        TypeTraits/AddRValueReference.hpp
        TypeTraits/Invoke.hpp TypeTraits/IsKindOf.hpp
        TypeTraits/IsBaseOf.hpp
        Primitives/General/Tuple.hpp
        MVC/SchedulingQuittingPredicate.hpp
        MVC/FormattedFloatingPoint.hpp
        MVC/RenderableConcept.hpp
        MVC/Renderable.hpp Primitives/General/StringView.hpp MVC/LinearAlgebraRenderable.hpp
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
        Process/options.c
        IO/unqlite.c
        Examples/ReflectionExample.hpp
        Examples/ReflectionExample.cpp
        Examples/TermBoxMenuExample.hpp
        Examples/TermBoxMenuExample.cpp
        TypeTraits/IsPointer.hpp
        TypeTraits/EnableIf.hpp
        IO/Database.hpp
        TypeTraits/IsSwappable.hpp
        Algorithms/Swap.hpp
        Algorithms/StringToPrimitive.hpp
        Examples/ReflectionDemoTypes.h
        Algorithms/StringAsNumber.h
        Primitives/General/Pair.h
        Async/Thread.cpp
        Async/ThreadAffinity.h
        Primitives/General/Context.h
        Primitives/General/ContextualVariant.h
        IO/UnqliteDatabaseBackend.h
        IO/DatabaseOperationResult.h
        IO/DatabaseOpenMode.h
        Primitives/General/Span.hpp
        TypeTraits/IsIterable.h
        Primitives/General/GenericIterator.h
        TypeTraits/Iterable.h TypeTraits/IsImmutable.h
        TypeTraits/Resizeable.h
        TypeTraits/StaticallySized.h
        Algorithms/Calculus.h
        Geometry/Circle.h
        Geometry/Square.h
        Geometry/Box.h
        Geometry/Cube.h
        Geometry/Plane.h
        Geometry/Sphere.h
        Geometry/Ray.h
        Geometry/Particle.h
        Algorithms/NewtonAppleDelanauy.h
        Geometry/Rectangle.h
        Geometry/Capsule.h
        Geometry/UniformBSpline.h
        Geometry/BezierCurve.h
        Geometry/Line.h
        Geometry/LineSegment.h
        Algorithms/KarnaughMap.h
        IO/FetchRequest.h
        IO/InsertionRequest.h
        IO/CachedDatabase.h
        IO/DatabaseEntity.h
        TypeTraits/Type.h
        IO/JsonDatabaseBackend.h
        IO/TieredDatabase.h
        Examples/CalculusExample.hpp
        Examples/CalculusExample.cpp
        Examples/MultithreadingExample.hpp
        Examples/MultithreadingExample.cpp
        MVC/TUI/TUITextLabel.h MVC/TUI/TUITextInput.h MVC/Mouse.h MVC/Cusor.h)
target_link_libraries(LandessDevCore pthread)
target_link_libraries(LandessDevCore ncurses)