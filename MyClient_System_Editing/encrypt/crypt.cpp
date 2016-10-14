#include "crypt.h"

LZObject::LZObject(char *eData, uint32_t *eKey, uint32_t eSize)
{
    this->data = eData;
    this->key = eKey;
    this->dSize = eSize;
}

LZObject::~LZObject()
{
    delete [] this->data;
    delete [] this->key;
    this->dSize = 0;
}

void LZObject::TEADecrypt(uint32_t *v, uint32_t *k)
{
    uint32_t sum = 0xC6EF3720, delta = 0x61C88647;
    uint32_t v0 = v[0], v1 = v[1];

	for (int i = 0; i < 32; i++)
	{
        v1 -= (v0 + (16 * v0 ^ (v0 >> 5))) ^ (sum + k[(sum >> 11) & 3]);
        sum += delta;
        v0 -= (v1 + (16 * v1 ^ (v1 >> 5))) ^ (sum + k[sum & 3]);
	}

    v[0] = v0; v[1] = v1;
}

void LZObject::ObjectDecrypt()
{
    int steps;
    uint32_t *uiData = reinterpret_cast<uint32_t *>(this->data);

    if (this->dSize % 8)
    {
        this->dSize = this->dSize - (this->dSize % 8) + 8;
    }

    steps = this->dSize / 8;

    for (int i = 0; i < steps; i++)
    {
        TEADecrypt(&uiData[i * 2], this->key);
    }
}

char *LZObject::GetDecryptedData()
{
    return this->data;
}

int LZObject::GetDataSize()
{
    return this->dSize;
}
