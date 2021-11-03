//
// Created by phoenixflower on 9/26/21.
//

#ifndef LANDESSDEVCORE_PROPERTYTYPES_HPP
#define LANDESSDEVCORE_PROPERTYTYPES_HPP
//
//  PropertyTypes.h
//  Reflection
//
//  Created by James Landess on 12/6/13.
//  Copyright (c) 2013 LandessDev. All rights reserved.
//

namespace LD
{
    namespace RT
    {
        enum ePropertyType
        {
            eptBOOL,
            eptDWORD,
            eptINT,
            eptFLOAT,
            eptSTRING,
            eptPTR,
            eptDOUBLE,
            eptLONG,
            eptCHAR,
            eptLONGINTEGER,
            eptMAX_PROPERTY_TYPES [[maybe_unused]]
        };

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CPropertyType
//
// This templatized class will associate compile-time types with unique enum members.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

        template <class T> class PropertyType
        {

        public :

            // Returns type ID associated with the templatized type.
            static	ePropertyType	GetTypeID()
            {
                return eptPTR;
            }

        private:

            //static			ePropertyType    ms_TypeID;

        };



        template <> class PropertyType<bool>
        {

        public :

            // Returns type ID associated with the templatized type.
            static	ePropertyType	GetTypeID()
            {
                return eptBOOL;
            }

        private:

            //static			ePropertyType    ms_TypeID;

        };

        template <> class PropertyType<unsigned long>
        {

        public :

            // Returns type ID associated with the templatized type.
            static	ePropertyType	GetTypeID()
            {
                return eptDWORD;
            }

        private:

            //static			ePropertyType    ms_TypeID;

        };

        template <> class PropertyType<int>
        {

        public :

            // Returns type ID associated with the templatized type.
            static	ePropertyType	GetTypeID()
            {
                return eptINT;
            }

        private:

            // static			ePropertyType    ms_TypeID;

        };

        template <> class PropertyType<float>
        {

        public :

            // Returns type ID associated with the templatized type.
            static	ePropertyType	GetTypeID()
            {
                return eptFLOAT;
            }

        private:

            //static			ePropertyType    ms_TypeID;

        };

        template <> class PropertyType<char*>
        {

        public :

            // Returns type ID associated with the templatized type.
            static	ePropertyType	GetTypeID()
            {
                return eptSTRING;
            }

        private:

            //static			ePropertyType    ms_TypeID;

        };


        template<> class PropertyType<double>
        {
        public:

            static ePropertyType GetTypeID()
            {
                return eptDOUBLE;
            }
        };

        template<> class PropertyType<long>
        {
        public:

            static ePropertyType GetTypeID()
            {
                return eptLONG;
            }
        };

        template <> class PropertyType<char>
        {
        public:
            static ePropertyType GetTypeID()
            {
                return eptCHAR;
            }
        };

        template<> class PropertyType<long long int>
        {
        public:
            static ePropertyType GetTypeID()
            {
                return eptLONGINTEGER;
            }
        };
    }
}


#endif //LANDESSDEVCORE_PROPERTYTYPES_HPP
