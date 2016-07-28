// Mina Jafari
// 12-08-2015

#include "bcc111.h"
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

double bcc111::m_DELTA_Z = 2.5;

bool bcc111::setAtoms(const std::vector<std::string> &xyzFile)
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

    mDistance = std::sqrt(3)/2 * mDeltaX; // height of the equilateral triangle

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
    else { std::cout << "ERROR: not ASE generated input file"; }

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

    return (isSet);
}

bool bcc111::isFound(const double &inX, const double &inY, const double &inZ) // check if the calculated * atoms exist
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

void bcc111::findAtop()
{
    std::string val1 = std::to_string(mStarMinusOneAtom[0]);
    std::string val2 = std::to_string(mStarMinusOneAtom[1]);
    std::string val3 = std::to_string(mStarMinusOneAtom[2] + m_DELTA_Z);
    std::string newElem = "C          " + val1 + "       " + val2 + "      " + val3;

    std::ofstream ofs;
    ofs.open ("bcc111-atop.xyz", std::ofstream::out);
    ofs << std::to_string( atoi(mVector[0].c_str()) + 1 ) << "\n";
    ofs << "\n";
    for (auto i = mVector.begin()+2; i != mVector.end(); ++i)
    {
        ofs << *i << "\n";
    }
    ofs << newElem;
    ofs.close();
} //findAtop

void bcc111::findShallowBridge()
{
    double brgX = mNthAtom[0] - (mNthAtom[0]-mStarAtom[0])/2;
    double brgY = mNthAtom[1] - mDistance/6;
    double brgZ = mNthAtom[2] + m_DELTA_Z;

    std::string val1 = std::to_string(brgX);
    std::string val2 = std::to_string(brgY);
    std::string val3 = std::to_string(brgZ);
    std::string newElem = "C          " + val1 + "       " + val2 + "      " + val3;

    std::ofstream ofs;
    ofs.open ("bcc111-shallowBrg.xyz", std::ofstream::out);
    ofs << std::to_string( atoi(mVector[0].c_str()) + 1 ) << "\n";
    ofs << "\n";
    for (auto i = mVector.begin()+2; i != mVector.end(); ++i)
    {   
        ofs << *i << "\n";
    }
    ofs << newElem;
    ofs.close();
} //findShallowBridge

void bcc111::findDeepBridge()
{
    double brgX = mStarAtom[0] - (mStarAtom[0]-mNthMinusOneAtom[0])/2;
    double brgY = mNthMinusOneAtom[1] - mDistance/2;
    double brgZ = mNthAtom[2] + m_DELTA_Z;

    std::string val1 = std::to_string(brgX);
    std::string val2 = std::to_string(brgY);
    std::string val3 = std::to_string(brgZ);
    std::string newElem = "C          " + val1 + "       " + val2 + "      " + val3;

    std::ofstream ofs;
    ofs.open ("bcc111-deepBrg.xyz", std::ofstream::out);
    ofs << std::to_string( atoi(mVector[0].c_str()) + 1 ) << "\n";
    ofs << "\n";
    for (auto i = mVector.begin()+2; i != mVector.end(); ++i)
    {   
        ofs << *i << "\n";
    }
    ofs << newElem;
    ofs.close();
} //findDeepBridge

void bcc111::findFcc()
{
    double fccX = mNthAtom[0] - (mDeltaX/2); // = mStarAtom[0]
    double fccY = mNthAtom[1] - (mDistance/3); // equilateral triangle
    double fccZ = mNthAtom[2] + m_DELTA_Z;

    if (isFound(fccX, fccY, mSecLayerZ))
    {
        std::string val1 = std::to_string(fccX);
        std::string val2 = std::to_string(fccY);
        std::string val3 = std::to_string(fccZ);
        std::string newElem = "C          " + val1 + "       " + val2 + "      " + val3;

        std::ofstream ofs;
        ofs.open ("bcc111-fcc.xyz", std::ofstream::out);
        ofs << std::to_string( atoi(mVector[0].c_str()) + 1 ) << "\n";
        ofs << "\n";
        for (auto i = mVector.begin()+2; i != mVector.end(); ++i)
        {   
            ofs << *i << "\n";
        }
        ofs << newElem;
        ofs.close();
    }
    else
    {
        std::cout << "ERROR: not able to find the FCC site" << std::endl;
    }
} //findFcc

void bcc111::findHcp()
{
    double hcpX = mStarAtom[0] - (mDeltaX/2); // = mNthMinusOneAtom[0]
    double hcpY = mNthMinusOneAtom[1] - (2*mDistance/3); // equilateral triangle, third layer
    double hcpZ = mNthAtom[2] + m_DELTA_Z;

    if ( (!(isFound(hcpX, hcpY, mThirLayerZ)) && (mThirLayerZ == 0.0)) || (isFound(hcpX, hcpY, mThirLayerZ)) )
//    if (isFound(hcpX, hcpY, mThirLayerZ)) // at least 3 layers are required
    {
        std::string val1 = std::to_string(hcpX);
        std::string val2 = std::to_string(hcpY);
        std::string val3 = std::to_string(hcpZ);
        std::string newElem = "C          " + val1 + "       " + val2 + "      " + val3;

        std::ofstream ofs;
        ofs.open ("bcc111-hcp.xyz", std::ofstream::out);
        ofs << std::to_string( atoi(mVector[0].c_str()) + 1 ) << "\n";
        ofs << "\n";
        for (auto i = mVector.begin()+2; i != mVector.end(); ++i)
        {   
            ofs << *i << "\n";
        }
        ofs << newElem;
        ofs.close();
    }
/*    else if (!(isFound(hcpX, hcpY, mThirLayerZ)) && (mThirLayerZ == 0.0)) // slab with only two layers
    {
        std::string val1 = std::to_string(hcpX);
        std::string val2 = std::to_string(hcpY);
        std::string val3 = std::to_string(hcpZ);
        std::string newElem = "C          " + val1 + "       " + val2 + "      " + val3;

        std::ofstream ofs;
        ofs.open ("bcc111-hcp.xyz", std::ofstream::out);
        ofs << std::to_string( atoi(mVector[0].c_str()) + 1 ) << "\n";
        ofs << "\n";
        for (auto i = mVector.begin()+2; i != mVector.end(); ++i)
        {   
            ofs << *i << "\n";
        }
        ofs << newElem;
        ofs.close();
    }*/
    else
    {
        std::cout << "ERROR: not able to find the HCP site" << std::endl;
    }
} //findHcp
