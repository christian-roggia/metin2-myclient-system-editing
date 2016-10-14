#include "tabs/MDATR.h"

QList<CMADTR> *MDATR::LMDATR;

MDATR::MDATR()
{
    this->LMDATR = new QList<CMADTR>();
}

void MDATR::LoadMATR(QString MDATRPath)
{
    uint32_t size;
    char *data;
    char header[14];

    size = IOReadFile(MDATRPath.toAscii().data(), data);
    memcpy(header, data, 14);
    data += 14;

    if(CheckHeader(header))
    {
        Syserr("MDATR::LoadMATR : Invalid MDATR header ("+QString(header)+" != AttributeData)");
        return;
    }

    this->collisions = *(uint32_t *)data;
    this->heights = *(uint32_t *)(data + 4);
    data += 8;

    for(int i = 0; i < this->collisions; i++)
    {
        this->tempCMADTR = new CMADTR();
        this->tempCMADTR->collType = *(uint32_t *)data;
        memcpy(this->tempCMADTR->name, data + 4, 32);
        data += 36;

        this->tempCMADTR->X = *(float *)data;
        this->tempCMADTR->Y = *(float *)(data + 4);
        this->tempCMADTR->Z = *(float *)(data + 8);
        data += 12;

        switch(this->tempCMADTR->collType)
        {
        case 1:
            this->tempCMADTR->unk1 = *(float *)data;
            this->tempCMADTR->unk2 = *(float *)(data + 4);
            this->tempCMADTR->unk3 = *(float *)(data + 8);
            data += 12;
            break;
        case 2:
            this->tempCMADTR->unk1 = *(float *)data;
            data += 4;
            break;
        case 0:
        case 3:
            this->tempCMADTR->unk1 = *(float *)data;
            this->tempCMADTR->unk2 = *(float *)(data + 4);
            data += 8;
            break;
        default:
            break;
        }

        this->tempCMADTR->f1 = *(float *)data;
        this->tempCMADTR->f2 = *(float *)(data + 4);
        this->tempCMADTR->f3 = *(float *)(data + 8);
        this->tempCMADTR->f4 = *(float *)(data + 12);
        data += 16;

        LMDATR->append(*this->tempCMADTR);
        delete this->tempCMADTR;
    }

    Syslog("MDATR::LoadMATR : Loaded "+QString::number(this->collisions)+" collision(s) | "+QString::number(this->heights)+" height(s)");
}

bool MDATR::CheckHeader(char *header)
{
    return (bool)strcmp(header, "AttributeData");
}
