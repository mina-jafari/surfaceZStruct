// Mina Jafari
// 12-04-2015

// atom assignment: first layer
//
//  ---(N-1)-----(N)
//       |        |
//       |        |
//  ---(N-1)*----(N)*
//       |        |

#ifndef _FCC110_H_
#define _FCC110_H_
#include <vector>
#include <string>

class fcc110
{
    private:
        static double m_DELTA_Z;
        std::vector<std::string> mVector;
        double mNthAtom[3];
        double mNthMinusOneAtom [3];
        double mStarAtom [3];
        double mStarMinusOneAtom [3];
        double mDistance;
        double mDeltaX;
        double mDeltaY;

    public:
        bool setAtoms(const std::vector<std::string> &xyzFile);
        bool isFound(const double &inX, const double &inY, const double &inZ);
        void findHollow(const unsigned int offsetX, const unsigned int offsetY);
        void findAtop(const unsigned int offsetX, const unsigned int offsetY);
        void findLongBridge(const unsigned int offsetX, const unsigned int offsetY);
        void findShortBridge(const unsigned int offsetX, const unsigned int offsetY);
};
#endif
