#ifndef MANAGER_H
#define MANAGER_H

#include "global.h"

struct GroupStruct
{
    QString Name;
    uint32_t Vnum;
    QString Leader;
    uint32_t LeaderVnum;
    QStringList SubMob;
    QList<uint32_t> SubMobVnum;
};

class GroupInfo
{
public:
    GroupInfo();

    static QList<GroupStruct> *Groups;

    bool CheckType(QStringList tabs);
    void OpenGroup(char *groupPath);

private:
    bool isOpen;
    GroupStruct *tempGr;
};

#endif // MANAGER_H
