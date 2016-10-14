#include "items.h"
#include <QFile>
#include <QTextStream>

int FullItemProto::Count;
FullItemProto *ItemsInfo::IIFullItemProto;

ItemsInfo::ItemsInfo()
{
    this->IIFullItemProto = new FullItemProto();
    this->ItemProtoIsLoaded = false;
    this->buffer = NULL;
    this->lSize = 0;
    this->Encode = true;
}

ItemsInfo::~ItemsInfo()
{
    delete [] this->IIFullItemProto;
    delete [] this->buffer;
    this->ItemProtoIsLoaded = false;
    this->lSize = 0;
    this->Encode = true;
}

FullItemProto::FullItemProto()
{
    this->vnum = new QStringList();
    this->gb2312 = new QStringList();
    this->name = new QStringList();
    this->type = new QStringList();
    this->subtype = new QStringList();
    this->weight = new QStringList();
    this->size = new QStringList();
    this->antiflag = new QStringList();
    this->wearflag = new QStringList();
    this->flag = new QStringList();
    this->immuneflag = new QStringList();
    this->gold = new QStringList();
    this->buy_price = new QStringList();
    this->limittype0 = new QStringList();
    this->limitvalue0 = new QStringList();
    this->limittype1 = new QStringList();
    this->limitvalue1 = new QStringList();
    this->applytype0 = new QStringList();
    this->applyvalue0 = new QStringList();
    this->applytype1 = new QStringList();
    this->applyvalue1 = new QStringList();
    this->applytype2 = new QStringList();
    this->applyvalue2 = new QStringList();
    this->value0 = new QStringList();
    this->value1 = new QStringList();
    this->value2 = new QStringList();
    this->value3 = new QStringList();
    this->value4 = new QStringList();
    this->value5 = new QStringList();
    this->socket0 = new QStringList();
    this->socket1 = new QStringList();
    this->socket2 = new QStringList();
    this->socket3 = new QStringList();
    this->socket4 = new QStringList();
    this->socket5 = new QStringList();
    this->refine_vnum = new QStringList();
    this->refine_set = new QStringList();
    this->magic_pct = new QStringList();
    this->specular = new QStringList();
    this->socket_pct = new QStringList();

    this->displayedNames = new QStringList();
    this->descs = new QStringList();

    this->types = new QStringList();
    this->icons = new QStringList();
    this->models = new QStringList();

    Count = 0;
}

FullItemProto::~FullItemProto()
{
    delete [] this->vnum;
    delete [] this->gb2312;
    delete [] this->name;
    delete [] this->type;
    delete [] this->subtype;
    delete [] this->weight;
    delete [] this->size;
    delete [] this->antiflag;
    delete [] this->flag;
    delete [] this->wearflag;
    delete [] this->immuneflag;
    delete [] this->gold;
    delete [] this->buy_price;
    delete [] this->limittype0;
    delete [] this->limitvalue0;
    delete [] this->limittype1;
    delete [] this->limitvalue1;
    delete [] this->applytype0;
    delete [] this->applyvalue0;
    delete [] this->applytype1;
    delete [] this->applyvalue1;
    delete [] this->applytype2;
    delete [] this->applyvalue2;
    delete [] this->value0;
    delete [] this->value1;
    delete [] this->value2;
    delete [] this->value3;
    delete [] this->value4;
    delete [] this->value5;
    delete [] this->socket0;
    delete [] this->socket1;
    delete [] this->socket2;
    delete [] this->socket3;
    delete [] this->socket4;
    delete [] this->socket5;
    delete [] this->refine_vnum;
    delete [] this->refine_set;
    delete [] this->magic_pct;
    delete [] this->specular;
    delete [] this->socket_pct;

    delete [] this->displayedNames;
    delete [] this->descs;

    delete [] this->types;
    delete [] this->icons;
    delete [] this->models;

    Count = 0;
}

int ItemsInfo::SearchValue(QString vnum)
{
    for (int i = 0; i < this->IIFullItemProto->Count; i++)
    {
        if (this->IIFullItemProto->vnum->at(i) == vnum)
        {
            return i;
        }
    }

    return -1;
}

