//
// Created by phoenixflower on 9/28/21.
//

#include "RunTimeReflectionExample.hpp"
#include "RuntimeReflection/BaseObject.hpp"
#include "RuntimeReflection/AbstractProperty.hpp"
#include "RuntimeReflection/ExampleRuntimeReflection.hpp"
#include "Memory/shared_ptr.hpp"
namespace LD
{
    namespace Example
    {
        void Print(LD::RT::BaseObject * object, const int spacing = 0)
        {

            std::vector<LD::RT::AbstractProperty*> properties;

            object->GetRTTI()->EnumProperties(properties);

            for (int n = 0; n<properties.size(); n++)
            {
                std::string offset = "";
                for (int k = 0; k<spacing; k++)
                {
                    offset+=" ";
                }

                std::cout << offset << properties[n]->GetName() << std::endl;
                if (properties[n]->GetType() == LD::RT::eptPTR)
                {
                    LD::RT::TypedProperty<LD::RT::BaseObject*>* pTypedProperty = (LD::RT::TypedProperty<LD::RT::BaseObject*>*)properties[n];


                    LD::RT::BaseObject * node = pTypedProperty->GetValue(object);
                    Print(node,spacing+2);
                }
            }
        }

        void RunTimeReflectionExample() noexcept
        {
            LD::SharedPointer<CWeapon> weapon = CWeapon::CreateShared();

            Print(weapon.get());
        }
    }
}