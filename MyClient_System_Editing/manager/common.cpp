#include "common.h"

void MLoadMobDropItem(QTableWidget *&MobTable)
{
    if(mdItem != NULL)
        delete mdItem;

    mdItem = new MobDropItem();

    mdItem->LoadGroup((char *)"server\\mob_drop_item.txt");

    // Clear rows
    for (int i = 0; i < MobTable->rowCount(); ++i)
        MobTable->removeRow(i);

    for(int i = 0; i < mdItem->MDIGroups->length(); i++)
    {
        MobTable->setRowCount(i + 1);
        MobTable->setItem(i, 0, new QTableWidgetItem(mdItem->MDIGroups->at(i).Name));
        MobTable->setItem(i, 1, new QTableWidgetItem(QString::number(mdItem->MDIGroups->at(i).Mob)));
    }
}

void MLoadSpecialItemGroup(QTableWidget *&SIGTable)
{
    if(SIGGroups != NULL)
        delete SIGGroups;

    SIGGroups = new SpecialItemGroup();

    SIGGroups->LoadGroup((char *)"server\\special_item_group.txt");

    // Clear rows
    for (int i = 0; i < SIGTable->rowCount(); ++i)
        SIGTable->removeRow(i);

    for(int i = 0; i < SIGGroups->SIGGroups->length(); i++)
    {
        SIGTable->setRowCount(i + 1);
        SIGTable->setItem(i, 0, new QTableWidgetItem(SIGGroups->SIGGroups->at(i).Name));
        SIGTable->setItem(i, 1, new QTableWidgetItem(QString::number(SIGGroups->SIGGroups->at(i).Vnum)));
    }
}

void MLoadGroup(QTableWidget *&GroupTable)
{
    if(gInfo != NULL)
        delete gInfo;

    gInfo = new GroupInfo();

    gInfo->OpenGroup((char *)"server\\group.txt");

    // Clear rows
    for (int i = 0; i < GroupTable->rowCount(); ++i)
        GroupTable->removeRow(i);

    for(int i = 0; i < gInfo->Groups->length(); i++)
    {
        GroupTable->setRowCount(i + 1);
        GroupTable->setItem(i, 0, new QTableWidgetItem(gInfo->Groups->at(i).Name));
        GroupTable->setItem(i, 1, new QTableWidgetItem(QString::number(gInfo->Groups->at(i).Vnum)));
    }
}

void MLoadFishing(QTableWidget *&FishTable)
{
    if(fish != NULL)
        delete fish;

    fish = new Fishing;

    fish->LoadFishingTable("server\\fishing.txt");

    // Clear rows
    for (int i = 0; i < FishTable->rowCount(); ++i)
        FishTable->removeRow(i);

    for(int i = 0; i < fish->FishingTable->length(); i++)
    {
        FishTable->setRowCount(i + 1);
        FishTable->setItem(i, 0, new QTableWidgetItem(fish->FishingTable->at(i).Name));
        FishTable->setItem(i, 1, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Vnum)));
        FishTable->setItem(i, 2, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Dead)));
        FishTable->setItem(i, 3, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Grilled)));
        FishTable->setItem(i, 4, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Prob1)));
        FishTable->setItem(i, 5, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Prob2)));
        FishTable->setItem(i, 6, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Prob3)));
        FishTable->setItem(i, 7, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Prob4)));
        FishTable->setItem(i, 8, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Max_Failures)));
        FishTable->setItem(i, 9, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Min_Failures)));
        FishTable->setItem(i, 10, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Time1)));
        FishTable->setItem(i, 11, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Time2)));
        FishTable->setItem(i, 12, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).Time3)));
        FishTable->setItem(i, 13, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).USED0)));
        FishTable->setItem(i, 14, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).USED1)));
        FishTable->setItem(i, 15, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).USED2)));
        FishTable->setItem(i, 16, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).USED3)));
        FishTable->setItem(i, 17, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).USED4)));
        FishTable->setItem(i, 18, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).USED5)));
        FishTable->setItem(i, 19, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).USED6)));
        FishTable->setItem(i, 20, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).USED7)));
        FishTable->setItem(i, 21, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).USED8)));
        FishTable->setItem(i, 22, new QTableWidgetItem(QString::number(fish->FishingTable->at(i).USED9)));
    }
}

