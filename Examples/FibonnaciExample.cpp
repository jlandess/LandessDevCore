//
// Created by phoenixflower on 3/23/21.
//
#include "Definitions/Integer.hpp"
#include <map>
#include "Core/RequestResponse.hpp"
#include "IO/UnqliteDatabaseBackend.h"
#include "IO/Database1.h"
#include <iostream>
namespace LD
{
    namespace Example
    {
        LD::UInteger Fib(LD::UInteger n , std::map<LD::UInteger,LD::UInteger> & map) noexcept
        {
            auto it = map.find(n);
            if (it != map.end())
            {
                return (*it).second;
            }
            if( n <= 2) return 1;
            map[n] =  Fib(n-1,map) + Fib(n-2,map);
            return map[n];
        }
        LD::UInteger NaiveFib(LD::UInteger n)
        {
            if( n <= 2) return 1;
            return NaiveFib(n-1) + NaiveFib(n-2);
        }
        LD::UInteger Fib(LD::UInteger n , LD::UnQliteBackend<char> & unqliteBackingStore) noexcept
        {
            LD::RequestResponse<LD::UInteger()> response = LD::Fetch(unqliteBackingStore,LD::ToImmutableString(n),LD::Type<LD::UInteger>{});

            auto onResponse = [](LD::UInteger number) noexcept
            {
                return LD::Optional<LD::UInteger>{number};
            };

            auto onError = [](LD::TransactionError error) noexcept
            {
                return LD::Optional<LD::UInteger>{};
            };

            LD::Optional<LD::UInteger> possibleFib = LD::InvokeVisitation(LD::Overload{onResponse,onError},response);
            if (possibleFib)
            {
                return (*possibleFib);
            }
            if( n <= 2) return 1;


            LD::Insert(unqliteBackingStore,LD::ToImmutableString(n),Fib(n-1,unqliteBackingStore) + Fib(n-2,unqliteBackingStore));

            LD::RequestResponse<LD::UInteger()> response1 = LD::Fetch(unqliteBackingStore,LD::ToImmutableString(n),LD::Type<LD::UInteger>{});

            auto onResponse1 = [](LD::UInteger number) noexcept -> LD::UInteger
            {
                return number;
            };

            auto onError1 = [](LD::TransactionError) noexcept -> LD::UInteger
            {
                return 1;
            };


            LD::UInteger a = Fib(n-1,unqliteBackingStore);
            LD::UInteger b = Fib(n-2,unqliteBackingStore);
            std::cout << "Rawr: " << a + b << std::endl;
            return LD::InvokeVisitation(LD::Overload{onResponse1,onError1},response1);

        }
        extern void Fibbonnaci()
        {
            LD::UnQliteBackend<char> unqliteBackingStore{"databasefib.db",LD::OpenAndCreateIfNotExists{}};
            std::cout << Fib(1000,unqliteBackingStore) << std::endl;
            std::map<LD::UInteger,LD::UInteger> fibMap;
            std::cout << Fib(60,fibMap) << std::endl;
        }
    }
}