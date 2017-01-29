#include "dialog.h"
#include "ui_dialog.h"

#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/tstring.h>
#include <taglib/fileref.h>
#include <ogg/ogg.h>
#include <taglib/oggfile.h>
#include <taglib/tfile.h>
#include <taglib/vorbisfile.h>

using namespace TagLib;


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setData(Music * _musicToEdit) {
    this->musicToEdit = _musicToEdit;
    ui->artistEdit->setText(musicToEdit->getArtistName().c_str());
    ui->albumEdit->setText(musicToEdit->getAlbumName().c_str());
    ui->titleEdit->setText(musicToEdit->getTrackName().c_str());
}

void Dialog::on_buttonBox_accepted(){
    std::string filename = musicToEdit->getTrackName() + ".ogg";
    FileRef file(musicToEdit->getPathFile().c_str());
    std::string artist = filename.substr(0,filename.find(" -"));
    Tag* t = file.tag();
    t->setArtist(ui->artistEdit->text().toStdString());
    t->setAlbum(ui->albumEdit->text().toStdString());
    t->setTitle(ui->titleEdit->text().toStdString());
    file.save();
}
