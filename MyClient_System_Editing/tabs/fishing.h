#ifndef FISHING_H
#define FISHING_H

#include "global.h"

struct FishingItem
{
    QString Name;
    uint32_t Vnum;
    uint32_t Dead;
    uint32_t Grilled;
    uint32_t Prob1;
    uint32_t Prob2;
    uint32_t Prob3;
    uint32_t Prob4;
    uint32_t Max_Failures;
    uint32_t Min_Failures;
    uint32_t Time1;
    uint32_t Time2;
    uint32_t Time3;
    uint32_t USED0;
    uint32_t USED1;
    uint32_t USED2;
    uint32_t USED3;
    uint32_t USED4;
    uint32_t USED5;
    uint32_t USED6;
    uint32_t USED7;
    uint32_t USED8;
    uint32_t USED9;
};

class Fishing
{
public:
    Fishing();

    bool LoadFishingTable(QString);

    static QList<FishingItem> *FishingTable;

private:
    bool IsComment(QString line);

    char *data;
    FishingItem *tempItem;
};

#endif // FISHING_H
