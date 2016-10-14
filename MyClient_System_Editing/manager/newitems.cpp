#include "manager/newitems.h"

void AddNewItemToItemProto(QTableWidget *&tIP)
{
    MyItemsInfo->InsertNewEmptyItem();
    int i = tIP->rowCount();
    tIP->setRowCount(i + 1);

    tIP->setItem(i, 0, new QTableWidgetItem(MyItemsInfo->IIFullItemProto->vnum->at(i)));
    tIP->setItem(i, 1, new QTableWidgetItem(MyItemsInfo->IIFullItemProto->name->at(i)));
    tIP->setItem(i, 2, new QTableWidgetItem(MyItemsInfo->IIFullItemProto->gb2312->at(i)));
}

void AddNewItemToMobDropItem(QTableWidget *&tIP)
{
    mdItem->InsertNewEmptyItem();
    int i = tIP->rowCount();
    tIP->setRowCount(i + 1);

    tIP->setItem(i, 0, new QTableWidgetItem(0));
    tIP->setItem(i, 1, new QTableWidgetItem(0));
}
