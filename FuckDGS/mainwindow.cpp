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

    filePath.clear();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTips(const QString &tips)
{
    if (tips.size() > 32)
    {
        //tips.leftJustified(20, '.', true);
        //tips.rightJustified(8, '.', true);
        ui->statusBar->showMessage(tips);
    }
    else
    {
        ui->statusBar->showMessage(tips);
    }
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
        showTips("文件不存在");
        return false;
    }

    if (! isEncryptFile(filePath))
    {
        showTips("文件没有被加密");
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
    QProcess process(this);
    process.setWorkingDirectory(fileInfo.path());
    process.start("cmd.exe");
    process.write(QTextCodec::codecForName("GB18030")->fromUnicode(cmd).constData());
    process.write("exit\r\n");
    process.waitForFinished();
    process.close();

    // 判断解密结果？
    if (! IsDecryptSucess(filePath, decPath))
    {
        showTips("解密文件失败");
        return false;
    }

    return true;
}


void MainWindow::on_pushButtonSelect_clicked()
{
    filePath = QFileDialog::getOpenFileName(this);

    if (! filePath.isEmpty())
    {
        QFileInfo fileInfo(filePath);
        showTips("您选择了: " + fileInfo.fileName());
    }
}


void MainWindow::on_pushButtonFuck_clicked()
{
    bool retVal = false;

    if (filePath.isEmpty())
    {
        showTips("请先选择文件哦！");
        return;
    }

    showTips("请稍等……");

    retVal = decryptFile(filePath);
    if (retVal)
    {
        showTips("恭喜您！解密成功！");
        filePath.clear();
    }
    else
    {
        showTips("抱歉，解密失败");
    }
}
