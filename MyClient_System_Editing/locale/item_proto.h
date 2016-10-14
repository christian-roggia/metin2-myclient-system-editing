#ifndef EINDEX_H
#define EINDEX_H

#include "encrypt/crypt.h"
#include "encrypt/encrypt.h"
#include "common/syslog.h"
#include <lzo/lzoconf.h>
#include <memory.h>
#include <malloc.h>

#define COMP_BUF_CAL(in_buf_size) in_buf_size + in_buf_size / 16 + 64 + 3 + 4

enum IType
{
    Weapon = 1,
    Armor = 2,
};

enum IApplyType
{
    SkillSpeed = 7,
};

enum ISubType
{
    Sword = 0,
    Dagger = 1,
    Bow = 2,
    BroadSword = 3,
    Bell = 4,
    Fan = 5,
};

class Load_Item_Proto
{
public:
        Load_Item_Proto(char *, uint32_t);
        ~Load_Item_Proto();

        bool CheckInfo();
        bool Decrypt();
        bool Decompress();

        char *GetData();
        int GetItemCount();

private:
        char *data;
        uint32_t itemCount, size, dSize, cSize, hItemCount;
};

class Save_Item_Proto
{
public:
        Save_Item_Proto();
        ~Save_Item_Proto();
        void Compress(const unsigned char *, int);
        void Encrypt();

        char *GetData();
        int GetTotalSize();

private:
        long int CheckPadding(long int Size);

        char *data;
        long int compSize;
        long int decompSize;
        long int encryptedDataSize;
        long int itemCount;
        long int LZObjectSize;
        long int totalSize;
};

#endif // EINDEX_H
