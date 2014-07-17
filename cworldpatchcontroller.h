#ifndef CWORLDPATCHCONTROLLER_H
#define CWORLDPATCHCONTROLLER_H

#include "datastruct.h"
#include "cworldpatch.h"

class cWorldPatchController
{
public:
    cWorldPatchController();

    void createPatchs(unsigned int longitudePatches, unsigned int latitudePatches);
    cWorldPatch* getPatch(WorldCoord &coord);
    unsigned int getNumberOfPatchs();

    void drawPatchs();

private:
    unsigned int m_NumLongitudePatches;
    unsigned int m_NumLatitudePatches;
    float m_LongiStepFac;
    float m_LatiStepFac;
    std::vector<cWorldPatch*> m_Patches;

    unsigned int getPatchIndexFromWorldCoord(WorldCoord &coord);
};

#endif // CWORLDPATCHCONTROLLER_H
