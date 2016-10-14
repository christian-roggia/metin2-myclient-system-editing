#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <windows.h>
#include <iostream>
#include <fstream>
#include <QLibrary>
#include <lzo/lzo1x.h>
#include <stdint.h>

namespace m2crypt {
    void encrypt(char *, char *, char *, size_t);
    void innerEncrypt(DWORD, DWORD, char *, char *);
}

#endif // ENCRYPT_H
