#include "server_attr.h"

ServerAttributeInfo::ServerAttributeInfo()
{
    this->data = NULL;
    this->SAI = new ServerAttrInfo;
}

ServerAttributeInfo::~ServerAttributeInfo()
{
    delete this->data;
    delete this->SAI;
}

bool ServerAttributeInfo::SaveServerAttribute(QString attrPath, int currentY, uint32_t &cmpSize)
{
    uint32_t size, compSize;
    char *attr, *temp, *header = (char *)malloc(4);

    size = IOReadFile(attrPath.toAscii().data(), this->data);

    if(size != 65542)
    {
        Syserr(QString("SERVER ATTRIBUTE INFO :: SAVE SERVER ATTRIBUTE :: Wrong file size [%0 != 65542]!").arg(QString::number(size)));
        return false;
    }

    if(!CheckHeader())
    {
        Syserr(QString("SERVER ATTRIBUTE INFO :: SAVE SERVER ATTRIBUTE :: Wrong file header!").arg(QString::number(size)));
        return false;
    }

    for(int x = 0; x < 4; x++)
    {
        temp = GetPaddedAttr(SplitInQuadrant(x, currentY));
        compSize = CompressData(temp, attr);
        cmpSize += compSize;
        memcpy(header, &compSize, 4);

        IOWriteFile("special\\development\\server_attr", header, 4, "ab");
        IOWriteFile("special\\development\\server_attr", attr, compSize, "ab");
    }

    return true;
}

bool ServerAttributeInfo::CheckHeader()
{
    char header[] = {0x4a, 0x0a, 0x00, 0x01, 0x00, 0x01};

    if(memcmp(header, this->data, 6))
        return false;

    this->data += 6;
    return true;
}

char *ServerAttributeInfo::SplitInQuadrant(int x, int y)
{
    char *temp;

    temp = (char *)malloc(0x1000);

    for(int i = 0; i < 64; i++)
    {
        memcpy(temp + i * 64, this->data + (64 * x) + (y * 0x4000) + (i * 256), 64);
    }

    temp = GetDoubleAttr(temp);

    return temp;
}

char *ServerAttributeInfo::GetDoubleAttr(char *attr)
{
    char *temp;

    temp = (char *)malloc(0x4000);

    for(int y = 0; y < 64; y++)
    {
        for(int x = 0; x < 64; x++)
        {
            temp[x * 2 + 1 + y * 256 + 128] = attr[x + y * 64];
            temp[x * 2 + y * 256 + 128] = attr[x + y * 64];
            temp[x * 2 + y * 256] = attr[x + y * 64];
            temp[x * 2 + 1 + y * 256] = attr[x + y * 64];
        }
    }

    return temp;
}

char *ServerAttributeInfo::GetPaddedAttr(char *attrFour)
{
    char *attrDivided = (char *)malloc(0x10000);
    memset(attrDivided, 0, 0x10000);

    for(int y = 0; y < 128; y++)
    {
        for(int x = 0; x < 128; x++)
        {
            attrDivided[(x + y * 128) * 4] = attrFour[x + y * 128];
        }
    }

    return attrDivided;
}

uint32_t ServerAttributeInfo::CompressData(char *decompAttr, char *&compAttr)
{
    lzo_init();
    lzo_voidp wrkmem = (lzo_voidp)malloc(LZO1X_999_MEM_COMPRESS);
    compAttr = (char *)malloc(COMP_BUF_CAL(65536));

    lzo_uint compressedSize = 0;
    lzo1x_999_compress((unsigned char *)decompAttr, 65536, (unsigned char *)compAttr, &compressedSize, wrkmem);

    free(decompAttr);
    free(wrkmem);

    return compressedSize;
}

bool ServerAttributeInfo::LoadServerAttribute(QString attrPath)
{
    uint32_t size;

    size = IOReadFile(attrPath.toAscii().data(), this->data);

    // Read X + Y
    SAI = (ServerAttrInfo *)this->data;
    data += 8;

    if(SAI->x == 0 || SAI->y == 0)
    {
        Syserr("SERVER ATTRIBUTE INFO :: LOAD SERVER ATTRIBUTE :: X or Y Size == 0!");
        return false;
    }

    Syslog(QString("SERVER ATTRIBUTE INFO :: LOAD SERVER ATTRIBUTE :: Load Attribute [SATRR->x = %1, SATRR->y = %2]!").arg(
               QString::number(SAI->x),
               QString::number(SAI->y)));

    if (!ReadNextAttr())
        return false;

    return true;
}

bool ServerAttributeInfo::ReadNextAttr()
{
    uint32_t arrSize = 0x4000 * (SAI->y * SAI->x);
    char *attr = (char *)malloc(arrSize);
    char *quad;

    memset(attr, 0, arrSize);

    for(int y = 0; y < SAI->y; y++)
    {
        for(int x = 0; x < SAI->x; x++)
        {
            quad = GetRealAttr(ReadNextAttrPart());

            //IOWriteFile(QString("special\\development\\mini\\%0 (%1_%2).raw").arg(
            //              QString::number((y * 4 + x)), QString::number(y), QString::number(x)).toAscii().data(), quad, 0x4000, (char*)"wb");

            for(int i = 0; i < 128; i++)
            {
                memcpy(attr + (128 * x) + (y * SAI->x * 128 * 128) + (i * SAI->x * 128), quad + i * 128, 128);
            }

            free(quad);
        }
    }

    IOWriteFile("special\\development\\FullAttribute.raw", attr, arrSize, "wb");

    free(attr);
    return true;
}

char *ServerAttributeInfo::GetRealAttr(char *attrFour)
{
    char *attrDivided = (char *)malloc(0x4000);

    for(int y = 0; y < 128; y++)
    {
        for(int x = 0; x < 128; x++)
        {
            attrDivided[x + y * 128] = attrFour[(x + y * 128) * 4];
        }
    }

    free(attrFour);
    return attrDivided;
}

char *ServerAttributeInfo::ReadNextAttrPart()
{
    uint32_t compSize, MAX_DECOMP_SIZE;
    lzo_uint decompSize;
    char *compData, *decompData;

    // Allocate memory
    MAX_DECOMP_SIZE = 0x10000 + (0x10000 >> 4) + 67 + 15;
    decompData = (char *)malloc(MAX_DECOMP_SIZE);

    // Read compressed data
    compSize = *(uint32_t *)this->data;
    compData = (char *)malloc(compSize);
    memcpy(compData, this->data + 4, compSize);
    this->data += compSize + 4;

    lzo1x_decompress((unsigned char *)compData, compSize, (unsigned char *)decompData, &decompSize, 0);

    if ( decompSize != 65536 )
    {
        Syserr(QString("SERVER ATTRIBUTE INFO :: READ NEXT QUADRANT :: Invalid decompressed Size = %0!").arg(QString::number(decompSize)));
        return NULL;
    }

    free(compData);
    return decompData;
}
