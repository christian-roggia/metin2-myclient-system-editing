#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Initialize()
{
    QFile *syslog = new QFile("logs\\syslog.txt");
    connect(ui->aLoadItems, SIGNAL(triggered()), this, SLOT(ActionStart()));
    connect(ui->aLoadGroup, SIGNAL(triggered()), this, SLOT(LoadGroup()));
    connect(ui->aMob_Drop_Item, SIGNAL(triggered()), this, SLOT(LoadMobDropItem()));
    connect(ui->actionSave_3, SIGNAL(triggered()), this, SLOT(ItemSave()));
    connect(ui->aSaveItemProto, SIGNAL(triggered()), this, SLOT(SaveAll()));
    connect(ui->aSaveSpecialItemGroup, SIGNAL(triggered()), this, SLOT(SaveSpecialItemGroup()));
    connect(ui->actionUndo_All, SIGNAL(triggered()), this, SLOT(Undo_All()));
    connect(ui->aLoadFishing, SIGNAL(triggered()), this, SLOT(LoadFishing()));
    connect(ui->aMob_Drop_Item_2, SIGNAL(triggered()), this, SLOT(SaveMobDropItem()));
    connect(ui->aSQL, SIGNAL(triggered()), this, SLOT(SaveSQLQuery()));
    connect(ui->aItemAdd, SIGNAL(triggered()), this, SLOT(AddNewItem()));
    connect(ui->aItemDuplicate, SIGNAL(triggered()), this, SLOT(DucplicateItem()));
    connect(ui->aLoadMDATR, SIGNAL(triggered()), this, SLOT(LoadMDATR()));
    connect(ui->aSpecialItemGroup, SIGNAL(triggered()), this, SLOT(LoadSpecialItemGroup()));

    ui->tableWidget->setColumnWidth(1, 220);
    ui->tableWidget->setColumnWidth(2, 162);
    ui->tableWidget_2->setColumnWidth(0, 240);
    ui->tableWidget_3->setColumnWidth(0, 370);
    ui->tableWidget_4->setColumnWidth(0, 270);
    ui->tableWidget_5->setColumnWidth(1, 140);
    ui->tableWidget_5->setColumnWidth(2, 145);
    ui->tableWidget_5->setColumnWidth(3, 60);
    ui->tableWidget_6->setColumnWidth(0, 370);
    ui->SIGTable->setColumnWidth(0, 370);

    // Clear Labels
    ui->lDesc->clear();
    ui->lName->clear();

    this->setFixedSize(this->width(),this->height());
    syslog->remove();
}

void MainWindow::Undo_All()
{
    this->on_tableWidget_currentCellChanged(ui->tableWidget->currentRow(),ui->tableWidget->currentColumn(), 0, 0);
}

void MainWindow::ActionStart()
{
    MyItemsInfo->SetEncode(ui->setEncode->isChecked());
    // Decrypt Item Proto
    if (MyItemsInfo->CheckType((char *)"client\\locale\\it\\item_proto", ItemProto))
        Syslog("Loaded 'locale\\it\\item_proto' successfully!");
    else
        Syserr("Loading of 'locale\\it\\item_proto' failed!");

    // Load the Item_List.txt
    if (MyItemsInfo->CheckType((char *)"client\\locale\\it\\item_list.txt", ItemList))
        Syslog("Loaded 'locale\\it\\item_list.txt' successfully!");
    else
        Syserr("Loading of 'locale\\it\\item_list.txt' failed!");

    // Load the ItemDesc.txt
    if (MyItemsInfo->CheckType((char *)"client\\locale\\it\\itemdesc.txt", ItemDesc))
        Syslog("Loaded 'locale\\it\\itemdesc.txt' successfully!");
    else
        Syserr("Loading of 'locale\\it\\itemdesc.txt' failed!");

    LoadItemTable(ui->tableWidget);

    SelectTabs(ui->tabWidget_3, ui->tabWidget, 2, 0);
}

