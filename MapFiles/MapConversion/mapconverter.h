#ifndef MAPCONVERTER_H
#define MAPCONVERTER_H

#include "mapconversionmanager.h"

/**
 * Abstract class to be inherited by classes implementing serialization and deserialization of Map instances for particular file types.
 */
class MapConverter
{
public:
    MapConverter();
    virtual ~MapConverter() = default;
    virtual void save_map_to_file(QSharedPointer<QFile> map_file, Map *map);
    virtual Map* load_map_from_file(QSharedPointer<QFile> map_file);
};

#endif // MAPCONVERTER_H
