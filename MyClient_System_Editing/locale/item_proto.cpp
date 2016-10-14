#include "item_proto.h"

Load_Item_Proto::Load_Item_Proto(char *ipData, uint32_t ipSize)
{
    this->itemCount = 0;
    this->cSize = 0;
    this->dSize = 0;
    this->hItemCount = 0;

    this->data = ipData;
    this->size = ipSize;
}

Load_Item_Proto::~Load_Item_Proto()
{
    delete this->data;

    this->itemCount = 0;
    this->cSize = 0;
    this->dSize = 0;
    this->hItemCount = 0;
    this->size = 0;
}

bool Load_Item_Proto::CheckInfo()
{
    IP_Header *ipHeader = (IP_Header *)(this->data);

    if (ipHeader->Header != MIPX && ipHeader->Header != MIPT)
    {
        Syserr(QString("ITEM PROTO : CHECK INFO :: Invalid header [%0]!").arg(QString::number(ipHeader->Header)));
        return false;
    }
    else if (ipHeader->Version != 1)
    {
        Syserr(QString("ITEM PROTO : CHECK INFO :: Unsupported version [%0]!").arg(QString::number(ipHeader->Version)));
        return false;
    }
    else if (ipHeader->SingleItemLength != 152)
    {
        Syserr(QString("ITEM PROTO : CHECK INFO :: Unsupported single item length [%0]!").arg(QString::number(ipHeader->SingleItemLength)));
        return false;
    }

    this->hItemCount = ipHeader->ItemCount;
    this->data += 20;
    this->size -= 20;
    return true;
}

bool Load_Item_Proto::Decrypt()
{
    uint32_t key[] = {0x0002A4A1, 0x045415AA, 0x185A8BE7, 0x01AAD6AB};
    LZObject *lzo;
    uint32_t dSize;

    LZObjectHeader* eInfo = (LZObjectHeader*)this->data;
    if (eInfo->Header != MCOZ)
    {
        Syserr(QString("ITEM PROTO : DECRYPTION :: Incorrect Magic Header Pre-Decryption [%0]!").arg(QString::number(eInfo->Header)));
        return false;
    }

    this->data += 16;
    this->size -= 16;

    if(eInfo->DecryptedBlockSize > this->size)
    {
        Syserr(QString("ITEM PROTO : DECRYPTION :: File is too small [DecryptedBlockSize = %0, size = %1]!").arg(QString::number(eInfo->DecryptedBlockSize),
                                                                                                                QString::number(size)));
        return false;
    }
    else if(eInfo->DecryptedBlockSize != this->size)
    {
        Syserr(QString("ITEM PROTO : DECRYPTION :: Warning size differs [DecryptedBlockSize = %0, size = %1]!").arg(QString::number(eInfo->DecryptedBlockSize),
                                                                                                                QString::number(size)));
        this->size = eInfo->DecryptedBlockSize;
    }
    else
    {
        Syslog(QString("ITEM PROTO : DECRYPTION :: File size OK! [DecryptedBlockSize = %0, size = %1]!").arg(QString::number(eInfo->DecryptedBlockSize),
                                                                                                             QString::number(size)));
    }

    lzo = new LZObject(this->data, key, this->size);

    lzo->ObjectDecrypt();
    dSize = lzo->GetDataSize();

    //delete this->data;
    this->data = (char *)malloc(dSize);
    memcpy(this->data, lzo->GetDecryptedData(), dSize);

    if (*(uint32_t *)this->data != MCOZ)
    {
        Syserr(QString("ITEM PROTO : DECRYPTION :: Incorrect Magic Header Post-Decryption [%0] - Decryption failed!").arg(QString::number(eInfo->Header)));
        return false;
    }

    this->cSize = eInfo->CompressedBlockSize;
    this->dSize = eInfo->DecompressedBlockSize;
    this->data += 4;
    return true;
}

