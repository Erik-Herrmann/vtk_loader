#ifndef CWORLDPATCH_H
#define CWORLDPATCH_H

#include <vector>
#include "globaldefines.h"
#include "datastruct.h"

class cWorldPatch
{
public:
    cWorldPatch();
    cWorldPatch(WorldCoord begin, WorldCoord end);
    cWorldPatch(WorldCoord begin, WorldCoord end, int stacks, int slices);
    ~cWorldPatch();

    bool isInPatch(WorldCoord coord);
    void setAlphaValue(float val);
    void updateAlpha(unsigned char detection);

    void setHistory(unsigned int count);

    void drawPatch();

private:
    WorldCoord m_PatchBegin;
    WorldCoord m_PatchEnd;

    float m_AlphaValue;
    //float m_AlphaReduction;

    unsigned int m_HistoryCount;

    std::vector<Point> m_PatchPoints;
    std::vector<Quad> m_PatchQuads;

    void createPatch(int stacks, int slices);
};

#endif // CWORLDPATCH_H