void MainWindow::LoadMDATR()
{
    mdatr = new MDATR();
    mdatr->LoadMATR("test.mdatr");

    for(int i = 0; i < mdatr->GetCollisions(); i++)
    {
        ui->MDATR_LIST->addItem(new QListWidgetItem(mdatr->LMDATR->at(i).name));
    }

    SelectTabs(ui->tabWidget_3, ui->tabWidget, 2, 1);
}

void MainWindow::LoadGroup()
{
    MLoadGroup(ui->tableWidget_3);
    SelectTabs(ui->tabWidget_3, ui->tabWidget_4, 3, 2);
}

void MainWindow::LoadMobDropItem()
{
    MLoadMobDropItem(ui->tableWidget_6);
    SelectTabs(ui->tabWidget_3, ui->tabWidget_4, 3, 3);
}

void MainWindow::LoadSpecialItemGroup()
{
    MLoadSpecialItemGroup(ui->SIGTable);
    SelectTabs(ui->tabWidget_3, ui->tabWidget_4, 3, 4);
}

void MainWindow::LoadFishing()
{
    MLoadFishing(ui->tableWidget_2);
    SelectTabs(ui->tabWidget_3, ui->tabWidget_4, 3, 1);
}

void MainWindow::SaveSQLQuery()
{

}

void MainWindow::AddNewItem()
{
    if(TabsSelected(ui->tabWidget_3, ui->tabWidget_4, 2, 0))
        AddNewItemToItemProto(ui->tableWidget);
    else if(TabsSelected(ui->tabWidget_3, ui->tabWidget_4, 3, 3))
        AddNewItemToMobDropItem(ui->tableWidget_6);
}

void MainWindow::DuplicateItem()
{
    MyItemsInfo->InsertNewEmptyItem();
    int i = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);

    QTableWidgetItem *WTItemVnum = new QTableWidgetItem(MyItemsInfo->IIFullItemProto->vnum->at(i));
    QTableWidgetItem *WTItemName = new QTableWidgetItem(MyItemsInfo->IIFullItemProto->name->at(i));
    QTableWidgetItem *WTItemGB2312 = new QTableWidgetItem(MyItemsInfo->IIFullItemProto->gb2312->at(i));

    ui->tableWidget->setItem(i, 0, WTItemVnum);
    ui->tableWidget->setItem(i, 1, WTItemName);
    ui->tableWidget->setItem(i, 2, WTItemGB2312);
}

void MainWindow::on_tableWidget_6_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    MSetSubMDI(currentRow, previousRow, ui->lineEdit_4, ui->lineEdit_5, ui->spinBox_2, ui->lExtra, ui->sExtra, ui->tableWidget_5);
}

void MainWindow::on_tableWidget_3_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    MSetGroupSubGroup(currentRow, previousRow, ui->lineEdit, ui->lineEdit_2, ui->spinBox, ui->tableWidget_4);
}

void MainWindow::SaveAll()
{
    if (MyItemsInfo->ItemProtoIsLoaded)
        MyItemsInfo->SaveItemProto("client\\locale\\it\\item_proto");
}

void MainWindow::SaveMobDropItem()
{
    mdItem->SaveGroup(ui->txtMDIp->text());
}

void MainWindow::SaveSpecialItemGroup()
{
    SIGGroups->SaveGroup(ui->txtSIGp->text());
}