bool Load_Item_Proto::Decompress()
{
    lzo_uint finalSize = 0;
    char *dData = (char *)malloc(this->dSize);

    lzo1x_decompress((unsigned char *)this->data, this->cSize, (unsigned char *)dData, &finalSize, 0);

    if(finalSize != this->dSize)
    {
        Syserr(QString("ITEM PROTO : DECOMPRESSION :: Size differs [dSize = %0, finalSize = % 1]!").arg(QString::number(this->dSize),
                                                                                                        QString::number(finalSize)));
        return false;
    }

    if (!finalSize % 152)
    {
        Syserr("ITEM PROTO : DECOMPRESSION :: Invalid decompressed Size!");
        return false;
    }

    this->itemCount = finalSize / 152;

    if(this->itemCount != this->hItemCount)
    {
        Syserr(QString("ITEM PROTO : DECOMPRESSION :: Invalid Item Count [itemCount = %0, hItemCount = %1]!").arg(QString::number(this->itemCount),
                                                                                                                  QString::number(this->hItemCount)));
        return false;
    }

    this->data = (char *)malloc(this->dSize);
    memcpy(this->data, dData, this->dSize);

    return true;
}

char *Load_Item_Proto::GetData()
{
    return this->data;
}

int Load_Item_Proto::GetItemCount()
{
    return itemCount;
}

Save_Item_Proto::Save_Item_Proto()
{
    this->data = NULL;
    this->compSize = 0;
    this->decompSize = 0;
    this->encryptedDataSize = 0;
    this->itemCount = 0;
    this->LZObjectSize = 0;
    this->totalSize = 0;
}

Save_Item_Proto::~Save_Item_Proto()
{
    delete [] this->data;
    this->compSize = 0;
    this->decompSize = 0;
    this->encryptedDataSize = 0;
    this->itemCount = 0;
    this->LZObjectSize = 0;
    this->totalSize = 0;
}

long int Save_Item_Proto::CheckPadding(long int Size)
{
    if (Size % 8)
    {
            Size = Size - (Size % 8) + 8;
    }

    return Size;
}

void Save_Item_Proto::Compress(const unsigned char *decompressedBuffer, int decompressedSize)
{
    lzo_init();
    lzo_voidp wrkmem = (lzo_voidp)malloc(LZO1X_999_MEM_COMPRESS);
    char *compressedBuffer = new char[COMP_BUF_CAL(decompressedSize)];

    lzo_uint compressedSize = 0;
    lzo1x_999_compress(decompressedBuffer, decompressedSize, (unsigned char *)compressedBuffer, &compressedSize, wrkmem);

    this->data = new char[compressedSize + 4];
    memcpy(this->data, (char *)"MCOZ", 4);
    memcpy(this->data + 4, compressedBuffer, compressedSize);

    this->compSize = compressedSize + 4;
    this->decompSize = decompressedSize;
    this->itemCount = decompressedSize / 152;
}

void Save_Item_Proto::Encrypt()
{
    IP_Header *IPh = new IP_Header;
    IPh->Header = 0x5850494D;
    IPh->Version = 1;
    IPh->SingleItemLength = 152;
    IPh->ItemCount = this->itemCount;
    IPh->LZObjectSize = this->encryptedDataSize + 16;

    this->encryptedDataSize = CheckPadding(this->compSize);
    this->LZObjectSize = this->encryptedDataSize + 16;
    this->totalSize = this->LZObjectSize + 20;

    char *finalBuffer = (char *)malloc(this->totalSize);
    char TEAkey[] = {0xA1, 0xA4, 0x02, 0x00, 0xAA, 0x15, 0x54, 0x04, 0xE7, 0x8B, 0x5A, 0x18, 0xAB, 0xD6, 0xAA, 0x01};

    m2crypt::encrypt(this->data, this->data, TEAkey, this->encryptedDataSize);

    memcpy(finalBuffer, IPh, 20);

    memcpy(finalBuffer + 20, (char *)"MCOZ", 4);
    memcpy(finalBuffer + 24, &this->encryptedDataSize, 4);
    memcpy(finalBuffer + 28, &this->compSize, 4);
    memcpy(finalBuffer + 32, &this->decompSize, 4);

    memcpy(finalBuffer + 36, this->data, this->encryptedDataSize);

    this->data = finalBuffer;
}

int Save_Item_Proto::GetTotalSize()
{
    return this->totalSize;
}

char *Save_Item_Proto::GetData()
{
    return this->data;
}
