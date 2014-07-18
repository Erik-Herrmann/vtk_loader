#include "cworldpatchcontroller.h"
#include "datafield/cdatafieldt.h"
#include "helpers.h"
#include <QDebug>

cWorldPatchController::cWorldPatchController(QObject *parent)
    : QObject(parent),
      m_FileDataPtr(0)
{
}

cWorldPatchController::~cWorldPatchController()
{
    for (cWorldPatch* patch : m_Patches){
        delete patch;
    }
    m_Patches.clear();
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

void cWorldPatchController::setFileData(cFileData *fileData){
    m_FileDataPtr = fileData;
}

bool cWorldPatchController::isFileDataSet(){
    return m_FileDataPtr;
}

unsigned int cWorldPatchController::getPatchIndexFromWorldCoord(WorldCoord &coord){
    // get Integer-Multiple = index
    unsigned int longiIdx = m_LongiStepFac*(coord.longitude+180.0f);
    unsigned int latiIdx = m_LatiStepFac*(coord.latitude+90.0f);

    return latiIdx*m_NumLongitudePatches + longiIdx;
}


void cWorldPatchController::updatePatches(double time){
    if (!m_FileDataPtr)
        return;
    int index = getDataIndexFromTime(time);

    if (index < 0)
        return;

    clearPatchAlpha();

    cDataFieldT<vtkPoint> * df_p =
            static_cast<cDataFieldT<vtkPoint>*>(m_FileDataPtr->getDatafield("POINTS"));
    if (!df_p)
        return;
    cDataFieldT<unsigned char> * df_d =
            static_cast<cDataFieldT<unsigned char>*>(m_FileDataPtr->getDatafield("detection"));
    if (!df_d)
        return;

    for (int i=index-999; i <= index; ++i){
        if (df_d->at(i) == 4){
            vtkPoint &p = df_p->at(i);
            WorldCoord coord(p.x, p.y);
            unsigned int patchIdx =
                    getPatchIndexFromWorldCoord(coord);
            m_Patches.at(patchIdx)->setAlphaValue(1.0f);
        }
    }

}

int cWorldPatchController::getDataIndexFromTime(double &time){
    double timeMax = time+100;
    double timeMin = time-100;

    cDataFieldT<double> * df_t =
            static_cast<cDataFieldT<double>*>(m_FileDataPtr->getDatafield("time"));
    if (!df_t)
        return -1;

    std::set<int> *filter0 = new std::set<int>;
    df_t->filterData(filter0, Compair::GREATER_EQUAL, QString::number(timeMin));
    if (filter0->empty())
        qDebug() << "Filter0 is empty!\n";
    std::set<int> *filter1 = new std::set<int>;
    df_t->filterData(filter1, Compair::LESS_EQUAL, QString::number(timeMax));
    if (filter1->empty())
        qDebug() << "Filter1 is empty!\n";
    std::vector<int> *vec = new std::vector<int>(std::min(filter0->size(), filter1->size()));
    std::vector<int>::iterator it;
    it = std::set_intersection(filter0->begin(), filter0->end(), filter1->begin(), filter1->end(), vec->begin());
    vec->resize(it-vec->begin());
    delete filter0;
    delete filter1;

    std::sort(vec->begin(), vec->end(), Helpers::greaterInt);

    int ret = vec->back();
    delete vec;

    return ret;
}

void cWorldPatchController::clearPatchAlpha(){
    for (cWorldPatch *patch : m_Patches){
        patch->setAlphaValue(0.0f);
    }
}

void cWorldPatchController::drawPatchs(){
    for (cWorldPatch* patch : m_Patches){
        patch->drawPatch();
    }
}
