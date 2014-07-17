#include "cworldpatchcontroller.h"

cWorldPatchController::cWorldPatchController()
{
}

void cWorldPatchController::createPatchs(unsigned int longitudePatches, unsigned int latitudePatches){
    // set the Number of Patches
    m_NumLongitudePatches = longitudePatches;
    m_NumLatitudePatches = latitudePatches;

    // reserve memory for pointers
    m_Patches.reserve(longitudePatches*latitudePatches);

    // create cWorldPatchs
    float longiStep = 360.0f/longitudePatches;
    float latiStep = 180.0f/latitudePatches;

    // Step-factor to later calculate Patch-Indices
    m_LongiStepFac = 1.0f/longiStep;
    m_LatiStepFac = 1.0f/latiStep;

    for (float latiPos=-90.0f; latiPos < 90.0f; latiPos+=latiStep){
        for (float longiPos=-180.0f; longiPos < 180.0f; longiPos+=longiStep){
            WorldCoord begin(longiPos, latiPos);
            WorldCoord end(longiPos+longiStep, latiPos+latiStep);
            cWorldPatch *newPatch = new cWorldPatch(begin, end, 2, 2);
            m_Patches.push_back(newPatch);
        }
    }
}

cWorldPatch* cWorldPatchController::getPatch(WorldCoord &coord){
    return m_Patches.at(getPatchIndexFromWorldCoord(coord));
}

unsigned int cWorldPatchController::getNumberOfPatchs(){
    return m_Patches.size();
}

unsigned int cWorldPatchController::getPatchIndexFromWorldCoord(WorldCoord &coord){
    // get Integer-Multiple = index
    unsigned int longiIdx = m_LongiStepFac*(coord.longitude+180.0f);
    unsigned int latiIdx = m_LatiStepFac*(coord.latitude+90.0f);

    return latiIdx*m_NumLongitudePatches + longiIdx;
}

void cWorldPatchController::drawPatchs(){
    for (cWorldPatch* patch : m_Patches){
        patch->drawPatch();
    }
}