void MSetGroupSubGroup(int currentRow, int previousRow, QLineEdit *&GName, QLineEdit *&GLeader, QSpinBox *&GLVnum, QTableWidget *&SubGroupTable)
{
    if (currentRow == previousRow)
        return;

    GName->setText(gInfo->Groups->at(currentRow).Name);
    GLeader->setText(gInfo->Groups->at(currentRow).Leader);
    GLVnum->setValue(gInfo->Groups->at(currentRow).LeaderVnum);
    SubGroupTable->clear();

    for(int i = 0; i < gInfo->Groups->at(currentRow).SubMob.count(); i++)
    {
        SubGroupTable->setRowCount(i + 1);
        SubGroupTable->setItem(i, 0, new QTableWidgetItem(gInfo->Groups->at(currentRow).SubMob.at(i)));
        SubGroupTable->setItem(i, 1, new QTableWidgetItem(QString::number(gInfo->Groups->at(currentRow).SubMobVnum.at(i))));
    }
}

void MSetSubMDI(int currentRow, int previousRow, QLineEdit *&MDIName, QLineEdit *&MDIType, QSpinBox *&MDIVnum, QLabel *&lMDIExtra, QSpinBox *&sMDIExtra, QTableWidget *&SubMDITable)
{
    if (currentRow == previousRow)
        return;

    QPalette sPalette(sMDIExtra->palette());

    MDIName->setText(mdItem->MDIGroups->at(currentRow).Name);
    MDIType->setText(mdItem->MDIGroups->at(currentRow).Type);
    MDIVnum->setValue(mdItem->MDIGroups->at(currentRow).Mob);

    if(mdItem->MDIGroups->at(currentRow).Type.toUpper() == "LIMIT")
    {
        lMDIExtra->setEnabled(true);
        sMDIExtra->setEnabled(true);
        lMDIExtra->setText("Level Limit");
        sMDIExtra->setValue(mdItem->MDIGroups->at(currentRow).LevelLimit);

        if(mdItem->MDIGroups->at(currentRow).LevelLimit == 0)
            sPalette.setColor(QPalette::Active, QPalette::Base, qRgb(245,57,14));
        else
            sPalette.setColor(QPalette::Active, QPalette::Base, qRgb(255,255,255));
    }
    else if(mdItem->MDIGroups->at(currentRow).Type.toUpper() == "KILL")
    {
        lMDIExtra->setEnabled(true);
        sMDIExtra->setEnabled(true);
        lMDIExtra->setText("Kill Drop");
        sMDIExtra->setValue(mdItem->MDIGroups->at(currentRow).KillDrop);

        if(mdItem->MDIGroups->at(currentRow).KillDrop == 0)
            sPalette.setColor(QPalette::Active, QPalette::Base, qRgb(245,57,14));
        else
            sPalette.setColor(QPalette::Active, QPalette::Base, qRgb(255,255,255));
    }
    else
    {
        lMDIExtra->setEnabled(false);
        sMDIExtra->setEnabled(false);
        lMDIExtra->setText("Extra Values");
        sMDIExtra->setValue(0);
        sPalette.setColor(QPalette::Active, QPalette::Base, qRgb(255,255,255));
    }
    sMDIExtra->setPalette(sPalette);

    for(int i = 0; i < mdItem->MDIGroups->at(currentRow).SubItemVnum->length(); i++)
    {
        SubMDITable->setRowCount(i + 1);
        SubMDITable->setItem(i, 0, new QTableWidgetItem(QString::number(mdItem->MDIGroups->at(currentRow).SubItemVnum->at(i))));
        SubMDITable->setItem(i, 1, new QTableWidgetItem(mdItem->MDIGroups->at(currentRow).SubName->at(i)));
        SubMDITable->setItem(i, 2, new QTableWidgetItem(mdItem->MDIGroups->at(currentRow).SubItemGBName->at(i)));
        SubMDITable->setItem(i, 3, new QTableWidgetItem(QString::number(mdItem->MDIGroups->at(currentRow).SubAmount->at(i))));
        SubMDITable->setItem(i, 4, new QTableWidgetItem(QString::number(mdItem->MDIGroups->at(currentRow).SubPercent->at(i))));
        SubMDITable->setItem(i, 5, new QTableWidgetItem(QString::number(mdItem->MDIGroups->at(currentRow).SubEvent->at(i))));
    }
}

void SelectTabs(QTabWidget *&tw1, QTabWidget *&tw2, int sw1, int sw2)
{
    tw1->setCurrentIndex(sw1);
    tw2->setCurrentIndex(sw2);
}

bool TabsSelected(QTabWidget *tw1, QTabWidget *tw2, int sw1, int sw2)
{
    return (tw1->currentIndex() == sw1 && tw2->currentIndex() == sw2);
}
