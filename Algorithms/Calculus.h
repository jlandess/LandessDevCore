//
// Created by phoenixflower on 5/31/20.
//

#ifndef LANDESSDEVCORE_CALCULUS_H
#define LANDESSDEVCORE_CALCULUS_H
#include "Exponential.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "TypeTraits/Detection.hpp"
#include "Primitives/General/ContextualVariant.h"
#include "Primitives/General/Context.h"
#include "TypeTraits/Declval.hpp"
#include "Definitions/Integer.hpp"
#include "Functor/LightWeightDelegate.h"
#include "Primitives/General/Vec2.hpp"
namespace LD
{
    class InvalidComputeResult
    {

    };

    class ComputeResult
    {

    };

    class MidPointRule
    {

    };

    class LeftHandedRule
    {

    };

    class RightHandedRule
    {

    };




    /**
   @brief Integrates in the first order within the interval [a,b]
   @param integrand - The function that you wish to integrate in the interval [a,b]
   @param a - The beginning of the interval which you wish to integrate
   @param b - The end of the interval which you wish to integrate
   */
    template<typename T, typename U , typename V ,typename F,
            typename Step = decltype(LD::Declval<T>() + LD::Declval<V>()*LD::Declval<LD::UInteger>()),
            typename Res = decltype(LD::Declval<F>()(LD::Declval<Step>()*LD::Declval<V>())*LD::Declval<V>())>
     LD::Enable_If_T<
    LD::Require<
    LD::IsFloatingPoint<T>,
    LD::IsFloatingPoint<U>,
    LD::IsFloatingPoint<V>,
    LD::ConvertiblyCallable<F,Res(Step)>::Value()
    >
    ,T> RectangularIntegrate(F && integrand,
            const T  & ainput,
            const U & binput,
            const LD::Variant<LeftHandedRule,MidPointRule,RightHandedRule> & rule = LD::MidPointRule{},
            const V & dx = 0.001) noexcept
    {

        using Branch = PDP::LightWeightDelegate<Res(const Step & step, const V & dx, F &&)>;
        Res sum = Res{};
        const T & a = ainput;
        const U & b = binput;


        auto steps = LD::Ceil((LD::Abs(b-a))/dx) ;

        Branch branches[3];



        auto performLeftHandRule = [](const Step & x, const V & dx, F && integrand)->Res
        {
            return integrand(x)*dx;
        };

        auto performMidpointRule = [](const Step & x, const V & dx, F && integrand)->Res
        {

            return integrand(x+dx/Step(2.0))*dx;
        };

        auto performRightHandRule = [](const Step & x, const V & dx, F && integrand)->Res
        {
            return integrand(x+dx)*dx;
        };

        branches[0] = Branch {&performLeftHandRule, &decltype(performLeftHandRule)::operator()};
        branches[1] = Branch {&performMidpointRule, &decltype(performMidpointRule)::operator()};
        branches[2] = Branch {&performRightHandRule, &decltype(performRightHandRule)::operator()};

        auto onLeftHandedRule = [](const LeftHandedRule & rule) noexcept -> LD::UInteger{return 0;};

        auto onMidePointRule = [](const MidPointRule & rule) noexcept-> LD::UInteger{return 1;};

        auto onRightHandRule = [](const RightHandedRule & rule) noexcept-> LD::UInteger{return 2;};


        Branch targetBranch = branches[LD::Match(rule,onLeftHandedRule,onMidePointRule,onRightHandRule)];
        for (LD::UInteger i = 0; i < steps; ++i)
        {
            Step x = a + dx*i;
            sum += targetBranch(x,dx,LD::Forward<F>(integrand));
        }

        return sum;
    }


    template<typename F, typename T, typename U, typename V,
            typename Res = decltype(LD::Declval<T>() + LD::Declval<V>()*LD::Declval<LD::UInteger>())>
    LD::Enable_If_T<
    LD::Require<
    LD::IsFloatingPoint<T>,
    LD::IsFloatingPoint<U>,
    LD::IsFloatingPoint<V>,
    LD::ConvertiblyCallable<F,Res(const Res &)>::Value()
    >
    ,Res> TrapezoidIntegrate(F && integrand, const T  & ainput, const U & binput, const V & dx = 0.001) noexcept
    {
        Res sum = {};


        const T & a = ainput;
        const U & b = binput;


        LD::UInteger steps = LD::Ceil((LD::Abs(b-a))/dx) ;

        for (LD::UInteger n = 0; n< steps; ++n )
        {
            Res x0 = a + dx*n;
            //CastedType x1 = a + dx*(n+1);
            //CastedType x2 = a + dx*(n+2);
            //CastedType x3 = a + dx*(n+3);

            sum += (integrand(x0) + integrand(x0+dx))/Res{2.0};
        }

        return sum*dx;
    }

