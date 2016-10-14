#ifndef MDATR_H
#define MDATR_H

#include "global.h"

class CMADTR
{
public:
    uint32_t collType;
    char name[32];
    float X, Y, Z;
    float unk1, unk2, unk3;
    float f1, f2, f3, f4;
};

class MDATR
{
public:
    MDATR();
    void LoadMATR(QString MDATRPath);
    bool CheckHeader(char *header);

    int GetCollisions()     {return this->collisions;}

    static QList<CMADTR> *LMDATR;

private:
    uint32_t collisions, heights;
    CMADTR *tempCMADTR;
};

#endif // MDATR_H
