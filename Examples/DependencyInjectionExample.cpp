//
// Created by phoenixflower on 10/23/21.
//
#include "Patterns/DependencyInjection.hpp"
#include "Algorithms/Exponential.hpp"
#include "Services/Logging.hpp"
#include "Patterns/Configuration.hpp"
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

        class DITestWithConfig: public LD::DependencyInjection<DITestWithConfig(LD::Configuration&,int,LD::BasicLogger&)>
        {
        private:
            int & mTest;
        public:
            DITestWithConfig(LD::Configuration & config,int & test, LD::BasicLogger & logger):mTest{test}
            {
                LD::Optional<LD::Integer> possibleInteger = config("key",LD::Type<LD::Integer>{});
                LD::Optional<LD::StringView> host = config("host",LD::Type<LD::StringView>{});
                if (host)
                {
                    logger << "Found host value: " << *host << "\n";
                }
                if (possibleInteger)
                {
                    logger << "Found Value From Config: " << *possibleInteger << "\n";
                }
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

            auto j2 = R"(
                {
                "happy": true,
                "pi": 3.141,
                "key": 12.97,
                "object" : {"key": 76 , "host": "rawr"}
                }
            )"_json;
            nlohmann::json mConfiguration{j2};

            LD::JsonConfiguration configuration(LD::Mem::GetNewDeleteResourceReference(),mConfiguration);


            //LD::Configuration & config = configuration;
            LD::BasicServiceExecutor<void(LD::AbstractLogger&)> coreServices{configuration,passedInLogger};

            auto logger = coreServices.Make<LD::BasicLogger>();
            LD::BasicServiceExecutor<void(int,float,LD::BasicLogger&)> executionContext{configuration,int{99},float{},logger};

            DITest test =  executionContext.Make<DITest>();
            auto uniqueDITest = executionContext.MakeUnique<DITest>();

            auto uniqueDITest1 = executionContext.Make<DITestWithConfig>("object");


            logger << "UniqueIDTest: " << uniqueDITest->CalculateMeme() << "\n";
            logger << test.CalculateMeme() << "\n";
        }
    }
}