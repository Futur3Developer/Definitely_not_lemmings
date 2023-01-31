#ifndef MAPXMLCONVERTER_H
#define MAPXMLCONVERTER_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "mapconverter.h"
/**
 * Child class of MapConverter implementing serialization and deserialization of Map instances to/from .xml files.
 */
class MapXMLconverter: public MapConverter
{
public:
    MapXMLconverter();
    ~MapXMLconverter();
    /**
     * Saves Map instance to .xml file.
     * To be more specific it performs data serialization and calls save_map_to_XML to handle writing it to file.
     * @param map_file - file where Map is to be saved.
     * @param map - pointer to Map instance to be saved.
     */
    void save_map_to_file(QSharedPointer<QFile> map_file, Map *map) override;

    /**
     * Loads Map instance from .xml file.
     * @param map_file - file from which data is to be loaded into Map instance.
     * @return - returns valid Map instance if there were no errors. Otherwise returns nullptr to be handled by caller.
     */
    Map* load_map_from_file(QSharedPointer<QFile> map_file) override;

private:
    /**
     * Writes Map instance serialized data into .xml file.
     */
    void save_map_to_XML(QSharedPointer<QXmlStreamWriter> writerXML, QString entrance_position, QString exit_position,
                         QString blocks_positions, QString lemmings_class_changes_available, QString lemmings_survival_rate);

    /**
     * Loads data from XML file into QStringList.
     * @return - returns pointer to Map instance data as QStringList if it was properly serialized file. Otherwise returns nullptr to be handled by caller.
     */
    QStringList *load_data_from_XML( QSharedPointer<QXmlStreamReader> readerXML);

    /**
     * Prepares Map instance using QStringList *map_components.
     * @return - returns pointer to Map instance. It does not return nullptr even if resulting Map instance will be empty.
     */
    Map *prepare_map();

    /**
     * Pointer to Map instance data to be deserialized.
     */
    QStringList *map_components;
};

#endif // MAPXMLCONVERTER_H
