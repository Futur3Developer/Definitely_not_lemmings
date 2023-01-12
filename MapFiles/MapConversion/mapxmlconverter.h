#ifndef MAPXMLCONVERTER_H
#define MAPXMLCONVERTER_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "mapconverter.h"

class MapXMLconverter: public MapConverter
{
public:
    MapXMLconverter();
    void save_map_to_file(QSharedPointer<QFile> map_file, Map *map) override;
    Map* load_map_from_file(QSharedPointer<QFile> map_file) override;

private:
    void save_map_to_XML(QSharedPointer<QXmlStreamWriter> writerXML, QString entrance_position, QString exit_position, QString blocks_positions);

    QList<QString> load_data_from_XML( QSharedPointer<QXmlStreamReader> readerXML);
    Map *prepare_map(QList<QString> map_components);
};

#endif // MAPXMLCONVERTER_H
