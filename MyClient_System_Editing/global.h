#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QTextCodec>
#include <QList>
#include <QFile>
#include <fstream>
#include <malloc.h>
#include <QStringList>
#include <QRegExp>
#include <stdint.h>

#include "common/IO.h"
#include "common/syslog.h"

#ifndef CONFIGFILE_H
#include "config/ConfigFile.h"
static CConfigFile *g_configs;
#endif

#ifndef ITEMSINFO_H
#include "tabs/items.h"
static ItemsInfo *MyItemsInfo = new ItemsInfo;
#endif

#ifndef FISHING_H
#include "tabs/fishing.h"
static Fishing *fish;
#endif

#ifndef MANAGER_H
#include <tabs/group.h>
static GroupInfo *gInfo;
#endif

#ifndef SERVER_ATTR_H
#include <tabs/server_attr.h>
#endif

#ifndef MOBDROPITEM_H
#include <tabs/mobdropitem.h>
static MobDropItem *mdItem;
#endif

#ifndef MDATR_H
#include "tabs/MDATR.h"
static MDATR *mdatr;
#endif

#ifndef SPECIALITEMGROUP_H
#include "tabs/special_item_group.h"
static SpecialItemGroup *SIGGroups;
#endif

#endif // GLOBAL_H