    template<typename F, typename T, typename U, typename V,
            typename Res = decltype(LD::Declval<T>() + LD::Declval<V>()*LD::Declval<LD::UInteger>())>
    LD::Enable_If_T<
            LD::Require<
                    LD::IsFloatingPoint<T>,
                    LD::IsFloatingPoint<U>,
                    LD::IsFloatingPoint<V>,
                    LD::ConvertiblyCallable<F,Res(const Res &)>::Value()
            >
            ,Res> SimpsonIntegrate(F && integrand, const T  & ainput, const U & binput, const V & dx = 0.001) noexcept
    {
        Res sum = {};
        sum = 0;
        const T & a=  ainput;
        const U & b = binput;
        LD::UInteger steps = LD::Ceil((LD::Abs(b-a))/dx) ;

        for (LD::UInteger n = 0; n<steps; ++n )
        {
            Res x = a + dx*n;

            sum += (integrand(x) + Res{4.0}*integrand(x+dx/Res{2.0}) + integrand(x+dx))/Res{6.0};
        }
        return sum*dx;
    }

    template<typename F, typename T, typename U, typename V,
            typename Res = decltype(LD::Declval<T>() + LD::Declval<V>()*LD::Declval<LD::UInteger>())>
    LD::Enable_If_T<
            LD::Require<
                    LD::IsFloatingPoint<T>,
                    LD::IsFloatingPoint<U>,
                    LD::IsFloatingPoint<V>,
                    LD::ConvertiblyCallable<F,Res(const Res &)>::Value()
            >
            ,Res> QuadratureIntegrate(F && integrand, const T  & ainput, const U & binput, const V & dx = 0.001) noexcept
    {
        const T & a=  ainput;
        const U & b = binput;
        static const Res x[]=
                {
                        Res(0.1488743389816312),
                        Res(0.4333953941292472),
                        Res(0.6794095682990244),
                        Res(0.8650633666889845),
                        Res(0.9739065285171717)

                };
        static const Res w[]=
                {
                        Res(0.2955242247147529),
                        Res(0.2692667193099963),
                        Res(0.2190863625159821),
                        Res(0.1494513491505806),
                        Res(0.0666713443086881)

                };


        Res xm= Res{0.5}*Res{(b+a)};
        Res xr= Res(0.5)*Res{(b-a)};

        Res s = {};
        for (LD::UInteger j=0;j<5;++j)
        {
            Res dx = xr*x[j];
            //dx = xr*x[j];

            s += w[j]*(integrand(xm+dx)+integrand(xm-dx));
        }
        return s *= xr;
    }

    template<typename F, typename T, typename U, typename V,typename R,typename Q,
            typename Res = decltype(LD::Declval<T>() + LD::Declval<V>()*LD::Declval<LD::UInteger>())>
    LD::Enable_If_T<
            LD::Require<
                    LD::IsFloatingPoint<T>,
                    LD::IsFloatingPoint<U>,
                    LD::IsFloatingPoint<V>,
                    LD::ConvertiblyCallable<F,Res(const Res &)>::Value(),
                    LD::ConvertiblyCallable<R,Res(const T &, const U &)>::Value()
            >
            ,Res> MonteCarloIntegrate(F && integrand, const T  & ainput, const U & binput, R && random , const Q & samples = 1000 ,const V & dx = 0.001) noexcept
    {
        Res sum = {};

        Res iterations = LD::Ceil((binput-ainput)/dx);


        // Monte Carlo Method Calculation
        for (LD::UInteger i = 0; i < iterations; i++)
        {
            for (LD::UInteger j = 0; j < samples; j++)
            {
                const Res vl = random(ainput,binput);
                sum += integrand(vl);
            }
        }

        sum /= iterations;
        sum = (sum * (binput - ainput)) / samples;

        return sum;
    }



    template<typename F, typename T, typename U, typename V,
            typename Res = decltype(LD::Declval<T>() + LD::Declval<V>()*LD::Declval<LD::UInteger>())>
    Res EulerDifferentialSolver(F && integrand, const T & maxTime, const LD::BasicVec2D<U> & initialConditions,const V & dt = 0.001) noexcept

    {
        U velocity = initialConditions.Y();
        Res position = initialConditions.X();
        Res t = initialConditions.X();
        Res time = {};
        for(time = {};time<=maxTime;time+=dt)
        {
            velocity = velocity + integrand(time) *dt;;//Regular Euler
            position = position + velocity * dt;//Regular Euler
        }
        return position;

    }

    template<typename F,typename T, typename U, typename V,
            typename Res = decltype(LD::Declval<T>() + LD::Declval<V>()*LD::Declval<LD::UInteger>())>
    Res EulerSympleticIntegrateDifferentialSolver(F && integrand, const T & maxTime, const LD::BasicVec2D<U> & initialConditions,const V & dt)
    {

        Res velocity = initialConditions.Y();
        Res position = initialConditions.X();
        Res t = initialConditions.X();
        for(Res t = 0;t<=maxTime;t+=dt)
        {

            position = position + velocity*dt;

            velocity = velocity + integrand(t)*dt;

        }
        return position;
    }

