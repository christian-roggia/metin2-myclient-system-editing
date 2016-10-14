#ifndef SERVER_ATTR_H
#define SERVER_ATTR_H

#include <lzo/lzoconf.h>
#include <lzo/lzo1x.h>

#include "global.h"

#define COMP_BUF_CAL(in_buf_size) in_buf_size + in_buf_size / 16 + 64 + 3 + 4

struct ServerAttrInfo
{
    uint32_t x;
    uint32_t y;
};

class ServerAttributeInfo
{
public:
    ServerAttributeInfo();
    ~ServerAttributeInfo();

    bool LoadServerAttribute(QString attrPath);
    bool SaveServerAttribute(QString attrPath, int currentY, uint32_t &compSize);
    
private:
    char *GetPaddedAttr(char *attrFour);
    char *SplitInQuadrant(int x, int y);
    char *GetDoubleAttr(char *attr);
    bool CheckHeader();
    uint32_t CompressData(char *decompAttr, char *&compAttr);

    char *ReadNextAttrPart();
    bool ReadNextAttr();
    char *GetRealAttr(char *attrFour);

    ServerAttrInfo *SAI;
    char *data;
};

#endif // SERVER_ATTR_H