bool ItemsInfo::CheckType(char *filePath, ItemFile Type)
{
    switch(Type)
    {
    case 1:
        this->lSize = IOReadFile(filePath, this->buffer);
        return LoadItemDesc();
        break;
    case 2:
        this->lSize = IOReadFile(filePath, this->buffer);
        return LoadItemList();
        break;
    case 3:
        this->lSize = IOReadFile(filePath, this->buffer);
        return LoadItemProto();
        break;
    default:
        return false;
        break;
    }
}

bool ItemsInfo::LoadItemDesc()
{
    QString *allItemDesc = new QString(buffer);
    QStringList arrItemDesc = allItemDesc->split(char(0x0A));
    int value;

    if (this->IIFullItemProto->Count == 0)
        return false;

    Syslog(QString("ITEMS INFO : LOAD ITEM DESC :: Item Count = %0").arg(QString::number(arrItemDesc.count())));

    for (int i = 0; i < arrItemDesc.count(); i++)
    {
        if (arrItemDesc.at(i).split(QChar(9)).count() > 2)
        {
            value = SearchValue(arrItemDesc.at(i).split(QChar(9)).at(0));

            if (value != -1)
            {
                this->IIFullItemProto->displayedNames->operator [](value) = arrItemDesc.at(i).split(QChar(9)).at(1);
                this->IIFullItemProto->descs->operator [](value) = arrItemDesc.at(i).split(QChar(9)).at(2);
            }
        }
    }

    Syslog("ITEMS INFO : LOAD ITEM DESC :: Item descriptions loaded sucessfully!");

    return true;
}

bool ItemsInfo::LoadItemList()
{
    if (this->IIFullItemProto->Count == 0)
        return false;

    QString *allItemList = new QString(buffer);
    QStringList arrItemList = allItemList->split(char(0x0A));
    QStringList ItemInfos;
    int value;

    Syslog(QString("ITEMS INFO : LOAD ITEM LIST :: Item Count = %0").arg(QString::number(arrItemList.count())));

    for (int i = 0; i < arrItemList.count(); i++)
    {
        ItemInfos = arrItemList.at(i).split(QChar(9));
        value = SearchValue(ItemInfos.at(0));

        if (value != -1)
        {
            switch(ItemInfos.count())
            {
            case 3:
                this->IIFullItemProto->types->operator [](value) = ItemInfos.at(1);
                this->IIFullItemProto->icons->operator [](value)  = ItemInfos.at(2);
                this->IIFullItemProto->models->operator [](value) = "";
                break;
            case 4:
                this->IIFullItemProto->types->operator [](value) = ItemInfos.at(1);
                this->IIFullItemProto->icons->operator [](value) = ItemInfos.at(2);
                this->IIFullItemProto->models->operator [](value) = ItemInfos.at(3);
                break;
            default:
                break;
            }
        }
    }

    Syslog("ITEMS INFO : LOAD ITEM LIST :: Item list loaded sucessfully!");

    return true;
}

