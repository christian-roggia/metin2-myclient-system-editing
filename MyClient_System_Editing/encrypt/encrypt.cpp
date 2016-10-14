#include "encrypt.h"

using namespace std;

void m2crypt::encrypt(char *outBuffer, char *inData, char *lzoData, size_t dwSize)
{
        if (dwSize % 8)
        {
                dwSize = dwSize - (dwSize % 8) + 8;
        }

        int steps, i;
        steps = dwSize / 8;
        char *outData = outBuffer;

        for (i = 0; i < steps; i++)
        {
                innerEncrypt(*(LPDWORD)(inData + 4), *(LPDWORD)inData, lzoData, outData);
                inData += 8;
                outData += 8;
        }
        return;
}

void m2crypt::innerEncrypt(DWORD first, DWORD second, char *lzKey, char *outBuffer)
{
    uint32_t delta = 0x61C88647, sum = 0;

    for (int i = 0; i < 32; i++)
    {
        second += (sum + *(LPDWORD)&lzKey[4 * (sum & 3)]) ^ (first + (16 * first ^ (first >> 5)));
        sum -= delta;
        first += (sum + *(LPDWORD)&lzKey[4 * ((sum >> 11) & 3)]) ^ (second + (16 * second ^ (second >> 5)));
    }

    *(LPDWORD)outBuffer = second;
    *(LPDWORD)(outBuffer + 4) = first;
}
