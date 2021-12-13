#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDataStream>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , en2seMapP(new RecordMap)
    , se2enMapP(new RecordMap)
{
    ui->setupUi(this);

    // Init input QLineEdit
    ui->rdbEn2se->setChecked(true);
    ui->inputLeft->clear();
    ui->inputRight->clear();

    // Init displayRecord
    ui->displayLabel->clear();
    ui->displayLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // Background setup
    languageType = LANGUAGE_MAP_EN_2_SE;
    en2seFilename = "D:\\Coding\\Qt\\vocabing\\.data\\serial_en2se.vcb";
    se2enFilename = "D:\\Coding\\Qt\\vocabing\\.data\\serial_se2en.vcb";
    deserializeMaps();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_rdbEn2se_clicked()
{
    languageType = LANGUAGE_MAP_EN_2_SE;

    QString key = ui->inputLeft->text();
    displayRecordLabel();
}

void MainWindow::on_rdbSe2en_clicked()
{
    languageType = LANGUAGE_MAP_SE_2_EN;

    QString key = ui->inputLeft->text();
    displayRecordLabel();
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

void MainWindow::on_inputLeft_textChanged(const QString &key)
{
    displayRecordLabel();
}

void MainWindow::on_inputRight_textChanged(const QString &key)
{
    // TODO: what to do here?
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


void MainWindow::serializeMaps()
{
    en2seMapP->serializeRecord(en2seFilename);
    se2enMapP->serializeRecord(se2enFilename);
}


void MainWindow::deserializeMaps()
{
    en2seMapP->deserializeRecord(en2seFilename);
    se2enMapP->deserializeRecord(se2enFilename);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    serializeMaps();
    event->accept();
}


void MainWindow::savePair()
{
    QString enText = (languageType == LANGUAGE_MAP_EN_2_SE) ?
                     ui->inputLeft->text() :
                     ui->inputRight->text();
    QString seText = (languageType == LANGUAGE_MAP_SE_2_EN) ?
                     ui->inputLeft->text() :
                     ui->inputRight->text();

    en2seMapP->addRecord(enText, seText);
    se2enMapP->addRecord(seText, enText);

    ui->inputLeft->clear();
    ui->inputRight->clear();
}


void MainWindow::removePair()
{
    QString enText = (languageType == LANGUAGE_MAP_EN_2_SE) ?
                     ui->inputLeft->text() :
                     ui->inputRight->text();
    QString seText = (languageType == LANGUAGE_MAP_SE_2_EN) ?
                     ui->inputLeft->text() :
                     ui->inputRight->text();

    en2seMapP->removeRecord(enText, seText);
    se2enMapP->removeRecord(seText, enText);

    ui->inputLeft->clear();
    ui->inputRight->clear();
}


void MainWindow::displayRecordLabel()
{
    const QString key = ui->inputLeft->text();
    const QString tag = (languageType == LANGUAGE_MAP_EN_2_SE) ?
                        "[EN]" : "[SE]";
    auto mapP = (languageType == LANGUAGE_MAP_EN_2_SE) ?
                en2seMapP : se2enMapP;

    if (key.isEmpty()) {
        // Return if no key input
        ui->displayLabel->clear();
        return;
    } else if (!mapP->isKeyContained(key)) {
        // Return if key is not in record
        ui->displayLabel->setText("No record for:\n" + tag + "\t" + key);
        return;
    }

    // Set record to display
    const QString otherTag = (languageType == LANGUAGE_MAP_EN_2_SE) ?
                             "[SE]" : "[EN]";
    QString label = tag + "\t" + key + "\n\n" + otherTag;
    for (QString v : mapP->getRecordList(key)) {
        label += ("\t" + v + "\n");
    }
    ui->displayLabel->setText(label);
}

