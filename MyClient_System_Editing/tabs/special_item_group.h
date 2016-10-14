#ifndef SPECIALITEMGROUP_H
#define SPECIALITEMGROUP_H

#include "global.h"

#include "tabs/items.h"

struct SpecialItemGroupStruct
{
    QString Name;
    uint32_t Vnum;
    QList<uint32_t> *SubItemVnum;
    QStringList *SubItemGBName;
    QStringList *SubName;
    QList<uint32_t> *SubAmount;
    QList<float> *SubPercent;
};


class SpecialItemGroup
{
public:
    SpecialItemGroup();

    static QList<SpecialItemGroupStruct> *SIGGroups;

    bool CheckType(QStringList tabs);
    void LoadGroup(QString groupPath);
    void SaveGroup(QString groupPath);
    void InsertNewEmptyItem(void);

private:
    void SearchValue();

    bool isOpen;
    SpecialItemGroupStruct *tempGr;
};

#endif // SPECIALITEMGROUP_H