bool ItemsInfo::LoadItemProto()
{
    Load_Item_Proto *ip = new Load_Item_Proto(buffer, lSize);
    QTextCodec *codec = QTextCodec::codecForName("GB2312");

    if(!ip->CheckInfo())
        return false;

    Syslog("ITEMS INFO : LOAD ITEM PROTO :: CheckInfo OK!");

    if(!ip->Decrypt())
        return false;

    Syslog("ITEMS INFO : LOAD ITEM PROTO :: Decryption OK!");

    if(!ip->Decompress())
        return false;

    Syslog("ITEMS INFO : LOAD ITEM PROTO :: Decompression OK!");

    if (ip->GetItemCount() == 0)
        return false;

    Syslog(QString("ITEMS INFO : LOAD ITEM PROTO :: Item Count = %0").arg(QString::number(ip->GetItemCount())));

    char *bufferData = ip->GetData();

    for (int i = 0; i < ip->GetItemCount(); i++)
    {
        ip_struct* itemIP = reinterpret_cast<ip_struct*>(bufferData);

        this->IIFullItemProto->vnum->append(QString::number(itemIP->vnum));

        if(this->Encode)
            this->IIFullItemProto->gb2312->append(codec->toUnicode(QString(itemIP->gb2312).toAscii().data()));
        else
            this->IIFullItemProto->gb2312->append(QString(itemIP->gb2312));

        this->IIFullItemProto->name->append(QString(itemIP->name));
        this->IIFullItemProto->type->append(QString::number(itemIP->type));
        this->IIFullItemProto->subtype->append(QString::number(itemIP->subtype));
        this->IIFullItemProto->weight->append(QString::number(itemIP->weight));
        this->IIFullItemProto->size->append(QString::number(itemIP->size));
        this->IIFullItemProto->antiflag->append(QString::number(itemIP->antiflag));
        this->IIFullItemProto->flag->append(QString::number(itemIP->flag));
        this->IIFullItemProto->wearflag->append(QString::number(itemIP->wearflag));
        this->IIFullItemProto->immuneflag->append(QString::number(itemIP->immuneflag));
        this->IIFullItemProto->gold->append(QString::number(itemIP->gold));
        this->IIFullItemProto->buy_price->append(QString::number(itemIP->buy_price));
        this->IIFullItemProto->limittype0->append(QString::number(itemIP->limittype0));
        this->IIFullItemProto->limitvalue0->append(QString::number(itemIP->limitvalue0));
        this->IIFullItemProto->limittype1->append(QString::number(itemIP->limittype1));
        this->IIFullItemProto->limitvalue1->append(QString::number(itemIP->limitvalue1));
        this->IIFullItemProto->applytype0->append(QString::number(itemIP->applytype0));
        this->IIFullItemProto->applyvalue0->append(QString::number(itemIP->applyvalue0));
        this->IIFullItemProto->applytype1->append(QString::number(itemIP->applytype1));
        this->IIFullItemProto->applyvalue1->append(QString::number(itemIP->applyvalue1));
        this->IIFullItemProto->applytype2->append(QString::number(itemIP->applytype2));
        this->IIFullItemProto->applyvalue2->append(QString::number(itemIP->applyvalue2));
        this->IIFullItemProto->value0->append(QString::number(itemIP->value0));
        this->IIFullItemProto->value1->append(QString::number(itemIP->value1));
        this->IIFullItemProto->value2->append(QString::number(itemIP->value2));
        this->IIFullItemProto->value3->append(QString::number(itemIP->value3));
        this->IIFullItemProto->value4->append(QString::number(itemIP->value4));
        this->IIFullItemProto->value5->append(QString::number(itemIP->value5));
        this->IIFullItemProto->socket0->append(QString::number(itemIP->socket0));
        this->IIFullItemProto->socket1->append(QString::number(itemIP->socket1));
        this->IIFullItemProto->socket2->append(QString::number(itemIP->socket2));
        this->IIFullItemProto->socket3->append(QString::number(itemIP->socket3));
        this->IIFullItemProto->socket4->append(QString::number(itemIP->socket4));
        this->IIFullItemProto->socket5->append(QString::number(itemIP->socket5));
        this->IIFullItemProto->refine_vnum->append(QString::number(itemIP->refine_vnum));
        this->IIFullItemProto->refine_set->append(QString::number(itemIP->refine_set));
        this->IIFullItemProto->magic_pct->append(QString::number(itemIP->magic_pct));
        this->IIFullItemProto->specular->append(QString::number(itemIP->specular));
        this->IIFullItemProto->socket_pct->append(QString::number(itemIP->socket_pct));

        this->IIFullItemProto->displayedNames->append("");
        this->IIFullItemProto->descs->append("");

        this->IIFullItemProto->types->append("");
        this->IIFullItemProto->icons->append("");
        this->IIFullItemProto->models->append("");

        bufferData += 152;
    }

    this->IIFullItemProto->Count = ip->GetItemCount();
    this->ItemProtoIsLoaded = true;

    Syslog("ITEMS INFO : LOAD ITEM PROTO :: Item prototype loaded sucessfully!");

    return true;
}

void ItemsInfo::memcpyi(char *a, unsigned long int uli, int i)
{
    int temp = uli;
    memcpy(a, &temp, i);
}

