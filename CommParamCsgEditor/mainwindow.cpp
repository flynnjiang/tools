#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDate>

static const QDate buildDate = QLocale( QLocale::English ).toDate( QString( __DATE__ ).replace( "  ", " 0" ), "MMM dd yyyy");


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    memset(&cfg, 0, sizeof(cfg));

    programName = "通信扩展参数编辑器（南网18规范" + buildDate.toString("yyyyMMdd") + "）";
    setWindowTitle(programName);

    lastMnc = 0;
    syncParamToForm(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


QString MainWindow::ipToString(struct in_addr &ip)
{
    char buf[32];

    snprintf(buf, sizeof(buf), "%d.%d.%d.%d",
              ip.s_addr & 0xff,
             (ip.s_addr >>  8) & 0xff,
             (ip.s_addr >> 16) & 0xff,
             (ip.s_addr >> 24) & 0xff);

    return QString(buf);
}

void MainWindow::stringToIp(const QString &str, struct in_addr &ip)
{
    unsigned int addr[4] = {0};
    sscanf(str.toStdString().c_str(), "%u.%u.%u.%u", &addr[0], &addr[1], &addr[2], &addr[3]);
    ip.s_addr = (addr[3] << 24) + (addr[2] << 16) + (addr[1] << 8) + addr[0];
}


void MainWindow::syncParamToForm(int mnc)
{
    ui->comboBoxNetMode->setCurrentIndex(cfg.iSetNetMode);

    ui->checkBoxAPN12G->setChecked(cfg.apn_mode[0].for2G);
    ui->checkBoxAPN13G->setChecked(cfg.apn_mode[0].for3G);
    ui->checkBoxAPN14G->setChecked(cfg.apn_mode[0].for4G);

    ui->checkBoxAPN22G->setChecked(cfg.apn_mode[1].for2G);
    ui->checkBoxAPN23G->setChecked(cfg.apn_mode[1].for3G);
    ui->checkBoxAPN24G->setChecked(cfg.apn_mode[1].for4G);

    ui->checkBoxAPN32G->setChecked(cfg.apn_mode[2].for2G);
    ui->checkBoxAPN33G->setChecked(cfg.apn_mode[2].for3G);
    ui->checkBoxAPN34G->setChecked(cfg.apn_mode[2].for4G);

    ui->checkBoxAPN42G->setChecked(cfg.apn_mode[3].for2G);
    ui->checkBoxAPN43G->setChecked(cfg.apn_mode[3].for3G);
    ui->checkBoxAPN44G->setChecked(cfg.apn_mode[3].for4G);

    ui->checkBoxAPN52G->setChecked(cfg.apn_mode[4].for2G);
    ui->checkBoxAPN53G->setChecked(cfg.apn_mode[4].for3G);
    ui->checkBoxAPN54G->setChecked(cfg.apn_mode[4].for4G);

    ui->lineEditIP1->setText(ipToString(cfg.st_NetParam[mnc].stHostIP1));
    ui->lineEditPort1->setText(QString::number(cfg.st_NetParam[mnc].iHostPort1));

    ui->lineEditIP2->setText(ipToString(cfg.st_NetParam[mnc].stHostIP2));
    ui->lineEditPort2->setText(QString::number(cfg.st_NetParam[mnc].iHostPort2));

    ui->lineEditIP3->setText(ipToString(cfg.st_NetParam[mnc].stHostIP3));
    ui->lineEditPort3->setText(QString::number(cfg.st_NetParam[mnc].iHostPort3));

    ui->lineEditIPProxy->setText(ipToString(cfg.st_NetParam[mnc].stMGateway));
    ui->lineEditPortProxy->setText(QString::number(cfg.st_NetParam[mnc].iGatewayPort));

    ui->lineEditAPN1->setText(cfg.stDialParam[mnc][0].ucAPNName);
    ui->lineEditUsername1->setText(cfg.stDialParam[mnc][0].ucUserName);
    ui->lineEditPassword1->setText(cfg.stDialParam[mnc][0].ucUserPasswd);

    ui->lineEditAPN2->setText(cfg.stDialParam[mnc][1].ucAPNName);
    ui->lineEditUsername2->setText(cfg.stDialParam[mnc][1].ucUserName);
    ui->lineEditPassword2->setText(cfg.stDialParam[mnc][1].ucUserPasswd);

    ui->lineEditAPN3->setText(cfg.stDialParam[mnc][2].ucAPNName);
    ui->lineEditUsername3->setText(cfg.stDialParam[mnc][2].ucUserName);
    ui->lineEditPassword3->setText(cfg.stDialParam[mnc][2].ucUserPasswd);

    ui->lineEditAPN4->setText(cfg.stDialParam[mnc][3].ucAPNName);
    ui->lineEditUsername4->setText(cfg.stDialParam[mnc][3].ucUserName);
    ui->lineEditPassword4->setText(cfg.stDialParam[mnc][3].ucUserPasswd);

    ui->lineEditAPN5->setText(cfg.stDialParam[mnc][4].ucAPNName);
    ui->lineEditUsername5->setText(cfg.stDialParam[mnc][4].ucUserName);
    ui->lineEditPassword5->setText(cfg.stDialParam[mnc][4].ucUserPasswd);
}

void MainWindow::syncFormToParam(int mnc)
{
    cfg.iSetNetMode = ui->comboBoxNetMode->currentIndex();

    cfg.apn_mode[0].for2G = ui->checkBoxAPN12G->isChecked();
    cfg.apn_mode[0].for3G = ui->checkBoxAPN13G->isChecked();
    cfg.apn_mode[0].for4G = ui->checkBoxAPN14G->isChecked();
    cfg.apn_mode[0].unused = 0;

    cfg.apn_mode[1].for2G = ui->checkBoxAPN22G->isChecked();
    cfg.apn_mode[1].for3G = ui->checkBoxAPN23G->isChecked();
    cfg.apn_mode[1].for4G = ui->checkBoxAPN24G->isChecked();
    cfg.apn_mode[1].unused = 0;

    cfg.apn_mode[2].for2G = ui->checkBoxAPN32G->isChecked();
    cfg.apn_mode[2].for3G = ui->checkBoxAPN33G->isChecked();
    cfg.apn_mode[2].for4G = ui->checkBoxAPN34G->isChecked();
    cfg.apn_mode[2].unused = 0;

    cfg.apn_mode[3].for2G = ui->checkBoxAPN42G->isChecked();
    cfg.apn_mode[3].for3G = ui->checkBoxAPN43G->isChecked();
    cfg.apn_mode[3].for4G = ui->checkBoxAPN44G->isChecked();
    cfg.apn_mode[3].unused = 0;

    cfg.apn_mode[4].for2G = ui->checkBoxAPN52G->isChecked();
    cfg.apn_mode[4].for3G = ui->checkBoxAPN53G->isChecked();
    cfg.apn_mode[4].for4G = ui->checkBoxAPN54G->isChecked();
    cfg.apn_mode[4].unused = 0;

    stringToIp(ui->lineEditIP1->text(), cfg.st_NetParam[mnc].stHostIP1);
    cfg.st_NetParam[mnc].iHostPort1 = ui->lineEditPort1->text().toUShort();

    stringToIp(ui->lineEditIP2->text(), cfg.st_NetParam[mnc].stHostIP2);
    cfg.st_NetParam[mnc].iHostPort2 = ui->lineEditPort2->text().toUShort();

    stringToIp(ui->lineEditIP3->text(), cfg.st_NetParam[mnc].stHostIP3);
    cfg.st_NetParam[mnc].iHostPort3 = ui->lineEditPort3->text().toUShort();

    stringToIp(ui->lineEditIPProxy->text(), cfg.st_NetParam[mnc].stMGateway);
    cfg.st_NetParam[mnc].iGatewayPort = ui->lineEditPortProxy->text().toUShort();

    snprintf(cfg.stDialParam[mnc][0].ucAPNName,
            sizeof(cfg.stDialParam[mnc][0].ucAPNName),
            "%s", ui->lineEditAPN1->text().toStdString().c_str());
    snprintf(cfg.stDialParam[mnc][0].ucUserName,
            sizeof(cfg.stDialParam[mnc][0].ucUserName),
            "%s", ui->lineEditUsername1->text().toStdString().c_str());
    snprintf(cfg.stDialParam[mnc][0].ucUserPasswd,
            sizeof(cfg.stDialParam[mnc][0].ucUserPasswd),
            "%s", ui->lineEditPassword1->text().toStdString().c_str());

    snprintf(cfg.stDialParam[mnc][1].ucAPNName,
            sizeof(cfg.stDialParam[mnc][1].ucAPNName),
            "%s", ui->lineEditAPN2->text().toStdString().c_str());
    snprintf(cfg.stDialParam[mnc][1].ucUserName,
            sizeof(cfg.stDialParam[mnc][1].ucUserName),
            "%s", ui->lineEditUsername2->text().toStdString().c_str());
    snprintf(cfg.stDialParam[mnc][1].ucUserPasswd,
            sizeof(cfg.stDialParam[mnc][1].ucUserPasswd),
            "%s", ui->lineEditPassword2->text().toStdString().c_str());

    snprintf(cfg.stDialParam[mnc][2].ucAPNName,
            sizeof(cfg.stDialParam[mnc][2].ucAPNName),
            "%s", ui->lineEditAPN3->text().toStdString().c_str());
    snprintf(cfg.stDialParam[mnc][2].ucUserName,
            sizeof(cfg.stDialParam[mnc][2].ucUserName),
            "%s", ui->lineEditUsername3->text().toStdString().c_str());
    snprintf(cfg.stDialParam[mnc][2].ucUserPasswd,
            sizeof(cfg.stDialParam[mnc][2].ucUserPasswd),
            "%s", ui->lineEditPassword3->text().toStdString().c_str());

    snprintf(cfg.stDialParam[mnc][3].ucAPNName,
            sizeof(cfg.stDialParam[mnc][3].ucAPNName),
            "%s", ui->lineEditAPN4->text().toStdString().c_str());
    snprintf(cfg.stDialParam[mnc][3].ucUserName,
            sizeof(cfg.stDialParam[mnc][3].ucUserName),
            "%s", ui->lineEditUsername4->text().toStdString().c_str());
    snprintf(cfg.stDialParam[mnc][3].ucUserPasswd,
            sizeof(cfg.stDialParam[mnc][3].ucUserPasswd),
            "%s", ui->lineEditPassword4->text().toStdString().c_str());

    snprintf(cfg.stDialParam[mnc][4].ucAPNName,
            sizeof(cfg.stDialParam[mnc][4].ucAPNName),
            "%s", ui->lineEditAPN5->text().toStdString().c_str());
    snprintf(cfg.stDialParam[mnc][4].ucUserName,
            sizeof(cfg.stDialParam[mnc][4].ucUserName),
            "%s", ui->lineEditUsername5->text().toStdString().c_str());
    snprintf(cfg.stDialParam[mnc][4].ucUserPasswd,
            sizeof(cfg.stDialParam[mnc][4].ucUserPasswd),
            "%s", ui->lineEditPassword5->text().toStdString().c_str());
}


void MainWindow::on_pushButtonOpen_clicked()
{
    QString path = QFileDialog::getOpenFileName(this);

    if (!path.isNull()) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);

        if (file.size() != sizeof(cfg)) {
            file.close();
            return;
        }

        QDataStream in(&file);
        in.readRawData((char *)&cfg, sizeof(cfg));

        file.close();

        filename = path;
        setWindowTitle(programName + " - " + filename);
        lastMnc = 0;
        ui->comboBoxNetOperator->setCurrentIndex(0);
        syncParamToForm(0);

        ui->statusBar->showMessage("打开文件成功", 3000);
    }
}

void MainWindow::on_pushButtonSave_clicked()
{
    if (!filename.isNull()) {
        syncFormToParam(ui->comboBoxNetOperator->currentIndex());

        QFile file(filename);
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out.writeRawData((char *)&cfg, sizeof(cfg));
        file.close();

        ui->statusBar->showMessage("保存文件成功", 3000);
    } else {
        on_pushButtonSaveAs_clicked();
    }
}

void MainWindow::on_pushButtonSaveAs_clicked()
{
    QString path = QFileDialog::getSaveFileName(this);
    if (!path.isNull()) {
        filename = path;
        setWindowTitle(programName + " - " + filename);
        on_pushButtonSave_clicked();
    }
}

void MainWindow::on_comboBoxNetOperator_currentIndexChanged(int index)
{
    if (index != lastMnc) {
        syncFormToParam(lastMnc);
        syncParamToForm(index);
        lastMnc = index;
    }
}
