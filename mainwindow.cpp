#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <string>
#include <QTimer>
#include <QTableWidget>
#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/tstring.h>
#include <taglib/fileref.h>
#include <sstream>
#include <ctime>
#include "music.h"
#include "soundplayer.h"
#include "dialog.h"
#include "QFileDialog"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
        ui->setupUi(this);
        player = SoundPlayer::startPlayer();

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(1000);
        this->player->goThroughFolder();
        this->initTable();
        this->checkButtons();
        ui->tableWidgetPlaylist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    if(this->player->getIsMusicLoaded() == false) {
        this->player->playPlayer();
        this->initSlider();
    } else {
        this->player->playPlayer();
    }
    ui->tableWidget->selectRow(this->player->getCurrent());
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    Mix_VolumeMusic(position);
}

void MainWindow::initSlider() {
    ui->positionSlider->setMaximum(this->player->getCurrentMusic()->getLength());
    this->player->getCurrentMusic()->setStartTime(time(0));

    ui->artist->setText( this->player->getCurrentMusic()->getArtistName().c_str() );
    ui->album->setText( this->player->getCurrentMusic()->getAlbumName().c_str() );
    ui->track->setText( this->player->getCurrentMusic()->getTrackName().c_str() );

    int length = this->player->getCurrentMusic()->getLength();
    int minutes = int(length/60);
    int seconds = length - minutes * 60;
    ui->positionSlider->setMaximum(length);
    ui->length->setText(QString("%1:%2").arg(minutes).arg(seconds));
}

void MainWindow::update(){
    if(Mix_PausedMusic()) {
        this->player->getCurrentMusic()->setStartTime(this->player->getCurrentMusic()->getStartTime() + 1);
    } else if(Mix_PlayingMusic()) {
        int elapsed_time = time(0) - this->player->getCurrentMusic()->getStartTime();
        ui->positionSlider->setSliderPosition(elapsed_time);
        ui->positionSlider->setValue(elapsed_time);

        this->previousPosition = elapsed_time;
        int length = elapsed_time;
        int minutes = int(length/60);
        int seconds = length - minutes * 60;
        ui->position->setText(QString("%1:%2").arg(minutes).arg(seconds));

        if(elapsed_time >= this->player->getCurrentMusic()->getLength()) {
            on_nextButton_clicked();
        }
    }else {
        ui->positionSlider->setSliderPosition(0);
    }
}

void MainWindow::checkButtons() {
    // Set availibility of buttons.
    QPixmap pixmapPlay("./assets/play_active.png");
    QIcon ButtonIcon(pixmapPlay);
    ui->pushButton->setIcon(ButtonIcon);
    ui->pushButton->setIconSize(QSize(30,30));

    int size = this->player->getAllMusics().size();
    if(this->player->getIsPlaylist())
        size = this->player->getAllPlaylist().size();

    if(this->player->getCurrent() == 0) {
        ui->prevButton->setEnabled(false);
        ui->prevButton->setDisabled(true);
        QPixmap pixmapPrevInactive("./assets/prev_inactive.png");
        QIcon ButtonIcon(pixmapPrevInactive);
        ui->prevButton->setIcon(ButtonIcon);
        ui->prevButton->setIconSize(QSize(30,30));

    } else {
        ui->prevButton->setEnabled(true);
        ui->prevButton->setDisabled(false);
        QPixmap pixmapPrevActive("./assets/prev_active.png");
        QIcon ButtonIcon(pixmapPrevActive);
        ui->prevButton->setIcon(ButtonIcon);
        ui->prevButton->setIconSize(QSize(30,30));
    }

    if(this->player->getCurrent() == (int)size - 1) {
        ui->nextButton->setEnabled(false);
        ui->nextButton->setDisabled(true);
        QPixmap pixmapNextInactive("./assets/next_inactive.png");
        QIcon ButtonIcon(pixmapNextInactive);
        ui->nextButton->setIcon(ButtonIcon);
        ui->nextButton->setIconSize(QSize(30,30));
    } else {
        ui->nextButton->setEnabled(true);
        ui->nextButton->setDisabled(false);
        QPixmap pixmapNextActive("./assets/next_active.png");
        QIcon ButtonIcon(pixmapNextActive);
        ui->nextButton->setIcon(ButtonIcon);
        ui->nextButton->setIconSize(QSize(30,30));
    }
}

void MainWindow::on_positionSlider_sliderMoved(int position)
{
    Mix_SetMusicPosition((double)position);
    if(previousPosition < position) {
        this->player->getCurrentMusic()->setStartTime(this->player->getCurrentMusic()->getStartTime() + previousPosition - position);
    } else {
        this->player->getCurrentMusic()->setStartTime(this->player->getCurrentMusic()->getStartTime() + previousPosition - position);
    }
    this->previousPosition = position;
}

void MainWindow::initTable() {
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for(int i = 0; i < (int)this->player->getAllMusics().size();i++) {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem( this->player->getMusicById(i)->getArtistName().c_str()));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem( this->player->getMusicById(i)->getAlbumName().c_str() ));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem( this->player->getMusicById(i)->getTrackName().c_str() ));
        int length = this->player->getMusicById(i)->getLength();
        int minutes = int(length/60);
        int seconds = length - minutes * 60;
        ui->tableWidget->setItem(i,3,new QTableWidgetItem( QString("%1:%2").arg(minutes).arg(seconds) ));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem( QString("Double ckick to edit")));
        ui->tableWidget->item(i, 4)->setBackground(QColor(0,118,255));
    }
}