bool ItemsInfo::SaveItemProto(QString path)
{
    int Size = this->IIFullItemProto->Count;
    char *bufferIP = (char *)malloc(Size * 152);
    int k, lIPname, lIPGB2312;

    memset(bufferIP, 0, Size * 152);

    // Decoding GB2312
    QTextCodec *codec = QTextCodec::codecForName("GB2312");

    for (int i = 0; i < Size; i++)
    {
        k = i *152;
        lIPname = this->IIFullItemProto->name->at(i).length();
        /*********** Encoding *************/
        if(this->Encode)
        {
            lIPGB2312 = codec->fromUnicode(this->IIFullItemProto->gb2312->at(i)).length();
            memcpy(bufferIP + k + 4, (char *)codec->fromUnicode(this->IIFullItemProto->gb2312->at(i)).data(), lIPGB2312);
        }
        else
        {
            lIPGB2312 = this->IIFullItemProto->gb2312->at(i).toAscii().length();
            memcpy(bufferIP + k + 4, (char *)this->IIFullItemProto->gb2312->at(i).toAscii().data(), lIPGB2312);
        }
        /**********************************/

        memcpyi(bufferIP + k, (long int)this->IIFullItemProto->vnum->at(i).toLong(), 4);
        memcpy(bufferIP + k + 29, (char *)this->IIFullItemProto->name->at(i).toAscii().data(), lIPname);
        memcpyi(bufferIP + k + 54, (short int)this->IIFullItemProto->type->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 55, (short int)this->IIFullItemProto->subtype->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 56, (short int)this->IIFullItemProto->weight->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 57, (short int)this->IIFullItemProto->size->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 58, (long int)this->IIFullItemProto->antiflag->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 62, (long int)this->IIFullItemProto->flag->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 66, (long int)this->IIFullItemProto->wearflag->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 70, (long int)this->IIFullItemProto->immuneflag->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 74, (long int)this->IIFullItemProto->gold->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 78, (long int)this->IIFullItemProto->buy_price->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 82, (short int)this->IIFullItemProto->limittype0->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 83, (long int)this->IIFullItemProto->limitvalue0->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 87, (short int)this->IIFullItemProto->limittype1->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 88, (long int)this->IIFullItemProto->limitvalue1->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 92, (short int)this->IIFullItemProto->applytype0->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 93, (long int)this->IIFullItemProto->applyvalue0->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 97, (short int)this->IIFullItemProto->applytype1->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 98, (long int)this->IIFullItemProto->applyvalue1->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 102, (short int)this->IIFullItemProto->applytype2->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 103, (long int)this->IIFullItemProto->applyvalue2->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 107, (long int)this->IIFullItemProto->value0->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 111, (long int)this->IIFullItemProto->value1->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 115, (long int)this->IIFullItemProto->value2->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 119, (long int)this->IIFullItemProto->value3->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 123, (long int)this->IIFullItemProto->value4->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 127, (long int)this->IIFullItemProto->value5->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 131, (long int)this->IIFullItemProto->socket0->at(i).toLong(), 2);
        memcpyi(bufferIP + k + 133, (long int)this->IIFullItemProto->socket1->at(i).toLong(), 2);
        memcpyi(bufferIP + k + 135, (long int)this->IIFullItemProto->socket2->at(i).toLong(), 2);
        memcpyi(bufferIP + k + 137, (long int)this->IIFullItemProto->socket3->at(i).toLong(), 2);
        memcpyi(bufferIP + k + 139, (long int)this->IIFullItemProto->socket4->at(i).toLong(), 2);
        memcpyi(bufferIP + k + 141, (long int)this->IIFullItemProto->socket5->at(i).toLong(), 2);
        memcpyi(bufferIP + k + 143, (long int)this->IIFullItemProto->refine_vnum->at(i).toLong(), 4);
        memcpyi(bufferIP + k + 147, (long int)this->IIFullItemProto->refine_set->at(i).toLong(), 2);
        memcpyi(bufferIP + k + 149, (short int)this->IIFullItemProto->magic_pct->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 150, (short int)this->IIFullItemProto->specular->at(i).toShort(), 1);
        memcpyi(bufferIP + k + 151, (short int)this->IIFullItemProto->socket_pct->at(i).toShort(), 1);
    }

    Save_Item_Proto *SIP = new Save_Item_Proto();
    SIP->Compress((const unsigned char *)bufferIP, Size * 152);
    SIP->Encrypt();

    QFile *bak = new QFile(path + ".bak");

    if(QFile::exists(path))
    {
        if(QFile::exists(path + ".bak"))
            bak->remove();

        QFile::rename(path, path + ".bak");
    }

    FILE *fIP;
    fIP = fopen(path.toAscii().data(), "wb");
    fwrite(SIP->GetData(), 1, SIP->GetTotalSize(), fIP);
    fclose(fIP);

    return true;
}

