#ifndef CRYPT_H
#define CRYPT_H

#include <iostream>
#include <lzo/lzo1x.h>
#include <stdint.h>

#define MCOZ 0x5A4F434D
#define MIPX 0x5850494D
#define MMPT 0x54504D4D
#define MIPT 0x5450494D
#define EPKD 0x444b5045

#pragma pack(push)
#pragma pack(1)

typedef struct
{
        uint32_t Header;
        uint32_t DecryptedBlockSize;
        uint32_t CompressedBlockSize;
        uint32_t DecompressedBlockSize;
}
LZObjectHeader;

struct IP_Header
{
    uint32_t Header;
    uint32_t Version;
    uint32_t SingleItemLength;
    uint32_t ItemCount;
    uint32_t LZObjectSize;
};

typedef struct
{
    uint32_t vnum;
    char gb2312[25];
    char name[25];
    uint8_t type;
    uint8_t subtype;
    uint8_t weight;
    uint8_t size;
    uint32_t antiflag;
    uint32_t flag;
    uint32_t wearflag;
    uint32_t immuneflag;
    uint32_t gold;
    uint32_t buy_price;
    uint8_t limittype0;
    uint32_t limitvalue0;
    uint8_t limittype1;
    uint32_t limitvalue1;
    uint8_t applytype0;
    uint32_t applyvalue0;
    uint8_t applytype1;
    uint32_t applyvalue1;
    uint8_t applytype2;
    uint32_t applyvalue2;
    uint32_t value0;
    uint32_t value1;
    uint32_t value2;
    uint32_t value3;
    uint32_t value4;
    uint32_t value5;
    uint16_t socket0;
    uint16_t socket1;
    uint16_t socket2;
    uint16_t socket3;
    uint16_t socket4;
    uint16_t socket5;
    uint32_t refine_vnum;
    uint16_t refine_set;
    uint8_t magic_pct;
    uint8_t specular;
    uint8_t socket_pct;
}
ip_struct;

#pragma pack(pop)

class LZObject
{
public:
    LZObject(char *, uint32_t *, uint32_t);
    ~LZObject();
    void ObjectDecrypt();
    char *GetDecryptedData();
    int GetDataSize();
private:
    void TEADecrypt(uint32_t *, uint32_t *);
    char *data;
    uint32_t *key, dSize;
};

#endif // CRYPT_H
