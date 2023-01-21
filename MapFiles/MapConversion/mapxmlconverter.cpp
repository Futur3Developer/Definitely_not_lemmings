#include "mapxmlconverter.h"
#include <QDebug>

MapXMLconverter::MapXMLconverter(){}

void MapXMLconverter::save_map_to_XML(QSharedPointer<QXmlStreamWriter> writerXML, QString entrance_position, QString exit_position, QString blocks_positions)
{
    writerXML->writeStartDocument();
    writerXML->setAutoFormatting(true);
    writerXML->setAutoFormattingIndent(4);
    writerXML->writeStartElement("map_configuration");
    writerXML->writeTextElement("entrance", entrance_position);
    writerXML->writeTextElement("exit", exit_position);
    writerXML->writeTextElement("blocks", blocks_positions);
    writerXML->writeEndElement();
    writerXML->writeEndDocument();
}

QList<QString> MapXMLconverter::load_data_from_XML( QSharedPointer<QXmlStreamReader> readerXML)
{
    QList<QString> map_components;

    readerXML->readNextStartElement();
    while(readerXML->name() != "map_configuration" && readerXML->error() == 0)
    {
        readerXML->readNextStartElement();
    }

    if(readerXML->name() == "map_configuration")
    {
        readerXML->readNextStartElement();
        if(readerXML->name() == "entrance")
        {
            map_components.append(readerXML->readElementText());
            readerXML->readNextStartElement();

            if(readerXML->name() == "exit" )
            {
                map_components.append(readerXML->readElementText());
                readerXML->readNextStartElement();

                if(readerXML->name() == "blocks")
                {
                    map_components.append(readerXML->readElementText());
                }
            }
        }
    }

    return map_components;
}

void MapXMLconverter::save_map_to_file(QSharedPointer<QFile> map_file, Map* map)
{
    QList<QGraphicsItem*> components_list = map -> items(0, 0, map -> get_width(), map -> get_height() - 230, Qt::ContainsItemShape, Qt::DescendingOrder, QTransform());

    QString entrance_position;
    QString exit_position;
    QString blocks_positions;

    foreach(const QGraphicsItem* component, components_list)
    {
        if(component -> type() == Block::Type)
        {
            blocks_positions += QString::number(component -> pos().x()) + "," + QString::number(component -> pos().y()) + ";";

        }
        else if(component -> type() == Exit::Type)
        {
            exit_position = QString::number(component -> pos().x()) + "," + QString::number(component -> pos().y());
        }
        else
        {
            entrance_position = QString::number(component -> pos().x()) + "," + QString::number(component -> pos().y());
        }
    }

//    foreach(int lemmings_class_changes_available, map -> available_lemmings_class_changes_list)
//    {

//    }

    blocks_positions.chop(1); //Removing last ";"

    QSharedPointer<QXmlStreamWriter> writerXML = QSharedPointer<QXmlStreamWriter>(new QXmlStreamWriter(map_file.data()));
    save_map_to_XML(writerXML, entrance_position, exit_position, blocks_positions);
}

Map *MapXMLconverter::load_map_from_file(QSharedPointer<QFile> map_file)
{
    QSharedPointer<QXmlStreamReader> readerXML = QSharedPointer<QXmlStreamReader>(new QXmlStreamReader(map_file.data()));

    Map* dummy_map = nullptr;
    QList<QString> map_components;

    map_components = load_data_from_XML(readerXML);

    if(map_components.isEmpty())
        return dummy_map;
    else
        if(map_components.length() < 3)
            return dummy_map;

    Map *map = prepare_map(map_components);

    return map;
}

Map *MapXMLconverter::prepare_map(QList<QString> map_components)
{
    Map* map = new Map();
    QStringList entrance_position_split = map_components[0].split(',');
    QStringList exit_position_split = map_components[1].split(',');
    QStringList blocks_positions_split = map_components[2].split(';');

    map -> add_entrance(entrance_position_split[0].toInt(), entrance_position_split[1].toInt());
    map -> add_exit(exit_position_split[0].toInt(), exit_position_split[1].toInt());

    for(int i = 0; i < blocks_positions_split.length() - 1; i++)
    {
        QStringList temp = blocks_positions_split[i].split(',');
        map -> add_block(temp[0].toInt(), temp[1].toInt());
    }

    return map;
}
