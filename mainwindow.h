#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

#include <QMap>
#include <QSet>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef enum
{
    LANGUAGE_MAP_EN_2_SE,
    LANGUAGE_MAP_SE_2_EN
} LANGUAGE_MAP_TYPE;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_rdbEn2se_clicked();
    void on_rdbSe2en_clicked();
    void on_inputLeft_returnPressed();
    void on_inputRight_returnPressed();

    void on_inputLeft_textChanged(const QString &key);

    void on_buttonAdd_clicked();
    void on_buttonRemove_clicked();

    void closeEvent(QCloseEvent *event);

private:
    void savePair();
    void removePair();
    void serializeMaps();
    void deserializeMaps();

    Ui::MainWindow *ui;

    // Language-related data
    LANGUAGE_MAP_TYPE languageMapType;
    QMap<QString, QSet<QString>> en2seMap;
    QMap<QString, QSet<QString>> se2enMap;
    QString en2seFilename;
    QString se2enFilename;
};
#endif // MAINWINDOW_H
