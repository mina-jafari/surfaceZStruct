// Mina Jafari
// 12-08-2015

#include "bcc110.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h> 
#include <vector>
#include <stdio.h>
#include <cmath>
#include <iomanip>
#include <cmath>

double bcc110::m_DELTA_Z = 2.5;

bool bcc110::setAtoms(const std::vector<std::string> &xyzFile)
{
    bool isSet = true;
    mVector = xyzFile;

    // setting mNthAtom
    std::string unwanted;
    std::istringstream iss1(xyzFile[xyzFile.size()-1]);
    iss1 >> unwanted >> mNthAtom[0] >> mNthAtom[1] >> mNthAtom[2] >> unwanted;

    // setting mNthMinusOneAtom
    std::istringstream iss2(xyzFile[xyzFile.size()-2]);
    iss2 >> unwanted >> mNthMinusOneAtom[0] >> mNthMinusOneAtom[1] >> mNthMinusOneAtom[2] >> unwanted;

    mDeltaX = mNthAtom[0] - mNthMinusOneAtom[0];
    mDeltaY = mNthAtom[1] - mNthMinusOneAtom[1];

    mDistance = std::sqrt(2)/2 * mDeltaX; 

    // setting mStarAtom & mStarMinusOneAtom
    // n-1 to n vector intersects w/ Y axis
    if (std::abs(mDeltaX) > std::abs(mDeltaY))
    {   
        mStarAtom[0] = mNthAtom[0] - mDeltaX/2; // x component
        mStarAtom[1] = mNthAtom[1] - mDistance; // y component
        mStarAtom[2] = mNthAtom[2]; // z component
        if (!(isFound(mStarAtom[0], mStarAtom[1], mStarAtom[2])))
        {
            std::cout << "ERROR: setting StarAtom failed\n";
            isSet = false;
        }
        mStarMinusOneAtom[0] = mNthMinusOneAtom[0] - mDeltaX/2; // x component
        mStarMinusOneAtom[1] = mNthMinusOneAtom[1] - mDistance; // y component
        mStarMinusOneAtom[2] = mNthMinusOneAtom[2]; // z component
        if (!(isFound(mStarMinusOneAtom[0], mStarMinusOneAtom[1], mStarMinusOneAtom[2])))
        {
            std::cout << "ERROR: setting StarMinusOneAtom failed\n";
            isSet = false;
        }
    }
    else { std::cout << "ERROR: not an ASE generated input file"; }
/*
    const double tolerance = 0.20;
    for (int j=(mVector.size()-1); j>1; j--)
    {
        double temp [3];
        std::string unwanted;
        std::istringstream iss(mVector[j]);
        iss >> unwanted >> temp[0] >> temp[1] >> temp[2] >> unwanted;
        if (temp[2] < (mNthAtom[2]-tolerance))
        {
            mSecLayerZ = temp[2];
            break;
        }
    }
    for (int j=(mVector.size()-1); j>1; j--)
    {
        double temp [3];
        std::string unwanted;
        std::istringstream iss(mVector[j]);
        iss >> unwanted >> temp[0] >> temp[1] >> temp[2] >> unwanted;
        if (temp[2] < (mSecLayerZ-tolerance))
        {
            mThirLayerZ = temp[2];
            break;
        }
    }
*/

    return (isSet);
}

bool bcc110::isFound(const double &inX, const double &inY, const double &inZ) // check if the calculated * atoms exist
{
    const double tolerance = 0.15;
    for (int j=(mVector.size()-1); j>1; j--)
    {
        double temp [3];
        std::string unwanted;
        std::istringstream iss(mVector[j]);
        iss >> unwanted >> temp[0] >> temp[1] >> temp[2] >> unwanted;
        if (  ((temp[0] <= (inX + tolerance)) && (temp[0] >= (inX - tolerance)) ) &&
              ((temp[1] <= (inY + tolerance)) && (temp[1] >= (inY - tolerance)) ) &&   
              ((temp[2] <= (inZ + tolerance)) && (temp[2] >= (inZ - tolerance)) )  )   
        {
            return (true);
        }
    }
    return (false);
}

void bcc110::findHollow()
{
    double hollowX = mNthAtom[0] - (mDeltaX/2);
    double hollowY = mNthAtom[1] - (mDistance/2);
    double hollowZ = mNthAtom[2] + m_DELTA_Z;

    std::string val1 = std::to_string(hollowX);
    std::string val2 = std::to_string(hollowY);
    std::string val3 = std::to_string(hollowZ);
    std::string newElem = "C          " + val1 + "       " + val2 + "      " + val3;

    std::ofstream ofs;
    ofs.open ("bcc110-hollow.xyz", std::ofstream::out);
    ofs << std::to_string( atoi(mVector[0].c_str()) + 1 ) << "\n";
    ofs << "\n";
    for (auto i = mVector.begin()+2; i != mVector.end(); ++i)
    {
        ofs << *i << "\n";
    }
    ofs << newElem;
    ofs.close();
} //findHollow

void bcc110::findAtop()
{
    std::string val1 = std::to_string(mStarMinusOneAtom[0]);
    std::string val2 = std::to_string(mStarMinusOneAtom[1]);
    std::string val3 = std::to_string(mStarMinusOneAtom[2] + m_DELTA_Z);
    std::string newElem = "C          " + val1 + "       " + val2 + "      " + val3;

    std::ofstream ofs;
    ofs.open ("bcc110-atop.xyz", std::ofstream::out);
    ofs << std::to_string( atoi(mVector[0].c_str()) + 1 ) << "\n";
    ofs << "\n";
    for (auto i = mVector.begin()+2; i != mVector.end(); ++i)
    {
        ofs << *i << "\n";
    }
    ofs << newElem;
    ofs.close();
} //findAtop

void bcc110::findLongBridge()
{
    std::string val1 = std::to_string(mNthMinusOneAtom[0]);
    std::string val2 = std::to_string(mStarAtom[1]);
    std::string val3 = std::to_string(mStarAtom[2] + m_DELTA_Z);
    std::string newElem = "C          " + val1 + "       " + val2 + "      " + val3;

    std::ofstream ofs;
    ofs.open ("bcc110-Longbrg.xyz", std::ofstream::out);
    ofs << std::to_string( atoi(mVector[0].c_str()) + 1 ) << "\n";
    ofs << "\n";
    for (auto i = mVector.begin()+2; i != mVector.end(); ++i)
    {   
        ofs << *i << "\n";
    }
    ofs << newElem;
    ofs.close();
} //findLongBridge

void bcc110::findShortBridge()
{
    double SbrgX = mStarAtom[0] - ( std::abs(mStarAtom[0] - mNthMinusOneAtom[0]) / 2 );
    double SbrgY = mNthMinusOneAtom[1] - ( std::abs(mNthMinusOneAtom[1] - mStarAtom[1]) / 2 );
    double SbrgZ = mNthAtom[2] + m_DELTA_Z;

    std::string val1 = std::to_string(SbrgX);
    std::string val2 = std::to_string(SbrgY);
    std::string val3 = std::to_string(SbrgZ);
    std::string newElem = "C          " + val1 + "       " + val2 + "      " + val3;

    std::ofstream ofs;
    ofs.open ("bcc110-Shortbrg.xyz", std::ofstream::out);
    ofs << std::to_string( atoi(mVector[0].c_str()) + 1 ) << "\n";
    ofs << "\n";
    for (auto i = mVector.begin()+2; i != mVector.end(); ++i)
    {   
        ofs << *i << "\n";
    }
    ofs << newElem;
    ofs.close();
} //findShortBridge
