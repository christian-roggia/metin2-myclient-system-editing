#include "fishing.h"

QList<FishingItem> *Fishing::FishingTable;

Fishing::Fishing()
{
    FishingTable = new QList<FishingItem>;
}

bool Fishing::LoadFishingTable(QString fishPath)
{
    uint32_t size;
    QStringList lines, values;

    size = IOReadFile(fishPath.toAscii().data(), this->data);

    if(size == 0)
        return false;

    lines = QString(this->data).split(QChar(0x0a));

    for(int i = 0; i < lines.length(); i++)
    {
        if(!IsComment(lines.at(i)))
        {
            values = lines.at(i).split(QChar(0x09));

            if(values.length() > 22)
            {
                tempItem = new FishingItem();
                tempItem->Name = values.at(0);
                tempItem->Vnum = values.at(1).toInt();
                tempItem->Dead = values.at(2).toInt();
                tempItem->Grilled = values.at(3).toInt();
                tempItem->Prob1 = values.at(4).toInt();
                tempItem->Prob2 = values.at(5).toInt();
                tempItem->Prob3 = values.at(6).toInt();
                tempItem->Prob4 = values.at(7).toInt();
                tempItem->Max_Failures = values.at(8).toInt();
                tempItem->Min_Failures = values.at(9).toInt();
                tempItem->Time1 = values.at(10).toInt();
                tempItem->Time2 = values.at(11).toInt();
                tempItem->Time3 = values.at(12).toInt();
                tempItem->USED0 = values.at(13).toInt();
                tempItem->USED1 = values.at(14).toInt();
                tempItem->USED2 = values.at(15).toInt();
                tempItem->USED3 = values.at(16).toInt();
                tempItem->USED4 = values.at(17).toInt();
                tempItem->USED5 = values.at(18).toInt();
                tempItem->USED6 = values.at(19).toInt();
                tempItem->USED7 = values.at(20).toInt();
                tempItem->USED8 = values.at(21).toInt();
                tempItem->USED9 = values.at(22).toInt();

                Syslog(QString("FISHING :: LOAD FISHING TABLE :: Loaded item (%0) #%1").arg(tempItem->Name,
                                                                                             QString::number(tempItem->Vnum)));


                FishingTable->append(*tempItem);
                delete tempItem;
            }
        }
        else
        {
            Syslog(QString("FISHING :: LOAD FISHING TABLE :: Line %0 is a comment!").arg(QString::number(i)));
        }
    }

    return true;
}

bool Fishing::IsComment(QString line)
{
    return line.left(1) == "#";
}
