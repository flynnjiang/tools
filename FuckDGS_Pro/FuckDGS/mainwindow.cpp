#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "decryptor.h"

#include <QFileDialog>
#include <QProcess>
#include <QTextCodec>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    selectFiles.clear();
    selectDir.clear();

    totalCnt = 0;
    SuccessCnt = 0;
    FailedCnt = 0;

    decryptors.clear();
    decryptors.append(new Decryptor("wps.exe",     QStringList() << "*.doc" << "*.docx"));
    decryptors.append(new Decryptor("et.exe",      QStringList() << "*.xls" << "*.xlsx"));
    decryptors.append(new Decryptor("wpp.exe",     QStringList() << "*.ppt" << "*.pptx"));
    decryptors.append(new Decryptor("Acrobat.exe", QStringList() << "*.pdf"));

    if (! isSysSupportDecrypt())
        QMessageBox::information(this, "注意", "未检测到DGS运行！本程序需要在已安装了DGS的系统内"
                                 "才能正常工作。请确认DGS是否已正常工作，若确认正常，请忽略此消息。");

}


MainWindow::~MainWindow()
{
    selectFiles.clear();
    selectDir.clear();

    for (int i = 0; i < decryptors.size(); i++)
        delete decryptors[i];

    decryptors.clear();

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

bool MainWindow::isSysSupportDecrypt()
{
    QProcess process(this);

    process.start("TASKLIST /FI \"IMAGENAME eq DGSService.exe\"");
    process.waitForFinished();
    return process.readAllStandardOutput().contains('=');
}


QStringList MainWindow::getSupportFileTypes()
{
    QStringList supportTypes;

    for (int i = 0; i < decryptors.size(); i++)
        supportTypes << decryptors[i]->getSupportFileTypes();

    return supportTypes;
}


// 根据文件扩展名选择对应解析器
bool MainWindow::decryptFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    bool retVal = false;

    if (! Decryptor::isEncryptFile(filePath))
        return false;

    qDebug() << "正在解密: " << filePath;

    totalCnt++;

    for (int i = 0; i < decryptors.size(); i++)
        if (decryptors[i]->getSupportFileTypes().contains("*." + fileInfo.suffix()))
            retVal = decryptors[i]->decryptFile(filePath);

    if (retVal) {
        qDebug() << "解密成功: " << filePath;
        SuccessCnt++;
    } else {
        qDebug() << "解密失败: " << filePath;
        FailedCnt++;
    }

    showTips(QString().sprintf("已处理%u个加密文件，成功%u个，失败%u个", totalCnt, SuccessCnt, FailedCnt));

    return retVal;
}

bool MainWindow::decryptFiles(const QStringList &fileList)
{
    for (int i = 0; i < fileList.size(); i++)
        decryptFile(fileList[i]);

    return true;
}


bool MainWindow::decryptDir(const QString &dirPath, const QStringList &filters)
{
    QDir dir(dirPath);

    qDebug() << "进入目录：" << dirPath;

    foreach(QString file, dir.entryList(filters, QDir::Files))
        decryptFile(dir.absoluteFilePath(file));

    foreach(QString subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
        decryptDir(dir.absolutePath() + "/" + subdir, filters);

    return true;
}


void MainWindow::on_pushButtonSelect_clicked()
{
    QString filterStr = "支持的文件 (" + getSupportFileTypes().join(' ') + ")";

    selectFiles = QFileDialog::getOpenFileNames(this, "选择文件（支持多选）", "C:", filterStr);
}

void MainWindow::on_pushButtonSelectDir_clicked()
{
    selectDir = QFileDialog::getExistingDirectory(this);
}

void MainWindow::on_pushButtonFuck_clicked()
{
    if (selectFiles.isEmpty() && selectDir.isEmpty())
    {
        showTips("请先选择文件（夹）哦！");
        return;
    }

    showTips("请稍等……");

    if (! selectFiles.isEmpty())
        decryptFiles(selectFiles);
    else if (! selectDir.isEmpty())
        decryptDir(selectDir, getSupportFileTypes());

    showTips(QString().sprintf("全部处理完成！共%u个文件，成功%u个，失败%u个", totalCnt, SuccessCnt, FailedCnt));

    selectFiles.clear();
    selectDir.clear();

    totalCnt = 0;
    SuccessCnt = 0;
    FailedCnt = 0;
}