void MainWindow::SetPreview(int row)
{
    QString IName = MyItemsInfo->IIFullItemProto->name->at(row);
    QString IGB2312 = MyItemsInfo->IIFullItemProto->gb2312->at(row);
    QString iconPath = MyItemsInfo->IIFullItemProto->icons->at(row);
    QString Qdesc;
    int Itype = MyItemsInfo->IIFullItemProto->type->at(row).toInt();

    if (ui->cGB2312->checkState() == Qt::Unchecked)
        ui->lName->setText(QString("<html><center>%0</center></html>").arg(IName));
    else
        ui->lName->setText(QString("<html><center>%0</center></html>").arg(IGB2312));

    ui->gvIcon->setScene(0);
    SetItemIcon(ui->gvIcon, QString("client\\" + iconPath), MyItemsInfo->IIFullItemProto->size->at(row).toInt());

    ui->lDesc->clear();
    Qdesc += "<html><center>";

    switch(Itype)
    {
    case Weapon:
        LoadWeapon(row, Qdesc);
        break;
    }

    ui->lDesc->setText(Qdesc + "</center></html>");

    Syslog(QString("Loaded Item #%0 [%1 - %2]").arg(QString::number(row), IName, IGB2312));
}

void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow == previousRow)
        return;

    SetPreview(currentRow);

    /* Set data */

    ui->txtItemDesc->setText(MyItemsInfo->IIFullItemProto->descs->at(currentRow));
    ui->txtItemName->setText(MyItemsInfo->IIFullItemProto->displayedNames->at(currentRow));
    ui->txtModelPath->setText(MyItemsInfo->IIFullItemProto->models->at(currentRow));
    ui->txtIconPath->setText(MyItemsInfo->IIFullItemProto->icons->at(currentRow));

    ui->sVnum->setValue(MyItemsInfo->IIFullItemProto->vnum->at(currentRow).toInt());
    ui->txtGB2312->setText(MyItemsInfo->IIFullItemProto->gb2312->at(currentRow));
    ui->txtName->setText(MyItemsInfo->IIFullItemProto->name->at(currentRow));
    ui->sType->setValue(MyItemsInfo->IIFullItemProto->type->at(currentRow).toInt());
    ui->sSubType->setValue(MyItemsInfo->IIFullItemProto->subtype->at(currentRow).toInt());
    ui->sWeight->setValue(MyItemsInfo->IIFullItemProto->weight->at(currentRow).toInt());
    ui->sSize->setValue(MyItemsInfo->IIFullItemProto->size->at(currentRow).toInt());
    ui->sAntiFlag->setValue(MyItemsInfo->IIFullItemProto->antiflag->at(currentRow).toULongLong());
    ui->sFlag->setValue(MyItemsInfo->IIFullItemProto->flag->at(currentRow).toULongLong());
    ui->sWearFlag->setValue(MyItemsInfo->IIFullItemProto->wearflag->at(currentRow).toULongLong());
    ui->sImmuneFlag->setValue(MyItemsInfo->IIFullItemProto->immuneflag->at(currentRow).toULongLong());
    ui->sGold->setValue(MyItemsInfo->IIFullItemProto->gold->at(currentRow).toULongLong());
    ui->sBuyPrice->setValue(MyItemsInfo->IIFullItemProto->buy_price->at(currentRow).toULongLong());
    ui->sLimitType0->setValue(MyItemsInfo->IIFullItemProto->limittype0->at(currentRow).toInt());
    ui->sLimitValue0->setValue(MyItemsInfo->IIFullItemProto->limitvalue0->at(currentRow).toULongLong());
    ui->sLimitType1->setValue(MyItemsInfo->IIFullItemProto->limittype1->at(currentRow).toInt());
    ui->sLimitValue1->setValue(MyItemsInfo->IIFullItemProto->limitvalue1->at(currentRow).toULongLong());
    ui->sApplyType0->setValue(MyItemsInfo->IIFullItemProto->applytype0->at(currentRow).toInt());
    ui->sApplyValue0->setValue(MyItemsInfo->IIFullItemProto->applyvalue0->at(currentRow).toULongLong());
    ui->sApplyType1->setValue(MyItemsInfo->IIFullItemProto->applytype1->at(currentRow).toInt());
    ui->sApplyValue1->setValue(MyItemsInfo->IIFullItemProto->applyvalue1->at(currentRow).toULongLong());
    ui->sApplyType2->setValue(MyItemsInfo->IIFullItemProto->applytype2->at(currentRow).toInt());
    ui->sApplyValue2->setValue(MyItemsInfo->IIFullItemProto->applyvalue2->at(currentRow).toULongLong());
    ui->sValue0->setValue(MyItemsInfo->IIFullItemProto->value0->at(currentRow).toULongLong());
    ui->sValue1->setValue(MyItemsInfo->IIFullItemProto->value1->at(currentRow).toULongLong());
    ui->sValue2->setValue(MyItemsInfo->IIFullItemProto->value2->at(currentRow).toULongLong());
    ui->sValue3->setValue(MyItemsInfo->IIFullItemProto->value3->at(currentRow).toULongLong());
    ui->sValue4->setValue(MyItemsInfo->IIFullItemProto->value4->at(currentRow).toULongLong());
    ui->sValue5->setValue(MyItemsInfo->IIFullItemProto->value5->at(currentRow).toULongLong());
    ui->sSocket0->setValue(MyItemsInfo->IIFullItemProto->socket0->at(currentRow).toULongLong());
    ui->sSocket1->setValue(MyItemsInfo->IIFullItemProto->socket1->at(currentRow).toULongLong());
    ui->sSocket2->setValue(MyItemsInfo->IIFullItemProto->socket2->at(currentRow).toULongLong());
    ui->sSocket3->setValue(MyItemsInfo->IIFullItemProto->socket3->at(currentRow).toULongLong());
    ui->sSocket4->setValue(MyItemsInfo->IIFullItemProto->socket4->at(currentRow).toULongLong());
    ui->sSocket5->setValue(MyItemsInfo->IIFullItemProto->socket5->at(currentRow).toULongLong());
    ui->sRefineVnum->setValue(MyItemsInfo->IIFullItemProto->refine_vnum->at(currentRow).toULongLong());
    ui->sRefineSet->setValue(MyItemsInfo->IIFullItemProto->refine_set->at(currentRow).toULong());
    ui->sMagicPCT->setValue(MyItemsInfo->IIFullItemProto->magic_pct->at(currentRow).toInt());
    ui->sSpecular->setValue(MyItemsInfo->IIFullItemProto->specular->at(currentRow).toInt());
    ui->sSocketPCT->setValue(MyItemsInfo->IIFullItemProto->socket_pct->at(currentRow).toInt());

    /* Set type */
    if (MyItemsInfo->IIFullItemProto->types->at(currentRow) == "ETC")
        ui->comboBox->setCurrentIndex(0);
    else if (MyItemsInfo->IIFullItemProto->types->at(currentRow) == "ARMOR")
        ui->comboBox->setCurrentIndex(1);
    else if (MyItemsInfo->IIFullItemProto->types->at(currentRow) == "WEAPON")
        ui->comboBox->setCurrentIndex(2);
    else
        ui->comboBox->setCurrentIndex(3);
}

