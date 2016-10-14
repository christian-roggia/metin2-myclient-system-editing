#ifndef ITEMPROTOMANAGER_H
#define ITEMPROTOMANAGER_H

#include <QString>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "global.h"
#include "common/tga.h"

void SetItemIcon(QGraphicsView *&,QString, int);
void LoadItemTable(QTableWidget *&);
void GetItemRace(QString &, int);
void LoadWeapon(int, QString &);

#endif // ITEMPROTOMANAGER_H
