#include "syslog.h"

void Syserr(QString Log)
{
    time_t rawtime;
    struct tm * timeinfo;
    FILE *fSys;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    Log = QString("%0:%1:%2 %4\\%5\\%6 :: %7\n").arg(QString::number(timeinfo->tm_hour), QString::number(timeinfo->tm_min),
                                                 QString::number(timeinfo->tm_sec), QString::number(timeinfo->tm_mday),
                                                 QString::number(timeinfo->tm_mon), QString::number(timeinfo->tm_year + 1900), Log);

    fSys = fopen ("logs\\syserr.txt", "ab");
    fwrite (Log.toAscii().data(), 1, Log.length(), fSys);
    fclose(fSys);
}

void Syslog(QString Log)
{
    time_t rawtime;
    struct tm * timeinfo;
    FILE *fSys;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    Log = QString("%0:%1:%2 %4\\%5\\%6 :: %7\n").arg(QString::number(timeinfo->tm_hour), QString::number(timeinfo->tm_min),
                                                 QString::number(timeinfo->tm_sec), QString::number(timeinfo->tm_mday),
                                                 QString::number(timeinfo->tm_mon), QString::number(timeinfo->tm_year + 1900), Log);

    fSys = fopen ("logs\\syslog.txt", "ab");
    fwrite (Log.toAscii().data(), 1, Log.length(), fSys);
    fclose(fSys);
}
