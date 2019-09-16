#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool decryptFile(const QString &filePath);

private:
    bool isEncryptFile(const QString &filePath);
    bool IsDecryptSucess(const QString &oldFile, const QString &newFile);

private slots:

private:
    Ui::MainWindow *ui;

    QString filePath;
};

#endif // MAINWINDOW_H
