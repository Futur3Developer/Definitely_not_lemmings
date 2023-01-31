#ifndef MAPCONVERSIONMANAGER_H
#define MAPCONVERSIONMANAGER_H

#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>

#include "MapFiles/map.h"
#include "MapFiles/mapcreator.h"

/**
 * Class designed as singleton to be used at each point where converting (serializing) Map instance is required.
 */
class MapConversionManager
{
public:
    MapConversionManager(const MapConversionManager&) = delete;

    static MapConversionManager& Get()
    {
        static MapConversionManager MapConversionManager_Instance;
        return MapConversionManager_Instance;
    }

    /**
     * Saves map to file selected by user.
     * If any error occures it is passed as QMessageBox to calling instance of MapCreator.
     * @param map_creator - pointer to calling instance of MapCreator which holds the map to be saved.
     */
    void process_map_saving_request(MapCreator *map_creator);

    /**
     * Loads file selected by user as instance of Map.
     * @param caller - pointer to calling QWidget, used to pass QMessageBox::warning corresponding to error if any occurs.
     * @return - if selected file can be loaded as Map instance the pointer to it is returned. Otherwise returns nullptr to be handled by caller.
     */
    Map* process_map_loading_request(QWidget *caller);

private:
    MapConversionManager();

    QStringList *file_types_supported;
    const QStringList *const_file_types_supported;
    QWidget *caller;

    /**
     * Method which both returns a value and MAY provide side effects!
     * Checks if file path passed as parameter can be used as directory to save files.
     * Creates the directory under provided path if it does not exist.
     * Shows QMessageBox::warning corresponding to error if any occurs.
     * @param user_maps_directory - directory to be checked or created if it does not exist.
     * @return - returns false if there is a file under provided path, path cannot be accessed or directory could not be created. Otherwise returns true.
     */
    bool assert_that_map_can_be_saved_in_default_directory(QFileInfo user_maps_directory);

    /**
     * Attemps to get file path from user through QFileDialog.
     * @return - returns QString containing file path if user provided it. Otherwise returns empty QString to be handled by calling function.
     */
    QString get_map_file_path(QFileDialog::FileMode file_mode, QFileDialog::AcceptMode accept_mode, QString window_title, QString user_maps_directory_path);

    /**
     * Checks if file path provided through map_file_path is not empty and can be accessed.
     * @return - returns true if file path is valid according to stated requirements. Otherwise returns false.
     */
    bool assert_that_map_file_path_is_valid(QString map_file_path, QSharedPointer<QFile> map_file, QString warning_for_file_not_selected, QString warning_for_file_without_access);
};

#endif // MAPCONVERSIONMANAGER_H