void MainWindow::ItemSave()
{
    if(ui->tableWidget->selectedItems().count() <= 0)
        return;

    int row = ui->tableWidget->selectedItems().takeLast()->row();

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(ui->sVnum->value())));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(ui->txtName->text()));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(ui->txtGB2312->text()));

    MyItemsInfo->IIFullItemProto->name->operator [](row) = ui->txtName->text();
    MyItemsInfo->IIFullItemProto->gb2312->operator [](row) = ui->txtGB2312->text();
    MyItemsInfo->IIFullItemProto->vnum->operator [](row) = QString::number(ui->sVnum->value());
    MyItemsInfo->IIFullItemProto->antiflag->operator [](row) = QString::number(ui->sAntiFlag->value());
    MyItemsInfo->IIFullItemProto->applytype0->operator [](row) = QString::number(ui->sApplyType0->value());
    MyItemsInfo->IIFullItemProto->applytype1->operator [](row) = QString::number(ui->sApplyType1->value());
    MyItemsInfo->IIFullItemProto->applytype2->operator [](row) = QString::number(ui->sApplyType2->value());
    MyItemsInfo->IIFullItemProto->applyvalue0->operator [](row) = QString::number(ui->sApplyValue0->value());
    MyItemsInfo->IIFullItemProto->applyvalue1->operator [](row) = QString::number(ui->sApplyValue1->value());
    MyItemsInfo->IIFullItemProto->applyvalue2->operator [](row) = QString::number(ui->sApplyValue2->value());
    MyItemsInfo->IIFullItemProto->buy_price->operator [](row) = QString::number(ui->sBuyPrice->value());
    MyItemsInfo->IIFullItemProto->descs->operator [](row) = ui->txtItemDesc->toPlainText();
    MyItemsInfo->IIFullItemProto->displayedNames->operator [](row) = ui->txtItemName->text();
    MyItemsInfo->IIFullItemProto->flag->operator [](row) = QString::number(ui->sFlag->value());
    MyItemsInfo->IIFullItemProto->gold->operator [](row) = QString::number(ui->sGold->value());
    MyItemsInfo->IIFullItemProto->icons->operator [](row) = ui->txtIconPath->text();
    MyItemsInfo->IIFullItemProto->immuneflag->operator [](row) = QString::number(ui->sImmuneFlag->value());
    MyItemsInfo->IIFullItemProto->limittype0->operator [](row) = QString::number(ui->sLimitType0->value());
    MyItemsInfo->IIFullItemProto->limittype1->operator [](row) = QString::number(ui->sLimitType1->value());
    MyItemsInfo->IIFullItemProto->limitvalue0->operator [](row) = QString::number(ui->sLimitValue0->value());
    MyItemsInfo->IIFullItemProto->limitvalue1->operator [](row) = QString::number(ui->sLimitValue1->value());
    MyItemsInfo->IIFullItemProto->magic_pct->operator [](row) = QString::number(ui->sMagicPCT->value());
    MyItemsInfo->IIFullItemProto->models->operator [](row) = ui->txtModelPath->text();
    MyItemsInfo->IIFullItemProto->refine_set->operator [](row) = QString::number(ui->sRefineSet->value());
    MyItemsInfo->IIFullItemProto->refine_vnum->operator [](row) = QString::number(ui->sRefineVnum->value());
    MyItemsInfo->IIFullItemProto->size->operator [](row) = QString::number(ui->sSize->value());
    MyItemsInfo->IIFullItemProto->socket0->operator [](row) = QString::number(ui->sSocket0->value());
    MyItemsInfo->IIFullItemProto->socket1->operator [](row) = QString::number(ui->sSocket1->value());
    MyItemsInfo->IIFullItemProto->socket2->operator [](row) = QString::number(ui->sSocket2->value());
    MyItemsInfo->IIFullItemProto->socket3->operator [](row) = QString::number(ui->sSocket3->value());
    MyItemsInfo->IIFullItemProto->socket4->operator [](row) = QString::number(ui->sSocket4->value());
    MyItemsInfo->IIFullItemProto->socket5->operator [](row) = QString::number(ui->sSocket5->value());
    MyItemsInfo->IIFullItemProto->socket_pct->operator [](row) = QString::number(ui->sSocketPCT->value());
    MyItemsInfo->IIFullItemProto->specular->operator [](row) = QString::number(ui->sSpecular->value());
    MyItemsInfo->IIFullItemProto->subtype->operator [](row) = QString::number(ui->sSubType->value());
    MyItemsInfo->IIFullItemProto->type->operator [](row) = QString::number(ui->sType->value());
    //MyItemsInfo->IIFullItemProto->types->operator [](row) = QString::number(ui->->value());
    MyItemsInfo->IIFullItemProto->value0->operator [](row) = QString::number(ui->sValue0->value());
    MyItemsInfo->IIFullItemProto->value1->operator [](row) = QString::number(ui->sValue1->value());
    MyItemsInfo->IIFullItemProto->value2->operator [](row) = QString::number(ui->sValue2->value());
    MyItemsInfo->IIFullItemProto->value3->operator [](row) = QString::number(ui->sValue3->value());
    MyItemsInfo->IIFullItemProto->value4->operator [](row) = QString::number(ui->sValue4->value());
    MyItemsInfo->IIFullItemProto->value5->operator [](row) = QString::number(ui->sValue5->value());
    MyItemsInfo->IIFullItemProto->wearflag->operator [](row) = QString::number(ui->sWearFlag->value());
    MyItemsInfo->IIFullItemProto->weight->operator [](row) = QString::number(ui->sWeight->value());
}

