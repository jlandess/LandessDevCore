//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_GAUSSIANELIMINATION_H
#define LANDESSDEVCORE_GAUSSIANELIMINATION_H
#include "Definitions/Integer.hpp"
#include "Primitives/General/StaticArray.hpp"
#include "Primitives/General/Matrix.hpp"
namespace LD
{
    template<typename Precision, const LD::UInteger systemSize = 10> class GausianElimination
    {
    private:

        LD::Integer n;
        //PDP::DataStructures::Linear::StaticMatrix<Precision,systemSize,systemSize+1> a;
        LD::ImmutableMatrix<Precision,systemSize,systemSize+1> a;

        Precision Solutions[systemSize+1];
        LD::StaticArray<Precision,systemSize> Results;

        void forwardSubstitution()
        {
            PDP::Integer i, j, k, max;
            Precision t;
            for (i = 0; i < n; ++i)
            {
                max = i;
                for (j = i + 1; j < n; ++j)
                    if (a[j][i] > a[max][i])
                        max = j;

                for (j = 0; j < n + 1; ++j)
                {
                    t = a[max][j];
                    a[max][j] = a[i][j];
                    a[i][j] = t;
                }

                for (j = n; j >= i; --j)
                    for (k = i + 1; k < n; ++k)
                        a[k][j] -= a[k][i]/a[i][i] * a[i][j];
            }
        }

        void reverseElimination()
        {
            PDP::Integer i, j;
            for (i = n - 1; i >= 0; --i)
            {
                a[i][n] = a[i][n] / a[i][i];
                a[i][i] = 1;
                for (j = i - 1; j >= 0; --j)
                {
                    a[j][n] -= a[j][i] * a[i][n];
                    a[j][i] = 0;
                }
            }
        }

        void gauss()
        {

            forwardSubstitution();
            reverseElimination();

        }

    public:
        GausianElimination()
        {

        }
        template<typename Matrix> const LD::StaticArray<Precision,systemSize> Solve(const Matrix & buffer, const PDP::UInteger & k)
        {
            //memset(Solutions, 0, systemSize+1);

            this->Results.Clear();


            this->n = k;
            for (PDP::Integer i = 0; i<this->n ; ++i )
            {

                for (PDP::Integer j = 0; j<(this->n+1); ++j )
                {

                    a[i][j] = (Precision)buffer[i][j];
                }
            }


            gauss();


            for (LD::UInteger i = 0; i<n; ++i )
            {
                //Solutions[i] = a[i][n];
                Results[i] = a[i][n];

            }

            return this->Results;
        }


        const LD::StaticArray<Precision,systemSize> GetResults() const
        {
            //PDP::ElementReference<PDP::StaticArray<Precision,systemSize>> refer(this->Results);

            //return PDP::OptionalReference<PDP::StaticArray<Precision,systemSize>> (refer);
            return this->Results;
        }

        /*
        const PDP::StaticArray<Precision, systemSize> & GetResults() const
        {
            return this->Results;
        }
        */
        /*
        const Precision * GetSolutions() const
        {
            return this->Solutions;
        }
         */

        const LD::Integer & GetDimension() const
        {
            return this->n;
        }

    };
}
#endif //LANDESSDEVCORE_GAUSSIANELIMINATION_H
