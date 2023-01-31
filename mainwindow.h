#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "qboxlayout.h"
#include "MapFiles/mapcreator.h"
#include "MapFiles/MapConversion/mapconversionmanager.h"
#include "MapFiles/MapConversion/mapconverter.h"
#include "MapFiles/MapConversion/mapxmlconverter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * Main menu of the game.
 * It allows user to choose between creating new map for the game or playing already existing one.
 * In addition, it allows to check basic game's informations, such as goal of the game and settings.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    /**
     * MapXMLconverter instance to be used in start_game() slot.
     */
    MapXMLconverter *map_converter;

    /**
     * Checks if directory for default levels exists and selected level map file can be opened.
     * @return - if map file can be opened, returns pointer to file. Otherwise returns nullptr to be handled by calling function.
     */
    QSharedPointer<QFile> assert_that_level_can_be_loaded();

private slots:

    /**
     * Starts game on map selected by user and hides MainWindow. If map cannot be loaded shows corresponding warning.
     */
    void start_game();

    void quit_program();

    /**
     * Creates MapCreator instance and shows it. Hides MainWindow.
     */
    void create_map();

    /**
     * Loads map through MapConversionManager.
     * If it returns valid Map instance creates MapCreator instance in map editing mode and shows it, hides MainWindow.
     * Otherwise warning from MapConversionManager is shown to the user and MainWindow stays open.
     */
    void load_map();

    /**
     * Shows basic game information and settings in form of QMessageBox.
     */
    void show_informations();
};
#endif // MAINWINDOW_H
