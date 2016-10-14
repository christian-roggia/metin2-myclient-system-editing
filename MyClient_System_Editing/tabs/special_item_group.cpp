#include "special_item_group.h"

QList<SpecialItemGroupStruct> *SpecialItemGroup::SIGGroups;

SpecialItemGroup::SpecialItemGroup()
{
    this->SIGGroups = new QList<SpecialItemGroupStruct>;
}

void SpecialItemGroup::LoadGroup(QString groupPath)
{
    uint32_t size;
    char *data;
    QStringList lines, tabs;

    size = IOReadFile(groupPath.toAscii().data(), data);
    lines = QString(data).split(QRegExp("(\r\n|\n|\r)"));

    for(int i = 0; i < lines.length(); i++)
    {
        tabs = lines.at(i).split(0x09);

        //Syslog(QString("SPECIAL ITEM GROUP :: LOAD GROUP :: Line %0, tab found = %1, line = %2").arg(QString::number(i), QString::number(lines.at(i).split(0x09).count()), lines.at(i)));

        if(!CheckType(tabs))
        {
            if(tabs.at(0).isEmpty())
                Syslog(QString("SPECIAL ITEM GROUP :: LOAD GROUP :: Empty line #"+QString::number(i)));
            else if(tabs.at(0).left(2) == "//")
                Syslog(QString("SPECIAL ITEM GROUP :: LOAD GROUP :: Comment line (//) #"+QString::number(i)));
            else if(tabs.at(0).left(1) == "#")
                Syslog(QString("SPECIAL ITEM GROUP :: LOAD GROUP :: Comment line (#) #"+QString::number(i)));
            else if(tabs.at(0).left(2) == "--")
                Syslog(QString("SPECIAL ITEM GROUP :: LOAD GROUP :: Comment line (--) #"+QString::number(i)));
            else
                Syserr(QString("SPECIAL ITEM GROUP :: LOAD GROUP :: Strange line #%0 ~ [%1 Tabs - (%2)]").arg(QString::number(i), QString::number(tabs.length()), lines.at(i)));
        }
    }
}

bool SpecialItemGroup::CheckType(QStringList tabs)
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
            this->SIGGroups->append(*tempGr);
            delete tempGr;
        }
        return true;
    }

    if(tabs.length() > 1)
    {
        if(tabs.at(0).toUpper() == "GROUP")
        {
            tempGr = new SpecialItemGroupStruct;
            tempGr->SubItemVnum = new QList<uint32_t>;
            tempGr->SubItemGBName = new QStringList();
            tempGr->SubAmount = new QList<uint32_t>;
            tempGr->SubPercent = new QList<float>;
            tempGr->SubName = new QStringList();
            //tempGr->Name = codec->toUnicode(tabs.at(1).toAscii().data(), tabs.at(1).length());
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
                //tempGr->SubItemGBName->append(codec->toUnicode(QString(tabs.at(2)).remove("'").remove("\"").toAscii().data(), QString(tabs.at(2)).remove("'").remove("\"").length()));
                tempGr->SubItemGBName->append(QString(tabs.at(2)).remove("'").remove("\""));
                tempGr->SubName->append("");
                SearchValue();
            }

            tempGr->SubAmount->append(tabs.at(3).toInt());
            tempGr->SubPercent->append(tabs.at(4).toFloat());
            return true;
        }
    }

    return false;
}

void SpecialItemGroup::SearchValue()
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

void SpecialItemGroup::SaveGroup(QString groupPath)
{
    QString SIG;
    QString NewLine = QString(QChar(0x0A));
    QString Tab = QString(QChar(0x09));
    QTextCodec *codec = QTextCodec::codecForName("GB2312");

    for(int i = 0; i < this->SIGGroups->length(); i++)
    {
        //SIG.append("Group" + Tab + codec->fromUnicode(this->SIGGroups->at(i).Name) + Tab + Tab + Tab + Tab + NewLine);
        SIG.append("Group" + Tab + this->SIGGroups->at(i).Name + Tab + Tab + Tab + Tab + NewLine);
        SIG.append("{" + Tab + Tab + Tab + Tab + Tab + NewLine);
        SIG.append(Tab + "Vnum" + Tab + QString::number(this->SIGGroups->at(i).Vnum) + Tab + Tab + Tab + NewLine);

        for(int s = 0; s < this->SIGGroups->at(i).SubItemVnum->count(); s++)
        {
            if (this->SIGGroups->at(i).SubItemVnum->at(s) != 0)
                SIG.append(Tab + QString::number(s + 1) + Tab + QString::number(this->SIGGroups->at(i).SubItemVnum->at(s)));
            else
                SIG.append(Tab + QString::number(s + 1) + Tab + this->SIGGroups->at(i).SubItemGBName->at(s));
                //SIG.append(Tab + QString::number(s + 1) + Tab + codec->fromUnicode(this->SIGGroups->at(i).SubItemGBName->at(s)));

            SIG.append(Tab + QString::number(this->SIGGroups->at(i).SubAmount->at(s)));
            SIG.append(Tab + QString::number(this->SIGGroups->at(i).SubPercent->at(s)));

            SIG.append(NewLine);
        }

        SIG.append("}" + Tab + Tab + Tab + Tab + Tab + NewLine);
    }

    QFile *bak = new QFile(groupPath + ".bak");

    if(QFile::exists(groupPath))
    {
        if(QFile::exists(groupPath + ".bak"))
            bak->remove();

        QFile::rename(groupPath, groupPath + ".bak");
        Syslog("Saved a backup of the 'SPECIAL ITEM GROUP' ("+groupPath+".bak)");
    }

    Syslog("Saved the 'SPECIAL ITEM GROUP' into the path ("+groupPath+")");
    IOWriteFile(groupPath, SIG.toAscii().data(), SIG.length(), "wb");
}

void SpecialItemGroup::InsertNewEmptyItem(void)
{
    if(this->tempGr)
        delete tempGr;

    if(!this->SIGGroups)
        this->SIGGroups = new QList<SpecialItemGroupStruct>;

    this->tempGr = new SpecialItemGroupStruct;
    this->tempGr->SubItemVnum = new QList<uint32_t>;
    this->tempGr->SubItemGBName = new QStringList();
    this->tempGr->SubAmount = new QList<uint32_t>;
    this->tempGr->SubPercent = new QList<float>;
    this->tempGr->SubName = new QStringList();
    this->tempGr->Name = "";
    this->tempGr->Vnum = 0;

    this->SIGGroups->append(*this->tempGr);
}
