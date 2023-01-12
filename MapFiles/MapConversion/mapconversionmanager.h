#ifndef MAPCONVERSIONMANAGER_H
#define MAPCONVERSIONMANAGER_H

#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>

#include "MapFiles/map.h"
#include "MapFiles/mapcreator.h"

class MapConversionManager
{
public:
    MapConversionManager(const MapConversionManager&) = delete;

    static MapConversionManager& Get()
    {
        static MapConversionManager MapConversionManager_Instance;
        return MapConversionManager_Instance;
    }

    void process_map_saving_request(MapCreator *map_creator);
    Map* process_map_loading_request(QWidget *caller);

private:
    MapConversionManager();

    QStringList *file_types_supported;
    const QStringList *const_file_types_supported;
    QWidget *caller;

    bool assert_that_map_can_be_saved_in_default_directory(QFileInfo user_maps_directory);
    QString get_map_file_path(QFileDialog::FileMode file_mode, QFileDialog::AcceptMode accept_mode, QString window_title, QString user_maps_directory_path);
    bool assert_that_map_file_path_is_valid(QString map_file_path, QSharedPointer<QFile> map_file, QString warning_for_file_not_selected, QString warning_for_file_without_access);
};

#endif // MAPCONVERSIONMANAGER_H
