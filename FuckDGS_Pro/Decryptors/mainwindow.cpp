#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QProcess>
#include <QTextCodec>
#include <QThread>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::isEncryptFile(const QString &filePath)
{
    return true;
}

bool MainWindow::IsDecryptSucess(const QString &oldFile, const QString &newFile)
{
    return true;
}

bool MainWindow::decryptFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);

    if (! fileInfo.exists())
    {
        return false;
    }

    if (! isEncryptFile(filePath))
    {
        return true;
    }

    QString tmpName = fileInfo.completeBaseName() + "_Fucked";
    QString tmpPath = fileInfo.path() + "/" + tmpName;

    QString decName = fileInfo.completeBaseName() + "_Fucked." + fileInfo.suffix();
    QString decPath = fileInfo.path() + "/" + decName;

    // 解密
    QFile file(filePath);
    file.copy(tmpPath);
    file.flush();
    file.close();

    QThread::sleep(1);

    // 重命名(这里不能直接调用API重命名，否则会被再次加密，只能借助cmd操作)
    QString cmd = "rename \"" + tmpName + "\" \"" + decName + "\"\r\n";
    QProcess process();
    process.setWorkingDirectory(fileInfo.path());
    process.start("cmd.exe");
    process.write(QTextCodec::codecForName("GB18030")->fromUnicode(cmd).constData());
    process.write("exit\r\n");
    process.waitForFinished();
    process.close();

    // 判断解密结果？
    if (! IsDecryptSucess(filePath, decPath))
    {
        return false;
    }

    return true;
}

