#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QProcess>
#include <QTextCodec>
#include <QThread>

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


void MainWindow::on_pushButtonSelect_clicked()
{
    fpath = QFileDialog::getOpenFileName(this);

    if (! fpath.isEmpty())
    {
        ui->statusBar->showMessage("已选择: " + fpath);
    }
}

void MainWindow::on_pushButtonFuck_clicked()
{
    if (fpath.isEmpty())
    {
        ui->statusBar->showMessage("请先选择文件!");
        return;
    }

    QFileInfo fi(fpath);

    QString tmpName = fi.baseName() + "_unlocked";
    QString newName = tmpName + "." + fi.completeSuffix();

    QString tmpPath = fi.path() + "/" + tmpName;

    /* 解密 */
    QFile file(fpath);
    file.copy(tmpPath);
    file.flush();
    file.close();

    //QThread::sleep(1);

    /* 重命名。这里不能直接调用API重命名，否则会被再次加密，只能借助cmd操作 */
    QProcess process(this);
    QString cmd = "rename " + tmpName + " " + newName + "\r\n";

    process.setWorkingDirectory(fi.path());
    process.start("cmd.exe");
    //process.write(cmd.toStdString().c_str());
    process.write(QTextCodec::codecForName("GB18030")->fromUnicode(cmd).constData());
    process.write("exit\r\n");
    process.waitForFinished();
    process.close();

    ui->statusBar->showMessage("已解密：" + newName);
}