long int ItemsInfo::SearchVnum(int vnumToSearch)
{
    QString vnumToSearchS = QString::number(vnumToSearch);

    for(int i = 0; i < this->IIFullItemProto->Count; i++)
    {
        if(vnumToSearchS == this->IIFullItemProto->vnum->at(i)){ return i; }
    }

    return -1;
}

long int ItemsInfo::SearchGB2312(QString GB2312ToSearch)
{
    GB2312ToSearch = GB2312ToSearch.remove("'").remove("\"");

    for(int i = 0; i < this->IIFullItemProto->Count; i++)
    {
        if(GB2312ToSearch == QString(this->IIFullItemProto->gb2312->at(i)).remove("'").remove("\"")){ return i; }
    }

    return -1;
}

long int ItemsInfo::SearchName(QString nameToSearch)
{
    nameToSearch = nameToSearch.remove("'").remove("\"");

    for(int i = 0; i < this->IIFullItemProto->Count; i++)
    {
        if(nameToSearch == QString(this->IIFullItemProto->name->at(i)).remove("'").remove("\"")){ return i; }
    }

    return -1;
}

void ItemsInfo::SetEncode(bool enc)
{
    this->Encode = enc;
}

void ItemsInfo::InsertNewEmptyItem()
{
    this->IIFullItemProto->vnum->append("0");
    if(this->Encode)
        this->IIFullItemProto->gb2312->append("");
    else
        this->IIFullItemProto->gb2312->append("");
    this->IIFullItemProto->name->append("");
    this->IIFullItemProto->type->append("0");
    this->IIFullItemProto->subtype->append("0");
    this->IIFullItemProto->weight->append("0");
    this->IIFullItemProto->size->append("0");
    this->IIFullItemProto->antiflag->append("0");
    this->IIFullItemProto->flag->append("0");
    this->IIFullItemProto->wearflag->append("0");
    this->IIFullItemProto->immuneflag->append("0");
    this->IIFullItemProto->gold->append("0");
    this->IIFullItemProto->buy_price->append("0");
    this->IIFullItemProto->limittype0->append("0");
    this->IIFullItemProto->limitvalue0->append("0");
    this->IIFullItemProto->limittype1->append("0");
    this->IIFullItemProto->limitvalue1->append("0");
    this->IIFullItemProto->applytype0->append("0");
    this->IIFullItemProto->applyvalue0->append("0");
    this->IIFullItemProto->applytype1->append("0");
    this->IIFullItemProto->applyvalue1->append("0");
    this->IIFullItemProto->applytype2->append("0");
    this->IIFullItemProto->applyvalue2->append("0");
    this->IIFullItemProto->value0->append("0");
    this->IIFullItemProto->value1->append("0");
    this->IIFullItemProto->value2->append("0");
    this->IIFullItemProto->value3->append("0");
    this->IIFullItemProto->value4->append("0");
    this->IIFullItemProto->value5->append("0");
    this->IIFullItemProto->socket0->append("0");
    this->IIFullItemProto->socket1->append("0");
    this->IIFullItemProto->socket2->append("0");
    this->IIFullItemProto->socket3->append("0");
    this->IIFullItemProto->socket4->append("0");
    this->IIFullItemProto->socket5->append("0");
    this->IIFullItemProto->refine_vnum->append("0");
    this->IIFullItemProto->refine_set->append("0");
    this->IIFullItemProto->magic_pct->append("0");
    this->IIFullItemProto->specular->append("0");
    this->IIFullItemProto->socket_pct->append("0");

    this->IIFullItemProto->displayedNames->append("");
    this->IIFullItemProto->descs->append("");

    this->IIFullItemProto->types->append("");
    this->IIFullItemProto->icons->append("");
    this->IIFullItemProto->models->append("");
}