void MainWindow::on_pushButton_clicked()
{
    ServerAttributeInfo * SAI = new ServerAttributeInfo();

    ui->lCState->setText("CURRENT STATE: DECOMPRESSING");

    if(!SAI->LoadServerAttribute("special\\development\\server_attr"))
        ui->lCState->setText("CURRENT STATE: ERROR");
    else
        ui->lCState->setText("CURRENT STATE: IDLE");
}

void MainWindow::on_pushButton_2_clicked()
{
    ServerAttributeInfo * SAI = new ServerAttributeInfo();
    char *header = (char *)malloc(8);
    QFile *syslog = new QFile("special\\development\\server_attr");
    int nX = ui->sX->value() * 4;
    int nY = ui->sY->value() * 4;
    int tempKB = 0, cmpSize = 8;
    uint32_t compSize = 0;

    syslog->remove();
    memcpy(header, &nX, 4);
    memcpy(header + 4, &nY, 4);
    IOWriteFile("special\\development\\server_attr", header, 8, "ab");

    ui->lCState->setText("CURRENT STATE: COMPRESSING");
    ui->progressBar->setMaximum(((nY / 4) * (nX / 4) * 4) * 2);

    for(int y = 0; y < nY / 4; y++)
    {
        for(int tempY = 0; tempY < 4; tempY++)
        {
            for(int x = 0; x < nX / 4; x++)
            {
                Syslog(QString("MAP_SERVER_ATTR :: Converting file %0%1\\attr.atr").arg(QString::number(x).rightJustified(3, '0'),
                                                                                            QString::number(y).rightJustified(3, '0')));
                if(!SAI->SaveServerAttribute(QString("special\\development\\client\\%0%1\\attr.atr").arg(QString::number(x).rightJustified(3, '0'),
                                                                                            QString::number(y).rightJustified(3, '0')), tempY, compSize))
                    ui->lCState->setText("CURRENT STATE: ERROR");
                else
                    ui->lCState->setText("CURRENT STATE: COMPRESSING");

                tempKB += 16;
                cmpSize += compSize + 16;
                compSize = 0;

                ui->progressBar->setValue(ui->progressBar->value() + 1);
                ui->lProcComp->setText(QString("Processed: %0 KB").arg(QString::number(tempKB)));
                ui->lCFSComp->setText(QString("Current File Size: %0 KB").arg(QString::number(cmpSize / 1024)));
                QApplication::processEvents();
            }
        }
    }

    ui->lCState->setText("CURRENT STATE: IDLE");
}

