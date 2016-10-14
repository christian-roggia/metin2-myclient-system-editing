#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QTreeWidgetItem>
#include <QTextCodec>
#include <QFile>
#include <QMessageBox>

#include "global.h"

#include "manager/common.h"
#include "manager/itemprotomanager.h"
#include "manager/newitems.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void ActionStart();
    void ItemSave();
    void SaveAll();
    void SaveMobDropItem();
    void SaveSpecialItemGroup();
    void Undo_All();
    void LoadGroup();
    void LoadFishing();
    void LoadMobDropItem();
    void LoadSpecialItemGroup();
    void SaveSQLQuery();
    void AddNewItem();
    void DuplicateItem();
    void LoadMDATR();
    void on_tableWidget_3_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_tableWidget_6_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_MDATR_LIST_currentRowChanged(int currentRow);

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;

    void Initialize();
    void LoadTable();
    void SetPreview(int);
};

#endif // MAINWINDOW_H
