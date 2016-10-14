#include "IO.h"
#include "syslog.h"

long int IOReadFile(QString FileName, char *&Buffer)
{
    FILE * pFile;
    uint32_t size;
    size_t result;

    pFile = fopen (FileName.toAscii().data(), "rb");
    if (pFile == NULL)
    {
        Syserr(QString("ITEMS INFO : READ FILE :: Impossible to open the file \"%0\"!").arg(QString(FileName)));
        return 0;
    }

    fseek(pFile, 0, SEEK_END);
    size = ftell (pFile);
    rewind(pFile);

    Buffer = (char*) malloc (size);
    if (Buffer == NULL)
    {
        Syserr("ITEMS INFO : READ FILE :: Cannot allocate memory!");
        return 0;
    }

    result = fread(Buffer, 1, size, pFile);
    if (result != size) {
        Syserr(QString("ITEMS INFO : READ FILE :: Impossible to read the file \"%0\"!").arg(QString(FileName)));
        return 0;
    }

    fclose (pFile);

    return size;
}

bool IOWriteFile(QString FileName, char *Buffer, int lSize, QString OpenMode)
{
    FILE *pFile;
    size_t result;

    pFile = fopen (FileName.toAscii().data(), OpenMode.toAscii().data());

    if (pFile == NULL)
    {
        Syserr(QString("ITEMS INFO : READ FILE :: Impossible to open the file \"%0\"!").arg(QString(FileName)));
        return false;
    }

    result = fwrite (Buffer, 1, lSize, pFile);
    if (result == 0) {
        Syserr(QString("ITEMS INFO : READ FILE :: Impossible to write the file \"%0\"!").arg(QString(FileName)));
        return false;
    }

    fclose(pFile);
    return true;
}