void MainWindow::on_MDATR_LIST_currentRowChanged(int currentRow)
{
    ui->MDATR_UNK_LIST->clear();
    ui->MDATR_F1->setText(QString::number(mdatr->LMDATR->at(currentRow).f1));
    ui->MDATR_F2->setText(QString::number(mdatr->LMDATR->at(currentRow).f2));
    ui->MDATR_F3->setText(QString::number(mdatr->LMDATR->at(currentRow).f3));
    ui->MDATR_F4->setText(QString::number(mdatr->LMDATR->at(currentRow).f4));
    ui->MDATR_X->setText(QString::number(mdatr->LMDATR->at(currentRow).X));
    ui->MDATR_Y->setText(QString::number(mdatr->LMDATR->at(currentRow).Y));
    ui->MDATR_Z->setText(QString::number(mdatr->LMDATR->at(currentRow).Z));
    ui->MDATR_F1->setText(QString::number(mdatr->LMDATR->at(currentRow).f1));

    if(mdatr->LMDATR->at(currentRow).unk1)
        ui->MDATR_UNK_LIST->addItem(new QListWidgetItem(QString::number(mdatr->LMDATR->at(currentRow).unk1)));
    if(mdatr->LMDATR->at(currentRow).unk2)
        ui->MDATR_UNK_LIST->addItem(new QListWidgetItem(QString::number(mdatr->LMDATR->at(currentRow).unk2)));
    if(mdatr->LMDATR->at(currentRow).unk3)
        ui->MDATR_UNK_LIST->addItem(new QListWidgetItem(QString::number(mdatr->LMDATR->at(currentRow).unk3)));
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->tableWidget_6->rowCount() <= 0)
        return;

    for(int i = 0; i < mdItem->MDIGroups->count(); i++)
    {
        switch(ui->MDIComboItemOp->currentIndex())
        {
        case 0:
            for(int j = 0; j < mdItem->MDIGroups->at(i).SubAmount->count(); j++)
            {
                mdItem->MDIGroups->at(i).SubAmount->operator [](j) *= ui->MDISpinOp->value();
            }
            break;
        case 1:
            for(int j = 0; j < mdItem->MDIGroups->at(i).SubPercent->count(); j++)
            {
                mdItem->MDIGroups->at(i).SubPercent->operator [](j) *= ui->MDISpinOp->value();
            }
            break;
        case 2:
            for(int j = 0; j < mdItem->MDIGroups->at(i).SubEvent->count(); j++)
            {
                mdItem->MDIGroups->at(i).SubEvent->operator [](j) *= ui->MDISpinOp->value();
            }
            break;
        default:
            QMessageBox msgBox;

            msgBox.setText("Unknown index in MDIComboItemOp. Please contact the support!");
            msgBox.exec();
            break;
        }
    }

    if(ui->tableWidget_6->currentRow() >= 0)
        this->on_tableWidget_6_currentCellChanged(ui->tableWidget_6->currentRow(), 0, -1, 0);
    else
        this->on_tableWidget_6_currentCellChanged(0, 0, -1, 0);
}

