#include "group.h"

QList<GroupStruct> *GroupInfo::Groups;

GroupInfo::GroupInfo()
{
    Groups = new QList<GroupStruct>;
}

void GroupInfo::OpenGroup(char *groupPath)
{
    uint32_t size;
    char *data;
    QStringList lines, tabs;

    size = IOReadFile(groupPath, data);
    lines = QString(data).split(0x0A);

    for(int i = 0; i < lines.length(); i++)
    {
        tabs = lines.at(i).split(0x09);

        //Syslog(QString("GROUP INFO :: LOAD GROUP :: Line %0, tab found = %1, line = %2").arg(QString::number(i), QString::number(lines.at(i).split(0x09).count()), lines.at(i)));

        if(!CheckType(tabs))
        {
            Syserr(QString("GROUP INFO :: LOAD GROUP :: Strange line [i == %0, tabs.length() == %1, lines.at(i) = %2]").arg(QString::number(i), QString::number(tabs.length()), lines.at(i)));
        }
    }
}

bool GroupInfo::CheckType(QStringList tabs)
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");

    if(tabs.at(0) == "{")
    {
        if(!isOpen)
        {
            isOpen = true;
        }
        return true;
    }
    else if(tabs.at(0) == "}")
    {
        isOpen = false;
        Groups->append(*tempGr);
        delete tempGr;
        return true;
    }

    if(tabs.length() > 1)
    {
        if(tabs.at(0).toUpper() == "GROUP")
        {
            tempGr = new GroupStruct();
            //tempGr->Name = codec->toUnicode(tabs.at(1).toAscii().data());
            tempGr->Name = tabs.at(1);
            return true;
        }
    }

    if(tabs.length() > 2)
    {
        if(tabs.at(1).toUpper() == "VNUM")
        {
            tempGr->Vnum = tabs.at(2).toInt();
            return true;
        }
    }

    if(tabs.length() > 3)
    {
        if(tabs.at(1).toInt() != 0)
        {
            tempGr->SubMobVnum.append(tabs.at(3).toInt());
            //tempGr->SubMob.append(codec->toUnicode(tabs.at(2).toAscii().data()));
            tempGr->SubMob.append(tabs.at(2));
            return true;
        }
        else if(tabs.at(1).toUpper() == "LEADER")
        {
            tempGr->LeaderVnum = tabs.at(3).toInt();
            //tempGr->Leader = codec->toUnicode(tabs.at(2).toAscii().data());
            tempGr->Leader = tabs.at(2);
            return true;
        }
    }

    return false;
}
