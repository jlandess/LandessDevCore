//
// Created by phoenixflower on 9/26/21.
//

#ifndef LANDESSDEVCORE_BUBBLETARGETER_HPP
#define LANDESSDEVCORE_BUBBLETARGETER_HPP
#include "Algorithms/Exponential.hpp"
#include "Algorithms/Periodic.hpp"
//---------------------------------------------------------------------------------------

//float fmax(float a, float b) { return a > b ? a : b; }

//---------------------------------------------------------------------------------------

class BubbleTargeter
{
public:
    // by default, a sphere with radius 1:
    BubbleTargeter()
    {
        mA = mB = mP = 0.0f;
        mC = mD = mQ = 1.0f;
        mM = mN = 0.0f;
    }

    // define bubble from characteristic dimensions:
    void Set(float zFar, float xFar, float yFar, float zxyFar, float zRear)
    {
        mP = zFar - zRear;
        mQ = zFar * zRear;

        float k1 = mP - 2 * zxyFar;
        float k0 = mQ + zxyFar * zxyFar;
        float xFarSqr = xFar * xFar;
        float yFarSqr = yFar * yFar;

        mA = k1 / xFarSqr;
        mB = k1 / yFarSqr;
        mC = k0 / xFarSqr;
        mD = k0 / yFarSqr;

        mM = mA * mQ - mC * mP;
        mN = mB * mQ - mD * mP;
    }

    // evaluate potential at P(x,y,z) in observer space:
    float Eval(float x, float y, float z) const
    {
        const float eps = 1e-9f;
        float R = LD::Max(mA * z + mC, 0);
        float S = LD::Max(mB * z + mD, 0);
        float D = LD::Max(mP * z + mQ, eps);
        return (z*z + R*x*x + S*y*y)/D;
    }

    // evaluate gradient at P(x,y,z) in observer space:
    void Gradient(float* pG, float x, float y, float z) const
    {
        const float eps = 1e-9f;
        float R = LD::Max(mA * z + mC, 0);
        float S = LD::Max(mB * z + mD, 0);
        float D = LD::Max(mP * z + mQ, eps);
        float invD = 1.0f/D;
        pG[0] = 2*x*R * invD;
        pG[1] = 2*y*S * invD;
        pG[2] = (mM*x*x + mN*y*y + (mP*z + 2*mQ)*z) * invD*invD;
    }

    // get a point on the isosurface v=vc at a given z coordinate
    // z must be in valid range (use IsoMinMaxZ() to determine valid z range)
    // phi is an angle in radians: -pi <= phi < pi
    // returns false if z is out of valid range (but still computes a point)
    bool IsoPoint(float* pP, float z, float phi, float vc = 1.0f) const
    {
        float R = mA * z + mC;
        float S = mB * z + mD;
        float D = mP * z + mQ;

        // we know z already:
        pP[2] = z;

        // is z in range?
        float k = D*vc - z*z;
        if (k < 0.0f)
        { // no - just zero the radius:
            pP[0] = pP[1] = 0.0f;
            return false;
        }

        // yes - compute x,y ellipse for this z:
        float rx = LD::Sqrt(k/R);
        float ry = LD::Sqrt(k/S);
        pP[0] = rx * LD::Sin(phi);
        pP[1] = ry * LD::Cos(phi);
        return true;
    }

    // get minimum and maximum z for a given potential vc:
    // (for vc=1 this is -zRear and zFar)
    void IsoMinMaxZ(float& minZ, float& maxZ, float vc = 1.0f) const
    { // these are simply the two solutions for the equation D*vc - z*z = 0
        float s = vc*mP;
        float t = vc*mQ;
        maxZ = 0.5f * (s + LD::Sqrt(s*s + 4*t));
        minZ = -t / maxZ;
    }

protected:
    float mA, mB, mC, mD;   // numerator coefficients
    float mP, mQ;           // denominator coefficients
    float mM, mN;           // handy shortcuts for gradient computation
};

//---------------------------------------------------------------------------------------
#endif //LANDESSDEVCORE_BUBBLETARGETER_HPP
