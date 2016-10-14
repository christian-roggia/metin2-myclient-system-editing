#ifndef MOBDROPITEM_H
#define MOBDROPITEM_H

#include "global.h"

#include "tabs/items.h"

struct MobDropItemStruct
{
    QString Name;
    QString Type;
    uint32_t Mob;
    uint32_t KillDrop;
    uint32_t LevelLimit;
    QList<uint32_t> *SubItemVnum;
    QStringList *SubItemGBName;
    QStringList *SubName;
    QList<uint32_t> *SubAmount;
    QList<float> *SubPercent;
    QList<float> *SubEvent;
};


class MobDropItem
{
public:
    MobDropItem();

    static QList<MobDropItemStruct> *MDIGroups;

    bool CheckType(QStringList tabs);
    void LoadGroup(QString groupPath);
    void SaveGroup(QString groupPath);
    void InsertNewEmptyItem(void);

private:
    void SearchValue();

    bool isOpen;
    MobDropItemStruct *tempGr;
};

#endif // MOBDROPITEM_H