void MainWindow::on_pushButton_5_clicked()
{
    if(ui->tableWidget_6->rowCount() <= 0)
        return;

    for(int i = 0; i < mdItem->MDIGroups->count(); i++)
    {
        switch(ui->MDIComboItemOp->currentIndex())
        {
        case 0:
            for(int j = 0; j < mdItem->MDIGroups->at(i).SubAmount->count(); j++)
            {
                mdItem->MDIGroups->at(i).SubAmount->operator [](j) /= ui->MDISpinOp->value();
            }
            break;
        case 1:
            for(int j = 0; j < mdItem->MDIGroups->at(i).SubPercent->count(); j++)
            {
                mdItem->MDIGroups->at(i).SubPercent->operator [](j) /= ui->MDISpinOp->value();
            }
            break;
        case 2:
            for(int j = 0; j < mdItem->MDIGroups->at(i).SubEvent->count(); j++)
            {
                mdItem->MDIGroups->at(i).SubEvent->operator [](j) /= ui->MDISpinOp->value();
            }
            break;
        default:
            QMessageBox msgBox;

            msgBox.setText("Unknown index in MDIComboItemOp. Please contact the support!");
            msgBox.exec();
            break;
        }
    }

    if(ui->tableWidget_6->currentRow() >= 0)
        this->on_tableWidget_6_currentCellChanged(ui->tableWidget_6->currentRow(), 0, -1, 0);
    else
        this->on_tableWidget_6_currentCellChanged(0, 0, -1, 0);
}
