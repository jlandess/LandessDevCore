//
// Created by phoenixflower on 10/23/21.
//
#include "Patterns/DependencyInjection.hpp"
#include "Algorithms/Exponential.hpp"
#include "Services/Logging.hpp"
namespace LD
{
    namespace Example
    {
    class DITest: public LD::DependencyInjection<DITest(int,LD::BasicLogger&)>
        {
        private:
            int & mTest;
        public:
            DITest(int & test, LD::BasicLogger & logger):mTest{test}
            {
                logger << "Test Number: " << this->mTest << "\n";
                //std::cout << "Test Number: " <<  mTest << "\n";
            }

            float CalculateMeme()
            {
                return LD::Sqrt(this->mTest);
            }
        };
        extern void DependencyInjectionExample(LD::AbstractLogger & passedInLogger) noexcept
        {

            LD::BasicServiceExecutor<LD::AbstractLogger&> coreServices{passedInLogger};

            auto logger = coreServices.Make<LD::BasicLogger>();
            LD::BasicServiceExecutor<int,float,LD::BasicLogger&> executionContext{int{99},float{},logger};

            DITest test =  executionContext.Make<DITest>();
            auto uniqueDITest = executionContext.MakeUnique<DITest>();


            logger << "UniqueIDTest: " << uniqueDITest->CalculateMeme() << "\n";
            logger << test.CalculateMeme() << "\n";
        }
    }
}