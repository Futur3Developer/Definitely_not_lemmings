#include "mapxmlconverter.h"
#include "qdebug.h"

MapXMLconverter::MapXMLconverter()
{
    map_components = new QStringList;
}

MapXMLconverter::~MapXMLconverter()
{
    delete map_components;
}

void MapXMLconverter::save_map_to_XML(QSharedPointer<QXmlStreamWriter> writerXML, QString entrance_position, QString exit_position,
                                      QString blocks_positions, QString lemmings_class_changes_available, QString lemmings_survival_rate)
{
    writerXML->writeStartDocument();
    writerXML->setAutoFormatting(true);
    writerXML->setAutoFormattingIndent(4);
    writerXML->writeStartElement("map_configuration");
    writerXML->writeTextElement("entrance", entrance_position);
    writerXML->writeTextElement("exit", exit_position);
    writerXML->writeTextElement("blocks", blocks_positions);
    writerXML->writeTextElement("lemmings_class_changes_available", lemmings_class_changes_available);
    writerXML->writeTextElement("lemmings_survival_rate", lemmings_survival_rate);
    writerXML->writeEndElement();
    writerXML->writeEndDocument();
}

QStringList *MapXMLconverter::load_data_from_XML( QSharedPointer<QXmlStreamReader> readerXML)
{
    QStringList *dummy_map_components = nullptr;

    readerXML->readNextStartElement();
    while(readerXML->name() != "map_configuration" && readerXML->error() == 0)
    {
        readerXML->readNextStartElement();
    }

    if(readerXML->name() == "map_configuration")
    {
        readerXML->readNextStartElement();
    }
    else {return dummy_map_components;};

    if(readerXML->name() == "entrance")
    {
        map_components->append(readerXML->readElementText());
        readerXML->readNextStartElement();
    }
    else {return dummy_map_components;};

    if(readerXML->name() == "exit")
    {
        map_components->append(readerXML->readElementText());
        readerXML->readNextStartElement();
    }
    else {return dummy_map_components;};

    if(readerXML->name() == "blocks")
    {
        //In case someone really wants Map with Entrance and Exit only
        QString temp = readerXML -> readElementText();
        if(temp.isEmpty())
            map_components->append("null");
        else
            map_components->append(temp);

        readerXML->readNextStartElement();
    }
    else {return dummy_map_components;};

    if(readerXML->name() == "lemmings_class_changes_available")
    {
        map_components->append(readerXML->readElementText());
        readerXML->readNextStartElement();
    }
    else {return dummy_map_components;};

    if(readerXML->name() == "lemmings_survival_rate")
    {
        map_components->append(readerXML->readElementText());
        readerXML->readNextStartElement();
    }
    else {return dummy_map_components;};
    
    return map_components;
}

void MapXMLconverter::save_map_to_file(QSharedPointer<QFile> map_file, Map* map)
{
    QList<QGraphicsItem*> components_list = map -> items(0, 0, map -> get_width(), map -> get_gui_panel_boundary_y_pos(), Qt::ContainsItemShape, Qt::DescendingOrder, QTransform());

    QString entrance_position;
    QString exit_position;
    QString blocks_positions;
    QString lemmings_class_changes_available_to_text;
    QString lemmings_survival_rate;

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

    foreach(int lemmings_class_changes_available, map -> available_lemmings_class_changes)
        lemmings_class_changes_available_to_text += QString::number(lemmings_class_changes_available) + ";";

    lemmings_survival_rate = QString::number(map ->lemmings_to_spawn) + ";" + QString::number(map ->lemmings_to_save);

    //Removing last ";"
    blocks_positions.chop(1);
    lemmings_class_changes_available_to_text.chop(1);

    QSharedPointer<QXmlStreamWriter> writerXML = QSharedPointer<QXmlStreamWriter>(new QXmlStreamWriter(map_file.data()));
    save_map_to_XML(writerXML, entrance_position, exit_position, blocks_positions, lemmings_class_changes_available_to_text,
                    lemmings_survival_rate);
}

Map *MapXMLconverter::load_map_from_file(QSharedPointer<QFile> map_file)
{
    QSharedPointer<QXmlStreamReader> readerXML = QSharedPointer<QXmlStreamReader>(new QXmlStreamReader(map_file.data()));

    Map* dummy_map = nullptr;

    if(load_data_from_XML(readerXML) == nullptr)
        return dummy_map;


    Map *map = prepare_map();

    return map;
}

Map *MapXMLconverter::prepare_map()
{
    Map* map = new Map();

    QStringList entrance_position_split = map_components->at(0).split(',');
    QStringList exit_position_split = map_components->at(1).split(',');
    QStringList blocks_positions_split = map_components->at(2).split(';');
    QStringList lemmings_class_changes_available = map_components->at(3).split(';');
    QStringList lemmings_survival_rate = map_components->at(4).split(';');

    map -> add_entrance(entrance_position_split[0].toInt(), entrance_position_split[1].toInt());
    map -> add_exit(exit_position_split[0].toInt(), exit_position_split[1].toInt());

    if(map_components->at(2) != "null")
    {
        for(int i = 0; i < blocks_positions_split.length(); i++)
        {
            QStringList temp = blocks_positions_split[i].split(',');
            map -> add_block(temp[0].toInt(), temp[1].toInt());
        }
    }

    for(int i =0; i < lemmings_class_changes_available.length(); i++)
        map -> available_lemmings_class_changes.append(lemmings_class_changes_available.at(i).toInt());

    map -> lemmings_to_spawn = lemmings_survival_rate.at(0).toInt();
    map -> lemmings_to_save = lemmings_survival_rate.at(1).toInt();

    map_components->clear();
    return map;
}
