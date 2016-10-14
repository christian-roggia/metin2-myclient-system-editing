#ifndef COMMONMANAGER_H
#define COMMONMANAGER_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QTabWidget>

#include "global.h"

void MLoadMobDropItem(QTableWidget *&);
void MLoadSpecialItemGroup(QTableWidget *&);
void MLoadGroup(QTableWidget *&);
void MLoadFishing(QTableWidget *&);

void MSetGroupSubGroup(int, int, QLineEdit *&, QLineEdit *&, QSpinBox *&, QTableWidget *&);
void MSetSubMDI(int, int, QLineEdit *&, QLineEdit *&, QSpinBox *&, QLabel *&, QSpinBox *&, QTableWidget *&);
bool TabsSelected(QTabWidget *, QTabWidget *, int, int);
void SelectTabs(QTabWidget *&, QTabWidget *&, int, int);


#endif // COMMONMANAGER_H
