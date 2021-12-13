#include "recordmap.h"
#include <QFile>
#include <QDataStream>
#include <algorithm>


RecordMap::RecordMap()
{
    map.clear();
}


bool RecordMap::isKeyContained(const QString &key) const
{
    return map.contains(key);
}

const RecordList RecordMap::getRecordList(const QString &key) const
{
    return map.value(key);
}


void RecordMap::addRecord(const QString &key, const QString &value)
{
    if (std::find(map[key].begin(), map[key].end(), value) == map[key].end()) {
        map[key].append(value);
    }
}

void RecordMap::addRecord(const QString &key, const RecordList &list)
{
    std::for_each(list.begin(), list.end(), [&](const QString &value) {
        this->addRecord(key, value);
    });
}

void RecordMap::removeRecord(const QString &key, const QString &value)
{
    auto removeIt = std::find(map[key].begin(), map[key].end(), value);
    if (removeIt != map[key].end()) {
        map[key].erase(removeIt);
    }
}

void RecordMap::removeRecord(const QString &key, const RecordList &list)
{
    std::for_each(list.begin(), list.end(), [&](const QString &value) {
        this->removeRecord(key, value);
    });
}


void RecordMap::serializeRecord(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QDataStream out(&file);
    std::for_each (map.keyBegin(), map.keyEnd(), [&](const QString &key) {
        out << key;
        out << map.value(key);
    });
}

void RecordMap::deserializeRecord(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&file);
    QString key;
    RecordList value;
    while (!file.atEnd()) {
        in >> key;
        in >> value;
        this->addRecord(key, value);
    }
}
