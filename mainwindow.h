#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "soundplayer.h"
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initSlider();
    int getPreviousPosition();
    void initTable();
    void initTablePlaylist();
    void selectedTrack(int id);
    void checkButtons();
    void swapMusic(int index, bool isP);

private slots:
    void on_pushButton_clicked();
    void on_horizontalSlider_sliderMoved(int position);
    void update();
    void on_positionSlider_sliderMoved(int position);
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_nextButton_clicked();
    void on_prevButton_clicked();
    void on_tabWidget_tabBarClicked(int index);
    void on_pushButtonM3U_clicked();

    void on_tableWidgetPlaylist_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    SoundPlayer * player;
    int previousPosition;
    Dialog * editDialog;
};

#endif // MAINWINDOW_H
