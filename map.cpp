#include "map.h"
#include "mapcreator.h"

#include <QDebug>

Map::Map()
{
    this->setSceneRect(0,0,map_width,map_height);
}

int Map::get_width() const
{
    return map_width;
}

int Map::get_height() const
{
    return map_height;
}

void Map::save_map_to_XML()
{
    QString user_maps_directory_path = qApp -> applicationDirPath() + "/" + "UserMaps";

    const QFileInfo user_maps_directory(user_maps_directory_path);
    if ((!user_maps_directory.exists()) || (!user_maps_directory.isDir()) || (!user_maps_directory.isWritable()))
    {
        if(user_maps_directory.exists())
        {
            QMessageBox::warning(&MapCreator::Get(), NULL, "Path for users maps " + user_maps_directory_path + " exists, but is not a directory or is not writable");
            return;
        }
        else
        {
            QDir directory(user_maps_directory_path);
            bool directory_created = directory.mkpath("user_maps_directory");
            if(!(directory_created))
            {
                QMessageBox::warning(&MapCreator::Get(), NULL, "Path for users maps " + user_maps_directory_path + " does not exist and can't be created.");
                return;
            }
        }

    }

    int map_file_number = 0;
    QString user_map_file_name_format = user_maps_directory_path + "/" + "user_map_";
    QString user_map_file_name =  user_map_file_name_format + QString::number(map_file_number) + ".xml";
    QFile new_map_file(user_map_file_name);

    //TODO: It would be better to save in another file what is the highest number of created map if there is going to be a lot of them.
    while(new_map_file.exists())
    {
        map_file_number += 1;
        user_map_file_name = user_map_file_name_format + QString::number(map_file_number) + ".xml";
        new_map_file.setFileName(user_map_file_name);
    }

    QSharedPointer<QFile> final_map_file = QSharedPointer<QFile>(new QFile(user_map_file_name));

    qDebug()<<user_map_file_name;

    if (!(final_map_file->open(QIODevice::ReadWrite)))
    {
        QMessageBox::warning(&MapCreator::Get(), NULL, "Map file " + user_map_file_name + "  could not be created.");
        return;
    }


    QList<QGraphicsItem*> components_list = this -> items(0, 0, map_width, map_height - 230, Qt::ContainsItemShape, Qt::DescendingOrder, QTransform());

    QString entrance_position;
    QString exit_position = 0;
    QString blocks_positions = 0;
    Q_FOREACH(const QGraphicsItem* component, components_list)
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

    QSharedPointer<QXmlStreamWriter> writerXML = QSharedPointer<QXmlStreamWriter>(new QXmlStreamWriter(final_map_file.data()));
    writerXML->writeStartDocument();
    writerXML->setAutoFormatting(true);
    writerXML->setAutoFormattingIndent(4);
    writerXML->writeStartElement("map_configuration");
    writerXML->writeTextElement("entrance", entrance_position);
    writerXML->writeTextElement("exit", exit_position);
    writerXML->writeTextElement("blocks", blocks_positions);
    writerXML->writeEndElement();
    writerXML->writeEndDocument();

//    QList<QGraphicsItem*> components_list = this -> items(0, 0, map_width, map_height - 230, Qt::ContainsItemShape, Qt::DescendingOrder, QTransform());

//    Q_FOREACH(const QGraphicsItem* component, components_list)
//    {
//        QSharedPointer<QFile> final_file = QSharedPointer<QFile>(new QFile(absolute_configuration_path));


//        if (!(final_file->open(QIODevice::ReadWrite)))
//        {

//        }

//        QSharedPointer<QXmlStreamWriter> writerXML = QSharedPointer<QXmlStreamWriter>(new QXmlStreamWriter(final_file.data()));

//        model_configuration = absolute_configuration_path;

//        writerXML->writeStartDocument();

//        writerXML->setAutoFormatting(true);
//        writerXML->setAutoFormattingIndent(4);

//        writerXML->writeStartElement("model_configuration");
//        writerXML->writeTextElement("model_json", temp);

//        readerXML->readNextStartElement();
//        // In the loop below configuration file will be created based on file that holds the model data
//        while(!(readerXML->atEnd()))
//        {
//            temp = readerXML->name().toString();
//            QString buff = readerXML->readElementText();

//            if(buff.length() != 0) //Do not copy empty start elements
//            {
//                writerXML->writeTextElement(temp,buff);
//            }
//            readerXML->readNextStartElement();
//        }
//        writerXML->writeEndElement(); // model_configuration
//        writerXML->writeEndDocument();
//    }
}

void Map::load_map_from_XML()
{

}