    template<typename F, typename T, typename U, typename V,
            typename Res = decltype(LD::Declval<T>() + LD::Declval<V>()*LD::Declval<LD::UInteger>())>
    Res VerletDifferentialSolver(F && integrand, const T & x,const LD::BasicVec2D<U> & initialConditions  ,const V & dt = 0.01) noexcept
    {

        Res last_acceleration = {};

        //last_acceleration= 0;

        U velocity = initialConditions.Y();

        //velocity = initialConditions.Y();

        Res position = initialConditions.X();

        //position = initialConditions.X();

        Res t = {};

        //t = 0;




        //LD::Float last_acceleration = 0;

        //velocity = initialConditions.Y();

        //position = initialConditions.X();

        t = 0;

        while (t <= x)
        {
            position += velocity * dt + ( 0.5 * last_acceleration * dt*dt );

            //LD::Float new_acceleration = force / mass;

            Res new_acceleration = integrand(t);

            Res avg_acceleration = ( last_acceleration + new_acceleration ) / 2.0;

            velocity += avg_acceleration * dt;

            t += dt;

            last_acceleration = new_acceleration;

        }
        return position;
    }

    template<typename F, typename T, typename U, typename V,
            typename Res = decltype(LD::Declval<T>() + LD::Declval<V>()*LD::Declval<LD::UInteger>())>
    Res RungaKutta4DifferentialSolver(F && function, const T & x,const LD::BasicVec2D<U> & initialCondition  ,const V & dt = V{0.01}) noexcept
    {
        const V & h = dt;
        //h = dt;
        LD::UInteger n = LD::Ceil(((x - initialCondition.X()) / h));
        //Res k1, k2, k3, k4;
        Res xi = initialCondition.X();
        // Iterate for number of iterations
        Res y = initialCondition.Y();
        LD::UInteger i = {};
        for (i=1; i<=n; i++)
        {
            // Apply Runge Kutta Formulas to find
            // next value of y
            Res k1 = h*function(xi, y);

            Res k2 = h*function(xi + Res{0.5}*h, y + Res{0.5}*k1);

            Res k3 = h*function(xi + Res{0.5}*h, y + Res{0.5}*k2);

            Res k4 = h*function(xi + h, y + k3);
            // Update next value of y
            y = y + Res{ (1.0/6.0)}*(k1 + Res{2.0}*k2 + Res{2.0}*k3 + k4);;
            // Update next value of x
            xi = xi + h;
        }
        return y;
    }
    template<typename F, typename T, typename U, typename V,
            typename Res = decltype(LD::Declval<T>() + LD::Declval<V>()*LD::Declval<LD::UInteger>())>
    Res RungaKutta4Integrate(F && integrand, const T & a, const U & b ,const V & dt) noexcept
    {
        return LD::RungaKutta4DifferentialSolver(LD::Forward<F>(integrand), b,LD::BasicVec2D<LD::Float>{0,0},dt) - LD::RungaKutta4DifferentialSolver(LD::Forward<F>(integrand), a,LD::BasicVec2D<LD::Float>{0,0},dt);
    }


    template<typename T>
    class RectangularIntegration
    {
    private:
        LD::Variant<LD::MidPointRule,LD::LeftHandedRule,LD::RightHandedRule> mRule;
    public:

        constexpr RectangularIntegration() noexcept
        {
        }
        constexpr RectangularIntegration(const LD::Variant<LD::MidPointRule,LD::LeftHandedRule,LD::RightHandedRule>& integrationMethod) noexcept :mRule(integrationMethod)
        {

        }


        template<typename F>
        const T operator()(F && integrand, const T & lowerBound, const T & upperBound, const T & dx) const
        {
            return LD::RectangularIntegrate(LD::Forward<F>(integrand), lowerBound, upperBound, this->mRule,dx);
        }
    };

    template<typename T>
    class TrapezoidalIntegration
    {
    private:

    public:

        constexpr TrapezoidalIntegration() noexcept
        {

        }



        template<typename F>
        const T operator()(F && integrand, const T & lowerBound, const T & upperBound, const T & dx) const
        {
            return LD::TrapezoidIntegrate(LD::Forward<F>(integrand), lowerBound, upperBound,dx);
        }
    };

    template<typename T>
    class SimpsonIntegration
    {
    private:

    public:

        SimpsonIntegration()
        {

        }



        template<typename F>
        const T operator()(F && integrand, const T & lowerBound, const T & upperBound, const T & dx) const
        {
            return LD::SimpsonIntegrate(LD::Forward<F>(integrand), lowerBound, upperBound,dx);
        }
    };

    template<typename T>
    class QuadratureIntegration
    {
    private:

    public:

        QuadratureIntegration()
        {

        }
        template<typename F>
        const T operator()(F && integrand, const T & lowerBound, const T & upperBound, const T & dx) const
        {
            return LD::QuadratureIntegrate(LD::Forward<F>(integrand), lowerBound, upperBound,dx);
        }
    };

}
#endif //LANDESSDEVCORE_CALCULUS_H
