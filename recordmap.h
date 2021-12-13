#ifndef RECORDMAP_H
#define RECORDMAP_H

#include <QMap>
#include <QList>
#include <QString>


typedef QList<QString> RecordList;


class RecordMap
{
public:
    RecordMap();
    ~RecordMap() = default;

    bool isKeyContained(const QString &key) const;
    const RecordList getRecordList(const QString &key) const;

    void addRecord(const QString &key, const QString &value);
    void addRecord(const QString &key, const RecordList &list);
    void removeRecord(const QString &key, const QString &value);
    void removeRecord(const QString &key, const RecordList &list);

    void serializeRecord(const QString &filename);
    void deserializeRecord(const QString &filename);

private:
    QMap<QString, RecordList> map;
};

#endif // RECORDMAP_H