void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index){
    if(this->player->getIsPlaylist() && index.column()!=4) {
        this->swapMusic(index.row(), false);
    } else if(index.column() == 4) {
        if(this->player->getIsMusicLoaded() == true) {
            Mix_FreeMusic(this->player->getCurrentMusic()->getMusic());
            this->player->setIsMusicLoaded(false);
        }
        editDialog = new Dialog(this);
        editDialog->setModal(true);
        this->player->setIsPlaylist(false);
        editDialog->setData(this->player->getMusicById(index.row()));
        editDialog->exec();
        ui->tableWidget->setRowCount(0);
        this->player->eraseMusic();
        this->player->goThroughFolder();
        this->initTable();
    } else {
        selectedTrack(index.row());
    }
}

void MainWindow::swapMusic(int index, bool isP) {
    Mix_FreeMusic(this->player->getCurrentMusic()->getMusic());
    this->player->setIsPlaylist(isP);
    this->player->setCurrent(index);
    this->player->setIsMusicLoaded(false);
    this->player->playPlayer();
    this->initSlider();
    this->checkButtons();
    if(this->player->getIsPlaylist() == true)
        ui->tableWidgetPlaylist->selectRow(this->player->getCurrent());
    else
        ui->tableWidget->selectRow(this->player->getCurrent());
}

void MainWindow::selectedTrack(int id) {
    Mix_FreeMusic(this->player->getCurrentMusic()->getMusic());
    this->player->setCurrent(id);
    this->player->setIsMusicLoaded(false);
    this->player->playPlayer();
    this->initSlider();
    this->checkButtons();
    if(this->player->getIsPlaylist() == true)
        ui->tableWidgetPlaylist->selectRow(this->player->getCurrent());
    else
        ui->tableWidget->selectRow(this->player->getCurrent());
}

void MainWindow::on_nextButton_clicked(){
    int size = this->player->getAllMusics().size();
    if(this->player->getIsPlaylist())
        size = this->player->getAllPlaylist().size();
    if(this->player->getCurrent() == (int)size - 1) {
        ui->nextButton->setEnabled(false);
        ui->nextButton->setDisabled(true);
        if(this->player->getIsMusicLoaded() == true) {
            Mix_FreeMusic(this->player->getCurrentMusic()->getMusic());
        }
        this->player->setIsMusicLoaded(false);
    } else {
        ui->nextButton->setEnabled(true);
        ui->nextButton->setDisabled(false);
        this->selectedTrack(this->player->getCurrent() + 1);
    }

    this->checkButtons();
}

void MainWindow::on_prevButton_clicked(){
    if(this->player->getCurrent() == 0) {
        ui->prevButton->setEnabled(false);
        ui->prevButton->setDisabled(true);
        if(this->player->getIsMusicLoaded() == true) {
            Mix_FreeMusic(this->player->getCurrentMusic()->getMusic());
        }
        this->player->setIsMusicLoaded(false);
    } else {
        ui->prevButton->setEnabled(true);
        ui->prevButton->setDisabled(false);
        this->selectedTrack(this->player->getCurrent() - 1);
    }

    this->checkButtons();
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
}


void MainWindow::on_pushButtonM3U_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Ouvir playlist"),tr("Playlist (*.txt)"));

    if(file.isEmpty()) {
        // Error ...
        return;
    }

    QFileInfo fileInfo(file);

    this->player->setPathPlaylist(fileInfo.absoluteFilePath().toStdString());
    this->player->loadPlaylist();
    this->initTablePlaylist();
}

void MainWindow::initTablePlaylist() {
    ui->tableWidgetPlaylist->setRowCount(0);
    ui->tableWidgetPlaylist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for(int i = 0; i < (int)this->player->getAllPlaylist().size();i++) {
        ui->tableWidgetPlaylist->insertRow(i);
        ui->tableWidgetPlaylist->setItem(i,0,new QTableWidgetItem( this->player->getMusicPlaylistById(i)->getArtistName().c_str()));
        ui->tableWidgetPlaylist->setItem(i,1,new QTableWidgetItem( this->player->getMusicPlaylistById(i)->getAlbumName().c_str() ));
        ui->tableWidgetPlaylist->setItem(i,2,new QTableWidgetItem( this->player->getMusicPlaylistById(i)->getTrackName().c_str() ));
        int length = this->player->getMusicPlaylistById(i)->getLength();
        int minutes = int(length/60);
        int seconds = length - minutes * 60;
        ui->tableWidgetPlaylist->setItem(i,3,new QTableWidgetItem( QString("%1:%2").arg(minutes).arg(seconds) ));
        ui->tableWidgetPlaylist->setItem(i,4,new QTableWidgetItem( QString("Double ckick to edit")));
        ui->tableWidgetPlaylist->item(i, 4)->setBackground(QColor(0,118,255));
    }
}

void MainWindow::on_tableWidgetPlaylist_doubleClicked(const QModelIndex &index){
    if(this->player->getIsPlaylist() == false && index.column()!=4) {
        this->swapMusic(index.row(), true);
    } else if(index.column() == 4) {
        if(this->player->getIsMusicLoaded() == true) {
            Mix_FreeMusic(this->player->getCurrentMusic()->getMusic());
            this->player->setIsMusicLoaded(false);
        }
        editDialog = new Dialog(this);
        editDialog->setModal(true);
        this->player->setIsPlaylist(true);
        editDialog->setData(this->player->getMusicById(index.row()));
        editDialog->exec();
        this->player->erasePlaylist();
        std::cout << this->player->getAllPlaylist().size() << std::endl;
        this->player->loadPlaylist();
        ui->tableWidgetPlaylist->setRowCount(0);
        this->initTablePlaylist();
    } else {
        selectedTrack(index.row());
    }
}
