#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDataStream>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Init input QLineEdit
    ui->rdbEn2se->setChecked(true);
    ui->inputLeft->clear();
    ui->inputRight->clear();

    // Init displayRecord
    ui->displayRecord->clear();
    ui->displayRecord->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // Background setup
    languageMapType = LANGUAGE_MAP_EN_2_SE;
    en2seFilename = "D:\\Coding\\Qt\\vocabing\\.data\\serial_en2se.vcb";
    se2enFilename = "D:\\Coding\\Qt\\vocabing\\.data\\serial_se2en.vcb";
    deserializeMaps();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::savePair()
{
    QString enText = (languageMapType == LANGUAGE_MAP_EN_2_SE) ?
                     ui->inputLeft->text() :
                     ui->inputRight->text();
    QString seText = (languageMapType == LANGUAGE_MAP_SE_2_EN) ?
                     ui->inputLeft->text() :
                     ui->inputRight->text();

    auto seSet = en2seMap.value(enText);
    en2seMap.insert(enText, seSet += seText);

    auto enSet = se2enMap.value(seText);
    se2enMap.insert(seText, enSet += enText);

    ui->inputLeft->clear();
    ui->inputRight->clear();
}


void MainWindow::removePair()
{
    QString enText = (languageMapType == LANGUAGE_MAP_EN_2_SE) ?
                     ui->inputLeft->text() :
                     ui->inputRight->text();
    QString seText = (languageMapType == LANGUAGE_MAP_SE_2_EN) ?
                     ui->inputLeft->text() :
                     ui->inputRight->text();

    en2seMap[enText].remove(seText);
    se2enMap[seText].remove(enText);

    ui->inputLeft->clear();
    ui->inputRight->clear();
}


void MainWindow::on_inputLeft_returnPressed()
{
    // TODO: what to do here?
}


void MainWindow::on_inputRight_returnPressed()
{
    if (!ui->inputLeft->text().isEmpty() && !ui->inputRight->text().isEmpty()) {
        savePair();
    }
}


void displaySet(QLabel *displayRecord,
                const QString &key,
                const QString &tag,
                const QSet<QString> &set)
{
    if (key.isEmpty()) {
        // Return if no key input
        displayRecord->clear();
        return;
    } else if (set.isEmpty()) {
        // Return if key is not in record
        displayRecord->setText("Cannot find:\n" + tag + "\t" + key);
        return;
    }

    QString display = tag + "\t" + key + "\n\n";
    for (QString v : set) {
        display += ("\t" + v + "\n");
    }
    displayRecord->setText(display);
}


void MainWindow::on_inputLeft_textChanged(const QString &key)
{
    QString tag = (languageMapType == LANGUAGE_MAP_EN_2_SE) ?
                  " [EN]" : " [SE]";
    auto *map_p = (languageMapType == LANGUAGE_MAP_EN_2_SE) ?
                  &en2seMap : &se2enMap;
    auto set = map_p->value(key);
    displaySet(ui->displayRecord, key, tag, set);
}


void MainWindow::on_rdbEn2se_clicked()
{
    languageMapType = LANGUAGE_MAP_EN_2_SE;

    QString key = ui->inputLeft->text();
    displaySet(ui->displayRecord,
               key,
               " [EN]",
               en2seMap.value(key));
}


void MainWindow::on_rdbSe2en_clicked()
{
    languageMapType = LANGUAGE_MAP_SE_2_EN;

    QString key = ui->inputLeft->text();
    displaySet(ui->displayRecord,
               key,
               " [SE]",
               se2enMap.value(key));
}


void MainWindow::on_buttonAdd_clicked()
{
    if (!ui->inputLeft->text().isEmpty() && !ui->inputRight->text().isEmpty()) {
        savePair();
    }
}


void MainWindow::on_buttonRemove_clicked()
{
    if (!ui->inputLeft->text().isEmpty() && !ui->inputRight->text().isEmpty()) {
        removePair();
    }
}


void serializeMap(const QMap<QString, QSet<QString>> &map, const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QDataStream out(&file);
    std::for_each (map.keyBegin(), map.keyEnd(), [&](const QString &key) {
        out << key;
        out << map.value(key);
    });
}


void MainWindow::serializeMaps()
{
    serializeMap(en2seMap, en2seFilename);
    serializeMap(se2enMap, se2enFilename);
}


void deserializeMap(QMap<QString, QSet<QString>> &map, const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&file);
    QString key;
    QSet<QString> value;
    while (!file.atEnd()) {
        in >> key;
        in >> value;
        map.insert(key, value);
    }
}


void MainWindow::deserializeMaps()
{
    deserializeMap(en2seMap, en2seFilename);
    deserializeMap(se2enMap, se2enFilename);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    serializeMaps();
    event->accept();
}

