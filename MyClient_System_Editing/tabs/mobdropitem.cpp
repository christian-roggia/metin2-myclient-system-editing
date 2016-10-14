#include "mobdropitem.h"

QList<MobDropItemStruct> *MobDropItem::MDIGroups;

MobDropItem::MobDropItem()
{
    this->MDIGroups = new QList<MobDropItemStruct>;
}

void MobDropItem::LoadGroup(QString groupPath)
{
    uint32_t size;
    char *data;
    QStringList lines, tabs;

    size = IOReadFile(groupPath.toAscii().data(), data);
    lines = QString(data).split(QRegExp("(\r\n|\n|\r)"));

    for(int i = 0; i < lines.length(); i++)
    {
        tabs = lines.at(i).split(0x09);

        //Syslog(QString("MOB DROP ITEM :: LOAD GROUP :: Line %0, tab found = %1, line = %2").arg(QString::number(i), QString::number(lines.at(i).split(0x09).count()), lines.at(i)));

        if(!CheckType(tabs))
        {
            if(tabs.at(0).isEmpty())
                Syslog(QString("MOB DROP ITEM :: LOAD GROUP :: Empty line #"+QString::number(i)));
            else if(tabs.at(0).left(2) == "//")
                Syslog(QString("MOB DROP ITEM :: LOAD GROUP :: Comment line (//) #"+QString::number(i)));
            else if(tabs.at(0).left(1) == "#")
                Syslog(QString("MOB DROP ITEM :: LOAD GROUP :: Comment line (#) #"+QString::number(i)));
            else if(tabs.at(0).left(2) == "--")
                Syslog(QString("MOB DROP ITEM :: LOAD GROUP :: Comment line (--) #"+QString::number(i)));
            else
                Syserr(QString("MOB DROP ITEM :: LOAD GROUP :: Strange line #%0 ~ [%1 Tabs - (%2)]").arg(QString::number(i), QString::number(tabs.length()), lines.at(i)));
        }
    }
}

bool MobDropItem::CheckType(QStringList tabs)
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
        if(this->tempGr)
        {
            MDIGroups->append(*tempGr);
            delete tempGr;
        }
        return true;
    }

    if(tabs.length() > 1)
    {
        if(tabs.at(0).toUpper() == "GROUP")
        {
            tempGr = new MobDropItemStruct;
            tempGr->SubItemVnum = new QList<uint32_t>;
            tempGr->SubItemGBName = new QStringList();
            tempGr->SubAmount = new QList<uint32_t>;
            tempGr->SubEvent = new QList<float>;
            tempGr->SubPercent = new QList<float>;
            tempGr->SubName = new QStringList();
            //tempGr->Name = codec->toUnicode(tabs.at(1).toAscii().data());
            tempGr->Name = tabs.at(1);
            return true;
        }
    }

    if(tabs.length() > 2)
    {
        if(tabs.at(1).toUpper() == "TYPE")
        {
            tempGr->Type = tabs.at(2);

            if(tempGr->Type.toUpper() == "KILL")
                tempGr->KillDrop = 0;
            else if(tempGr->Type.toUpper() == "LIMIT")
                tempGr->LevelLimit = 0;

            return true;
        }
        else if(tabs.at(1).toUpper() == "MOB")
        {
            tempGr->Mob = tabs.at(2).toInt();
            return true;
        }
        else if(tabs.at(1).toUpper() == "KILL_DROP")
        {
            tempGr->KillDrop = tabs.at(2).toInt();
            return true;
        }
        else if(tabs.at(1).toUpper() == "LEVEL_LIMIT")
        {
            tempGr->LevelLimit = tabs.at(2).toInt();
            return true;
        }
    }

    if(tabs.length() > 4)
    {
        if(tabs.at(1).toInt() != 0)
        {
            if(tabs.at(2).toInt() != 0)
            {
                tempGr->SubItemVnum->append(tabs.at(2).toInt());
                tempGr->SubItemGBName->append("");
                tempGr->SubName->append("");
                SearchValue();
            }
            else
            {
                tempGr->SubItemVnum->append(0);
                //tempGr->SubItemGBName->append(codec->toUnicode(QString(tabs.at(2)).remove("'").remove("\"").toAscii().data()));
                tempGr->SubItemGBName->append(QString(tabs.at(2)).remove("'").remove("\""));
                tempGr->SubName->append("");
                SearchValue();
            }

            tempGr->SubAmount->append(tabs.at(3).toInt());
            tempGr->SubPercent->append(tabs.at(4).toFloat());

            if(tabs.length() > 5)
                tempGr->SubEvent->append(tabs.at(5).toFloat());
            else
                tempGr->SubEvent->append(0);

            return true;
        }
    }

    return false;
}

