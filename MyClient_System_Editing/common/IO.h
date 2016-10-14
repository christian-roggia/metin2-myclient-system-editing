#ifndef IO_H
#define IO_H

#include <iostream>
#include <fstream>
#include <malloc.h>
#include <common/syslog.h>
#include <stdint.h>

long int IOReadFile(QString FileName, char *&Buffer);
bool IOWriteFile(QString FileName, char *Buffer, int lSize, QString OpenMode);

#endif // IO_H
