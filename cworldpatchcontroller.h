#ifndef CWORLDPATCHCONTROLLER_H
#define CWORLDPATCHCONTROLLER_H

#include <QObject>
#include "datastruct.h"
#include "cworldpatch.h"
#include "cfiledata.h"

class cWorldPatchController : public QObject
{

    Q_OBJECT
public:
    explicit cWorldPatchController(QObject *parent = 0);
    ~cWorldPatchController();

    void createPatchs(unsigned int longitudePatches, unsigned int latitudePatches);
    cWorldPatch* getPatch(WorldCoord &coord);
    unsigned int getNumberOfPatches();

    unsigned int setFileData(cFileData *fileData);
    bool isFileDataSet();

    void drawPatchs();

private:
    unsigned int m_NumLongitudePatches;
    unsigned int m_NumLatitudePatches;
    float m_LongiStepFac;
    float m_LatiStepFac;
    std::vector<cWorldPatch*> m_Patches;
    std::vector<unsigned int> m_TimeSliderLookup;

    cFileData *m_FileDataPtr;

    unsigned int m_HistoryCount;

    unsigned int getPatchIndexFromWorldCoord(WorldCoord &coord);
    int getDataIndexFromTime(double &time);
    void clearPatchAlpha();


public slots:
    void updatePatches(std::vector<unsigned int> *dataindices, unsigned int timeStep);
};

#endif // CWORLDPATCHCONTROLLER_H
