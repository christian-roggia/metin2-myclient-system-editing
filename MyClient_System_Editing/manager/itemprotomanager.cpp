#include "itemprotomanager.h"

void SetItemIcon(QGraphicsView *&gvIcon, QString iconPath, int hIcon)
{
    if(iconPath == "client\\")
        return;

    if(!QFile::exists(iconPath))
    {
        Syserr(QString("SET ITEM ICON :: Icon not found (%0)!").arg(iconPath));
        return;
    }

    TGAImg *newTGA = new TGAImg;
    QImage *MyTGA;
    QGraphicsScene *gs = new QGraphicsScene();

    gvIcon->setFixedSize(34, 32 * hIcon + 2);

    if(newTGA->Load(iconPath.toAscii().data()) != IMG_OK)
    {
        Syserr(QString("SET ITEM ICON :: Invalid file Truevision Targa (%0)!").arg(iconPath));
        return;
    }

    MyTGA = new QImage(newTGA->GetImg(), newTGA->GetWidth(), newTGA->GetHeight(), QImage::Format_ARGB32);

    gs->addPixmap(QPixmap::fromImage(*MyTGA));
    gvIcon->setScene(gs);
}

void LoadItemTable(QTableWidget *&itemTable)
{
    for (int i = 0; i < MyItemsInfo->IIFullItemProto->vnum->count(); i++)
    {
        itemTable->setRowCount(i + 1);

        QTableWidgetItem *WTItemVnum = new QTableWidgetItem(MyItemsInfo->IIFullItemProto->vnum->at(i));
        QTableWidgetItem *WTItemName = new QTableWidgetItem(MyItemsInfo->IIFullItemProto->name->at(i));
        QTableWidgetItem *WTItemGB2312 = new QTableWidgetItem(MyItemsInfo->IIFullItemProto->gb2312->at(i));

        itemTable->setItem(i, 0, WTItemVnum);
        itemTable->setItem(i, 1, WTItemName);
        itemTable->setItem(i, 2, WTItemGB2312);
    }
}

void GetItemRace(QString &Qdesc, int race)
{
    // Equip
    switch(race)
    {
    case Sword:
        Qdesc += "<br>Warrior Assasin Sura<br>";
        break;
    case Dagger:
    case Bow:
        Qdesc += "<br>Assasin<br>";
        break;
    case BroadSword:
        Qdesc += "<br>Warrior<br>";
        break;
    case Bell:
    case Fan:
        Qdesc += "<br>Shaman<br>";
        break;
    default:
        Qdesc += "<br>Unknown<br>";
        break;
    }
}

void LoadWeapon(int row, QString &Qdesc)
{
    int Race = MyItemsInfo->IIFullItemProto->subtype->at(row).toInt();
    QString atkMin = QString::number(MyItemsInfo->IIFullItemProto->value3->at(row).toInt() + MyItemsInfo->IIFullItemProto->value5->at(row).toInt());
    QString atkMax = QString::number(MyItemsInfo->IIFullItemProto->value4->at(row).toInt() + MyItemsInfo->IIFullItemProto->value5->at(row).toInt());
    QString sAtkMin = QString::number(MyItemsInfo->IIFullItemProto->value1->at(row).toInt() + MyItemsInfo->IIFullItemProto->value5->at(row).toInt());
    QString sAtkMax = QString::number(MyItemsInfo->IIFullItemProto->value2->at(row).toInt() + MyItemsInfo->IIFullItemProto->value5->at(row).toInt());

    // Level Required
    if (MyItemsInfo->IIFullItemProto->limittype0->at(row).toInt() == true)
        Qdesc += QString("From Level: %0<br><br>").arg(MyItemsInfo->IIFullItemProto->limitvalue0->at(row));

    Qdesc += QString("Attack Power +%0 - %1<br>").arg(atkMin, atkMax);
    Qdesc += QString("Magic Attack Power +%0 - %1<br>").arg(sAtkMin, sAtkMax);

    // Skill Speed
    if (MyItemsInfo->IIFullItemProto->applytype0->at(row).toInt() == SkillSpeed)
        Qdesc += QString("Attack Speed +%0%<br>").arg(MyItemsInfo->IIFullItemProto->applyvalue0->at(row));

    // Wearable + Attack + Attack Speed
    Qdesc += "<br><span style=\" font-size:8pt; color:#bababa;\">[Wearable]</span><br>";

    GetItemRace(Qdesc, Race);
}