void MobDropItem::SearchValue()
{
    int i = tempGr->SubItemVnum->length() - 1, searchRes;

    if(MyItemsInfo->IIFullItemProto->Count == 0)
    {
        return;
    }

    if(tempGr->SubItemVnum->at(i) != 0)
    {
        searchRes = MyItemsInfo->SearchVnum(tempGr->SubItemVnum->at(i));

        if(searchRes != -1)
        {
            tempGr->SubItemGBName->operator [](i) = MyItemsInfo->IIFullItemProto->gb2312->at(searchRes);
            tempGr->SubName->operator [](i) = MyItemsInfo->IIFullItemProto->name->at(searchRes);
        }
    }
    else
    {
        searchRes = MyItemsInfo->SearchGB2312(tempGr->SubItemGBName->at(i));

        if(searchRes != -1)
        {
            tempGr->SubItemVnum->operator [](i) = MyItemsInfo->IIFullItemProto->vnum->at(searchRes).toInt();
            tempGr->SubName->operator [](i) = MyItemsInfo->IIFullItemProto->name->at(searchRes);
        }
    }
}

void MobDropItem::SaveGroup(QString groupPath)
{
    QString MDI;
    QString NewLine = QString(QChar(0x0A));
    QString Tab = QString(QChar(0x09));
    QTextCodec *codec = QTextCodec::codecForName("GB2312");

    for(int i = 0; i < this->MDIGroups->length(); i++)
    {
        //MDI.append("Group" + Tab + codec->fromUnicode(this->MDIGroups->at(i).Name) + Tab + Tab + Tab + Tab + NewLine);
        MDI.append("Group" + Tab + this->MDIGroups->at(i).Name + Tab + Tab + Tab + Tab + NewLine);
        MDI.append("{" + Tab + Tab + Tab + Tab + Tab + NewLine);
        MDI.append(Tab + "Type" + Tab + this->MDIGroups->at(i).Type + Tab + Tab + Tab + NewLine);
        MDI.append(Tab + "Mob" + Tab + QString::number(this->MDIGroups->at(i).Mob) + Tab + Tab + Tab + NewLine);

        if(this->MDIGroups->at(i).Type.toUpper() == "LIMIT")
            MDI.append(Tab + "level_limit" + Tab + QString::number(this->MDIGroups->at(i).LevelLimit) + Tab + Tab + Tab + NewLine);
        else if(this->MDIGroups->at(i).Type.toUpper() == "KILL")
            MDI.append(Tab + "kill_drop" + Tab + QString::number(this->MDIGroups->at(i).KillDrop) + Tab + Tab + Tab + NewLine);

        for(int s = 0; s < this->MDIGroups->at(i).SubItemVnum->count(); s++)
        {
            if (this->MDIGroups->at(i).SubItemVnum->at(s) != 0)
                MDI.append(Tab + QString::number(s + 1) + Tab + QString::number(this->MDIGroups->at(i).SubItemVnum->at(s)));
            else
                MDI.append(Tab + QString::number(s + 1) + Tab + this->MDIGroups->at(i).SubItemGBName->at(s));
                //MDI.append(Tab + QString::number(s + 1) + Tab + codec->fromUnicode(this->MDIGroups->at(i).SubItemGBName->at(s)));

            MDI.append(Tab + QString::number(this->MDIGroups->at(i).SubAmount->at(s)));
            MDI.append(Tab + QString::number(this->MDIGroups->at(i).SubPercent->at(s)));

            if(this->MDIGroups->at(i).SubEvent->at(s) != 0)
                MDI.append(Tab + QString::number(this->MDIGroups->at(i).SubEvent->at(s)));
            else
                MDI.append(Tab);

            MDI.append(NewLine);
        }

        MDI.append("}" + Tab + Tab + Tab + Tab + Tab + NewLine);
    }

    QFile *bak = new QFile(groupPath + ".bak");

    if(QFile::exists(groupPath))
    {
        if(QFile::exists(groupPath + ".bak"))
            bak->remove();

        QFile::rename(groupPath, groupPath + ".bak");
        Syslog("Saved a backup of the 'Mob Drop Item' ("+groupPath+".bak)");
    }

    Syslog("Saved the 'Mob Drop Item' into the path ("+groupPath+")");
    IOWriteFile(groupPath, MDI.toAscii().data(), MDI.length(), "wb");
}

void MobDropItem::InsertNewEmptyItem(void)
{
    if(this->tempGr)
        delete tempGr;

    if(!this->MDIGroups)
        this->MDIGroups = new QList<MobDropItemStruct>;

    this->tempGr = new MobDropItemStruct;
    this->tempGr->SubItemVnum = new QList<uint32_t>;
    this->tempGr->SubItemGBName = new QStringList();
    this->tempGr->SubAmount = new QList<uint32_t>;
    this->tempGr->SubEvent = new QList<float>;
    this->tempGr->SubPercent = new QList<float>;
    this->tempGr->SubName = new QStringList();
    this->tempGr->Name = "";
    this->tempGr->Mob = 0;
    this->tempGr->Type = "drop";

    this->MDIGroups->append(*this->tempGr);
}
