#ifndef ITEMSINFO_H
#define ITEMSINFO_H

#include <locale/item_proto.h>
#include "global.h"

enum ItemFile
{
    ItemDesc = 1,
    ItemList = 2,
    ItemProto = 3
};

class FullItemProto
{
public:
    FullItemProto();
    ~FullItemProto();
    QStringList *vnum;
    QStringList *gb2312;
    QStringList *name;
    QStringList *type;
    QStringList *subtype;
    QStringList *weight;
    QStringList *size;
    QStringList *antiflag;
    QStringList *flag;
    QStringList *wearflag;
    QStringList *immuneflag;
    QStringList *gold;
    QStringList *buy_price;
    QStringList *limittype0;
    QStringList *limitvalue0;
    QStringList *limittype1;
    QStringList *limitvalue1;
    QStringList *applytype0;
    QStringList *applyvalue0;
    QStringList *applytype1;
    QStringList *applyvalue1;
    QStringList *applytype2;
    QStringList *applyvalue2;
    QStringList *value0;
    QStringList *value1;
    QStringList *value2;
    QStringList *value3;
    QStringList *value4;
    QStringList *value5;
    QStringList *socket0;
    QStringList *socket1;
    QStringList *socket2;
    QStringList *socket3;
    QStringList *socket4;
    QStringList *socket5;
    QStringList *refine_vnum;
    QStringList *refine_set;
    QStringList *magic_pct;
    QStringList *specular;
    QStringList *socket_pct;

    QStringList *types;
    QStringList *icons;
    QStringList *models;

    QStringList *displayedNames;
    QStringList *descs;

    static int Count;
};

class ItemsInfo
{
public:
    ItemsInfo();
    ~ItemsInfo();

    bool CheckType(char *filePath, ItemFile Type);
    bool SaveItemProto(QString);
    long int SearchVnum(int);
    long int SearchGB2312(QString);
    long int SearchName(QString);
    void SetEncode(bool);
    void InsertNewEmptyItem();

    bool ItemProtoIsLoaded;
    static FullItemProto *IIFullItemProto;
private:
    int SearchValue(QString);
    void memcpyi(char *a, unsigned long int uli, int i);

    bool LoadItemDesc();
    bool LoadItemList();
    bool LoadItemProto();

    uint32_t lSize;
    char *buffer;
    bool Encode;
};

#endif // ITEMSINFO_H
