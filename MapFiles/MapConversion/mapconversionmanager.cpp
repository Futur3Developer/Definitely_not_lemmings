#include "mapconversionmanager.h"
#include "mapconverter.h"
#include "mapxmlconverter.h"

MapConversionManager::MapConversionManager()
{
    file_types_supported = new QStringList;

    /*
     * Support of runtime search of supported map file types based on existing MapConverter child classes was considered as an option.
     * The idea was deemed unnecessary and dropped but the arrangements stayed.
    */
    file_types_supported -> append("*.xml");
    const_file_types_supported = file_types_supported;
}

void MapConversionManager::process_map_saving_request(MapCreator* map_creator)
{
    this -> caller = map_creator;

    QString backup_directory_path = qApp -> applicationDirPath();
    QString user_maps_directory_path = backup_directory_path + "/UsersMaps";
    const QFileInfo user_maps_directory(user_maps_directory_path);

    if ((!user_maps_directory.exists()) || (!user_maps_directory.isDir()) || (!user_maps_directory.isWritable()))
    {
        bool default_users_maps_directory_is_prepared = assert_that_map_can_be_saved_in_default_directory(user_maps_directory);

        if(!(default_users_maps_directory_is_prepared))
            user_maps_directory_path = backup_directory_path;
    }

    QString map_file_path = get_map_file_path(QFileDialog::AnyFile, QFileDialog::AcceptSave, "Map_Saving", user_maps_directory_path);

    QString warning_for_map_file_not_selected = "Map file was not created.";
    QString warning_for_map_file_without_access = "Map file " + map_file_path + "  could not be created.";

    QSharedPointer<QFile> map_file = QSharedPointer<QFile>(new QFile(map_file_path));
    bool map_file_path_is_valid = assert_that_map_file_path_is_valid(map_file_path, map_file, warning_for_map_file_not_selected, warning_for_map_file_without_access);

    if(!(map_file_path_is_valid))
        return;

    MapConverter *map_converter = new MapXMLconverter;
    map_converter -> save_map_to_file(map_file, map_creator -> get_map());
    delete map_converter;
}

bool MapConversionManager::assert_that_map_can_be_saved_in_default_directory(QFileInfo user_maps_directory)
{
    if(user_maps_directory.exists())
    {
        QMessageBox::warning(this -> caller, NULL, "Path for users maps " + user_maps_directory.absoluteFilePath() + " exists, but is not a directory or is not writable.");
        return false;
    }
    else
    {
        QDir directory(qApp -> applicationDirPath());
        bool directory_created = directory.mkpath("UserMaps");
        if(!(directory_created))
        {
            QMessageBox::warning(this -> caller, NULL, "Directory for users maps " + user_maps_directory.absoluteFilePath() + " does not exist and can't be created.");
            return false;
        }
    }
    return true;
}

QString MapConversionManager::get_map_file_path(QFileDialog::FileMode file_mode, QFileDialog::AcceptMode accept_mode, QString window_title, QString user_maps_directory_path)
{
    QString map_file_path;

    QFileDialog dialog(this -> caller);
    dialog.setFileMode(file_mode);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(accept_mode);
    dialog.setDirectory(user_maps_directory_path);
    dialog.setNameFilters(*const_file_types_supported);
    dialog.setWindowTitle(window_title);

    QStringList file_paths = QStringList();

    if (dialog.exec())
        file_paths = dialog.selectedFiles();

    if (file_paths.size() > 0)
        map_file_path = file_paths.at(0);

    return map_file_path;
}

Map *MapConversionManager::process_map_loading_request(QWidget *caller)
{
    this -> caller = caller;

    QString backup_directory_path = qApp -> applicationDirPath();
    QString user_maps_directory_path = backup_directory_path + "/UserMaps";
    const QFileInfo user_maps_directory(user_maps_directory_path);

    if ((!user_maps_directory.exists()) || (!user_maps_directory.isDir()) || (!user_maps_directory.isWritable()))
        user_maps_directory_path = backup_directory_path;

    QString map_file_path = get_map_file_path(QFileDialog::ExistingFile, QFileDialog::AcceptOpen, "Map_Loading", user_maps_directory_path);
    QString warning_for_map_file_not_selected = "Map file was not selected.";
    QString warning_for_map_file_without_access = "Map file " + map_file_path + "  could not be loaded.";

    QSharedPointer<QFile> map_file = QSharedPointer<QFile>(new QFile(map_file_path));

    bool map_file_path_is_valid = assert_that_map_file_path_is_valid(map_file_path, map_file, warning_for_map_file_not_selected, warning_for_map_file_without_access);

    if(!(map_file_path_is_valid))
    {
        Map* dummy_map = nullptr;
        return dummy_map;
    }

    MapConverter *map_converter = new MapXMLconverter;
    Map* map = map_converter -> load_map_from_file(map_file);
    delete map_converter;

    if(map == nullptr)
    {
        QMessageBox::warning(this -> caller, NULL, "Selected file was not recognized as valid map file");
        return map;
    }

    return map;
}

bool MapConversionManager::assert_that_map_file_path_is_valid(QString map_file_path,  QSharedPointer<QFile> map_file,  QString warning_for_file_not_selected, QString warning_for_file_without_access)
{
    if(map_file_path.isEmpty())
    {
        QMessageBox::warning(this -> caller, NULL, warning_for_file_not_selected);
        return false;
    }

    if (!(map_file->open(QIODevice::ReadWrite)))
    {
        map_file -> close();
        QMessageBox::warning(this -> caller, NULL, warning_for_file_without_access);
        return false;
    }

    return true;
}
