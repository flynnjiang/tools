#include <QFileDialog>
#include <QDate>
#include <QTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"

static const QDate buildDate = QLocale( QLocale::English ).toDate( QString( __DATE__ ).replace( "  ", " 0" ), "MMM dd yyyy");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    memset(&cfg, 0, sizeof(cfg));

    programName = "通信扩展参数编辑器（内蒙版" + buildDate.toString("yyyyMMdd") +"）";
    setWindowTitle(programName);

    lastMnc = 0;
    syncParamToForm(0);

    set2GParamEnabled(false);
    set3GParamEnabled(false);
    set4GParamEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sync2GParamTo34G()
{
    // 同步到3G
    ui->checkBox3G->setChecked(ui->checkBox2G->isChecked());

    ui->lineEdit3GApn->setText(ui->lineEdit2GApn->text());
    ui->lineEdit3GUsername->setText(ui->lineEdit2GUsername->text());
    ui->lineEdit3GPassword->setText(ui->lineEdit2GPassword->text());

    ui->lineEdit3GSeverIP1->setText(ui->lineEdit2GSeverIP1->text());
    ui->lineEdit3GSeverIP2->setText(ui->lineEdit2GSeverIP2->text());
    ui->lineEdit3GSeverIP3->setText(ui->lineEdit2GSeverIP3->text());
    ui->lineEdit3GProxyIP->setText(ui->lineEdit2GProxyIP->text());

    ui->lineEdit3GSeverPort1->setText(ui->lineEdit2GSeverPort1->text());
    ui->lineEdit3GSeverPort2->setText(ui->lineEdit2GSeverPort2->text());
    ui->lineEdit3GSeverPort3->setText(ui->lineEdit2GSeverPort3->text());
    ui->lineEdit3GProxyPort->setText(ui->lineEdit2GProxyPort->text());

    // 同步到4G
    ui->checkBox4G->setChecked(ui->checkBox2G->isChecked());

    ui->lineEdit4GApn->setText(ui->lineEdit2GApn->text());
    ui->lineEdit4GUsername->setText(ui->lineEdit2GUsername->text());
    ui->lineEdit4GPassword->setText(ui->lineEdit2GPassword->text());

    ui->lineEdit4GSeverIP1->setText(ui->lineEdit2GSeverIP1->text());
    ui->lineEdit4GSeverIP2->setText(ui->lineEdit2GSeverIP2->text());
    ui->lineEdit4GSeverIP3->setText(ui->lineEdit2GSeverIP3->text());
    ui->lineEdit4GProxyIP->setText(ui->lineEdit2GProxyIP->text());

    ui->lineEdit4GSeverPort1->setText(ui->lineEdit2GSeverPort1->text());
    ui->lineEdit4GSeverPort2->setText(ui->lineEdit2GSeverPort2->text());
    ui->lineEdit4GSeverPort3->setText(ui->lineEdit2GSeverPort3->text());
    ui->lineEdit4GProxyPort->setText(ui->lineEdit2GProxyPort->text());

}

void MainWindow::set2GParamEnabled(bool enable)
{
    ui->lineEdit2GApn->setEnabled(enable);
    ui->lineEdit2GUsername->setEnabled(enable);
    ui->lineEdit2GPassword->setEnabled(enable);

    ui->lineEdit2GSeverIP1->setEnabled(enable);
    ui->lineEdit2GSeverIP2->setEnabled(enable);
    ui->lineEdit2GSeverIP3->setEnabled(enable);
    ui->lineEdit2GProxyIP->setEnabled(enable);
    ui->lineEdit2GSeverPort1->setEnabled(enable);
    ui->lineEdit2GSeverPort2->setEnabled(enable);
    ui->lineEdit2GSeverPort3->setEnabled(enable);
    ui->lineEdit2GProxyPort->setEnabled(enable);

}

void MainWindow::set3GParamEnabled(bool enable)
{
    ui->lineEdit3GApn->setEnabled(enable);
    ui->lineEdit3GUsername->setEnabled(enable);
    ui->lineEdit3GPassword->setEnabled(enable);

    ui->lineEdit3GSeverIP1->setEnabled(enable);
    ui->lineEdit3GSeverIP2->setEnabled(enable);
    ui->lineEdit3GSeverIP3->setEnabled(enable);
    ui->lineEdit3GProxyIP->setEnabled(enable);
    ui->lineEdit3GSeverPort1->setEnabled(enable);
    ui->lineEdit3GSeverPort2->setEnabled(enable);
    ui->lineEdit3GSeverPort3->setEnabled(enable);
    ui->lineEdit3GProxyPort->setEnabled(enable);

}

void MainWindow::set4GParamEnabled(bool enable)
{
    ui->lineEdit4GApn->setEnabled(enable);
    ui->lineEdit4GUsername->setEnabled(enable);
    ui->lineEdit4GPassword->setEnabled(enable);

    ui->lineEdit4GSeverIP1->setEnabled(enable);
    ui->lineEdit4GSeverIP2->setEnabled(enable);
    ui->lineEdit4GSeverIP3->setEnabled(enable);
    ui->lineEdit4GProxyIP->setEnabled(enable);
    ui->lineEdit4GSeverPort1->setEnabled(enable);
    ui->lineEdit4GSeverPort2->setEnabled(enable);
    ui->lineEdit4GSeverPort3->setEnabled(enable);
    ui->lineEdit4GProxyPort->setEnabled(enable);
}

void MainWindow::on_checkBox2G_stateChanged(int arg1)
{
    set2GParamEnabled(Qt::Checked == arg1);

    if (ui->checkBoxSameParam->isChecked()) {
        ui->checkBox3G->setChecked(arg1);
        ui->checkBox4G->setChecked(arg1);
    }
}

void MainWindow::on_checkBox3G_stateChanged(int arg1)
{
    set3GParamEnabled(Qt::Checked == arg1 && !ui->checkBoxSameParam->isChecked());
}

void MainWindow::on_checkBox4G_stateChanged(int arg1)
{
    set4GParamEnabled(Qt::Checked == arg1 && !ui->checkBoxSameParam->isChecked());
}

void MainWindow::on_checkBoxSameParam_stateChanged(int arg1)
{
    if (Qt::Checked == arg1) {
        sync2GParamTo34G();

        ui->checkBox3G->setEnabled(false);
        set3GParamEnabled(false);

        ui->checkBox4G->setEnabled(false);
        set4GParamEnabled(false);
    } else {
        ui->checkBox3G->setEnabled(true);
        if (ui->checkBox3G->isChecked())
            set3GParamEnabled(true);

        ui->checkBox4G->setEnabled(true);
        if (ui->checkBox4G->isChecked())
            set4GParamEnabled(true);
    }
}


void MainWindow::on_lineEdit2GApn_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GApn->setText(arg1);
        ui->lineEdit4GApn->setText(arg1);
    }
}

void MainWindow::on_lineEdit2GUsername_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GUsername->setText(arg1);
        ui->lineEdit4GUsername->setText(arg1);
    }
}

void MainWindow::on_lineEdit2GPassword_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GPassword->setText(arg1);
        ui->lineEdit4GPassword->setText(arg1);
    }
}

void MainWindow::on_lineEdit2GSeverIP1_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GSeverIP1->setText(arg1);
        ui->lineEdit4GSeverIP1->setText(arg1);
    }
}

void MainWindow::on_lineEdit2GSeverPort1_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GSeverPort1->setText(arg1);
        ui->lineEdit4GSeverPort1->setText(arg1);
    }
}

void MainWindow::on_lineEdit2GSeverIP2_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GSeverIP2->setText(arg1);
        ui->lineEdit4GSeverIP2->setText(arg1);
    }
}

void MainWindow::on_lineEdit2GSeverPort2_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GSeverPort2->setText(arg1);
        ui->lineEdit4GSeverPort2->setText(arg1);
    }
}

void MainWindow::on_lineEdit2GSeverIP3_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GSeverIP3->setText(arg1);
        ui->lineEdit4GSeverIP3->setText(arg1);
    }
}

void MainWindow::on_lineEdit2GSeverPort3_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GSeverPort3->setText(arg1);
        ui->lineEdit4GSeverPort3->setText(arg1);
    }
}

void MainWindow::on_lineEdit2GProxyIP_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GProxyIP->setText(arg1);
        ui->lineEdit4GProxyIP->setText(arg1);
    }
}

void MainWindow::on_lineEdit2GProxyPort_textChanged(const QString &arg1)
{
    if (ui->checkBoxSameParam->isChecked()) {
        ui->lineEdit3GProxyPort->setText(arg1);
        ui->lineEdit4GProxyPort->setText(arg1);
    }
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


void MainWindow::syncFormToParam(int mnc)
{
    cfg.iSetNetMode = ui->comboBoxNetMode->currentIndex();
    cfg.lockAfterLogin = ui->checkBoxLockMode->isChecked();

    //cfg.bSameSysinfoMNC[mnc] = ui->checkBoxSameParam->isChecked();

    cfg.stSysinfoParam[mnc][0].bParamValid = ui->checkBox2G->isChecked();
    snprintf(cfg.stSysinfoParam[mnc][0].st_Dialfile.ucAPNName,
            sizeof(cfg.stSysinfoParam[mnc][0].st_Dialfile.ucAPNName),
            "%s", ui->lineEdit2GApn->text().toStdString().c_str());
    snprintf(cfg.stSysinfoParam[mnc][0].st_Dialfile.ucUserName,
            sizeof(cfg.stSysinfoParam[mnc][0].st_Dialfile.ucUserName),
            "%s", ui->lineEdit2GUsername->text().toStdString().c_str());
    snprintf(cfg.stSysinfoParam[mnc][0].st_Dialfile.ucUserPasswd,
            sizeof(cfg.stSysinfoParam[mnc][0].st_Dialfile.ucUserPasswd),
            "%s", ui->lineEdit2GPassword->text().toStdString().c_str());
    stringToIp(ui->lineEdit2GSeverIP1->text(), cfg.stSysinfoParam[mnc][0].st_NetParam.stHostIP1);
    stringToIp(ui->lineEdit2GSeverIP2->text(), cfg.stSysinfoParam[mnc][0].st_NetParam.stHostIP2);
    stringToIp(ui->lineEdit2GSeverIP3->text(), cfg.stSysinfoParam[mnc][0].st_NetParam.stHostIP3);
    stringToIp(ui->lineEdit2GProxyIP->text(), cfg.stSysinfoParam[mnc][0].st_NetParam.stMGateway);
    cfg.stSysinfoParam[mnc][0].st_NetParam.iHostPort1 = ui->lineEdit2GSeverPort1->text().toUShort();
    cfg.stSysinfoParam[mnc][0].st_NetParam.iHostPort2 = ui->lineEdit2GSeverPort2->text().toUShort();
    cfg.stSysinfoParam[mnc][0].st_NetParam.iHostPort3 = ui->lineEdit2GSeverPort3->text().toUShort();
    cfg.stSysinfoParam[mnc][0].st_NetParam.iGatewayPort = ui->lineEdit2GProxyPort->text().toUShort();

    //cfg.stSysinfoParam[mnc][0].st_extParam.iHeartBeat = ui->lineEdit2GHeartInt->text().toInt();
    //cfg.stSysinfoParam[mnc][0].st_extParam.iRedialInterval = ui->lineEdit2GRedialInt->text().toInt();
    //cfg.stSysinfoParam[mnc][0].st_extParam.iRedialCnt = ui->lineEdit2GRedialCnt->text().toInt();


    cfg.stSysinfoParam[mnc][1].bParamValid = ui->checkBox3G->isChecked();
    snprintf(cfg.stSysinfoParam[mnc][1].st_Dialfile.ucAPNName,
            sizeof(cfg.stSysinfoParam[mnc][1].st_Dialfile.ucAPNName),
            "%s", ui->lineEdit3GApn->text().toStdString().c_str());
    snprintf(cfg.stSysinfoParam[mnc][1].st_Dialfile.ucUserName,
            sizeof(cfg.stSysinfoParam[mnc][1].st_Dialfile.ucUserName),
            "%s", ui->lineEdit3GUsername->text().toStdString().c_str());
    snprintf(cfg.stSysinfoParam[mnc][1].st_Dialfile.ucUserPasswd,
            sizeof(cfg.stSysinfoParam[mnc][1].st_Dialfile.ucUserPasswd),
            "%s", ui->lineEdit3GPassword->text().toStdString().c_str());
    stringToIp(ui->lineEdit3GSeverIP1->text(), cfg.stSysinfoParam[mnc][1].st_NetParam.stHostIP1);
    stringToIp(ui->lineEdit3GSeverIP2->text(), cfg.stSysinfoParam[mnc][1].st_NetParam.stHostIP2);
    stringToIp(ui->lineEdit3GSeverIP3->text(), cfg.stSysinfoParam[mnc][1].st_NetParam.stHostIP3);
    stringToIp(ui->lineEdit3GProxyIP->text(), cfg.stSysinfoParam[mnc][1].st_NetParam.stMGateway);
    cfg.stSysinfoParam[mnc][1].st_NetParam.iHostPort1 = ui->lineEdit3GSeverPort1->text().toUShort();
    cfg.stSysinfoParam[mnc][1].st_NetParam.iHostPort2 = ui->lineEdit3GSeverPort2->text().toUShort();
    cfg.stSysinfoParam[mnc][1].st_NetParam.iHostPort3 = ui->lineEdit3GSeverPort3->text().toUShort();
    cfg.stSysinfoParam[mnc][1].st_NetParam.iGatewayPort = ui->lineEdit3GProxyPort->text().toUShort();

    //cfg.stSysinfoParam[mnc][1].st_extParam.iHeartBeat = ui->lineEdit3GHeartInt->text().toInt();
    //cfg.stSysinfoParam[mnc][1].st_extParam.iRedialInterval = ui->lineEdit3GRedialInt->text().toInt();
    //cfg.stSysinfoParam[mnc][1].st_extParam.iRedialCnt = ui->lineEdit3GRedialCnt->text().toInt();


    cfg.stSysinfoParam[mnc][2].bParamValid = ui->checkBox4G->isChecked();
    snprintf(cfg.stSysinfoParam[mnc][2].st_Dialfile.ucAPNName,
            sizeof(cfg.stSysinfoParam[mnc][2].st_Dialfile.ucAPNName),
            "%s", ui->lineEdit4GApn->text().toStdString().c_str());
    snprintf(cfg.stSysinfoParam[mnc][2].st_Dialfile.ucUserName,
            sizeof(cfg.stSysinfoParam[mnc][2].st_Dialfile.ucUserName),
            "%s", ui->lineEdit4GUsername->text().toStdString().c_str());
    snprintf(cfg.stSysinfoParam[mnc][2].st_Dialfile.ucUserPasswd,
            sizeof(cfg.stSysinfoParam[mnc][2].st_Dialfile.ucUserPasswd),
            "%s", ui->lineEdit4GPassword->text().toStdString().c_str());
    stringToIp(ui->lineEdit4GSeverIP1->text(), cfg.stSysinfoParam[mnc][2].st_NetParam.stHostIP1);
    stringToIp(ui->lineEdit4GSeverIP2->text(), cfg.stSysinfoParam[mnc][2].st_NetParam.stHostIP2);
    stringToIp(ui->lineEdit4GSeverIP3->text(), cfg.stSysinfoParam[mnc][2].st_NetParam.stHostIP3);
    stringToIp(ui->lineEdit4GProxyIP->text(), cfg.stSysinfoParam[mnc][2].st_NetParam.stMGateway);
    cfg.stSysinfoParam[mnc][2].st_NetParam.iHostPort1 = ui->lineEdit4GSeverPort1->text().toUShort();
    cfg.stSysinfoParam[mnc][2].st_NetParam.iHostPort2 = ui->lineEdit4GSeverPort2->text().toUShort();
    cfg.stSysinfoParam[mnc][2].st_NetParam.iHostPort3 = ui->lineEdit4GSeverPort3->text().toUShort();
    cfg.stSysinfoParam[mnc][2].st_NetParam.iGatewayPort = ui->lineEdit4GProxyPort->text().toUShort();

    //cfg.stSysinfoParam[mnc][2].st_extParam.iHeartBeat = ui->lineEdit4GHeartInt->text().toInt();
    //cfg.stSysinfoParam[mnc][2].st_extParam.iRedialInterval = ui->lineEdit4GRedialInt->text().toInt();
    //cfg.stSysinfoParam[mnc][2].st_extParam.iRedialCnt = ui->lineEdit4GRedialCnt->text().toInt();
}

void MainWindow::syncParamToForm(int mnc)
{
    ui->comboBoxNetMode->setCurrentIndex(cfg.iSetNetMode);
    ui->checkBoxLockMode->setChecked(1 == cfg.lockAfterLogin);

    //ui->checkBoxSameParam->setChecked(1 == cfg.bSameSysinfoMNC[mnc]);

    ui->checkBox2G->setChecked(1 == cfg.stSysinfoParam[mnc][0].bParamValid);
    ui->lineEdit2GApn->setText(cfg.stSysinfoParam[mnc][0].st_Dialfile.ucAPNName);
    ui->lineEdit2GUsername->setText(cfg.stSysinfoParam[mnc][0].st_Dialfile.ucUserName);
    ui->lineEdit2GPassword->setText(cfg.stSysinfoParam[mnc][0].st_Dialfile.ucUserPasswd);
    ui->lineEdit2GSeverIP1->setText(ipToString(cfg.stSysinfoParam[mnc][0].st_NetParam.stHostIP1));
    ui->lineEdit2GSeverIP2->setText(ipToString(cfg.stSysinfoParam[mnc][0].st_NetParam.stHostIP2));
    ui->lineEdit2GSeverIP3->setText(ipToString(cfg.stSysinfoParam[mnc][0].st_NetParam.stHostIP3));
    ui->lineEdit2GProxyIP->setText(ipToString(cfg.stSysinfoParam[mnc][0].st_NetParam.stMGateway));
    ui->lineEdit2GSeverPort1->setText(QString::number(cfg.stSysinfoParam[mnc][0].st_NetParam.iHostPort1));
    ui->lineEdit2GSeverPort2->setText(QString::number(cfg.stSysinfoParam[mnc][0].st_NetParam.iHostPort2));
    ui->lineEdit2GSeverPort3->setText(QString::number(cfg.stSysinfoParam[mnc][0].st_NetParam.iHostPort3));
    ui->lineEdit2GProxyPort->setText(QString::number(cfg.stSysinfoParam[mnc][0].st_NetParam.iGatewayPort));
    //ui->lineEdit2GHeartInt->setText(QString::number(cfg.stSysinfoParam[mnc][0].st_extParam.iHeartBeat));
    //ui->lineEdit2GRedialInt->setText(QString::number(cfg.stSysinfoParam[mnc][0].st_extParam.iRedialInterval));
    //ui->lineEdit2GRedialCnt->setText(QString::number(cfg.stSysinfoParam[mnc][0].st_extParam.iRedialCnt));

    ui->checkBox3G->setChecked(1 == cfg.stSysinfoParam[mnc][1].bParamValid);
    ui->lineEdit3GApn->setText(cfg.stSysinfoParam[mnc][1].st_Dialfile.ucAPNName);
    ui->lineEdit3GUsername->setText(cfg.stSysinfoParam[mnc][1].st_Dialfile.ucUserName);
    ui->lineEdit3GPassword->setText(cfg.stSysinfoParam[mnc][1].st_Dialfile.ucUserPasswd);
    ui->lineEdit3GSeverIP1->setText(ipToString(cfg.stSysinfoParam[mnc][1].st_NetParam.stHostIP1));
    ui->lineEdit3GSeverIP2->setText(ipToString(cfg.stSysinfoParam[mnc][1].st_NetParam.stHostIP2));
    ui->lineEdit3GSeverIP3->setText(ipToString(cfg.stSysinfoParam[mnc][1].st_NetParam.stHostIP3));
    ui->lineEdit3GProxyIP->setText(ipToString(cfg.stSysinfoParam[mnc][1].st_NetParam.stMGateway));
    ui->lineEdit3GSeverPort1->setText(QString::number(cfg.stSysinfoParam[mnc][1].st_NetParam.iHostPort1));
    ui->lineEdit3GSeverPort2->setText(QString::number(cfg.stSysinfoParam[mnc][1].st_NetParam.iHostPort2));
    ui->lineEdit3GSeverPort3->setText(QString::number(cfg.stSysinfoParam[mnc][1].st_NetParam.iHostPort3));
    ui->lineEdit3GProxyPort->setText(QString::number(cfg.stSysinfoParam[mnc][1].st_NetParam.iGatewayPort));
    //ui->lineEdit3GHeartInt->setText(QString::number(cfg.stSysinfoParam[mnc][1].st_extParam.iHeartBeat));
    //ui->lineEdit3GRedialInt->setText(QString::number(cfg.stSysinfoParam[mnc][1].st_extParam.iRedialInterval));
    //ui->lineEdit3GRedialCnt->setText(QString::number(cfg.stSysinfoParam[mnc][1].st_extParam.iRedialCnt));

    ui->checkBox4G->setChecked(1 == cfg.stSysinfoParam[mnc][2].bParamValid);
    ui->lineEdit4GApn->setText(cfg.stSysinfoParam[mnc][2].st_Dialfile.ucAPNName);
    ui->lineEdit4GUsername->setText(cfg.stSysinfoParam[mnc][2].st_Dialfile.ucUserName);
    ui->lineEdit4GPassword->setText(cfg.stSysinfoParam[mnc][2].st_Dialfile.ucUserPasswd);
    ui->lineEdit4GSeverIP1->setText(ipToString(cfg.stSysinfoParam[mnc][2].st_NetParam.stHostIP1));
    ui->lineEdit4GSeverIP2->setText(ipToString(cfg.stSysinfoParam[mnc][2].st_NetParam.stHostIP2));
    ui->lineEdit4GSeverIP3->setText(ipToString(cfg.stSysinfoParam[mnc][2].st_NetParam.stHostIP3));
    ui->lineEdit4GProxyIP->setText(ipToString(cfg.stSysinfoParam[mnc][2].st_NetParam.stMGateway));
    ui->lineEdit4GSeverPort1->setText(QString::number(cfg.stSysinfoParam[mnc][2].st_NetParam.iHostPort1));
    ui->lineEdit4GSeverPort2->setText(QString::number(cfg.stSysinfoParam[mnc][2].st_NetParam.iHostPort2));
    ui->lineEdit4GSeverPort3->setText(QString::number(cfg.stSysinfoParam[mnc][2].st_NetParam.iHostPort3));
    ui->lineEdit4GProxyPort->setText(QString::number(cfg.stSysinfoParam[mnc][2].st_NetParam.iGatewayPort));
    //ui->lineEdit4GHeartInt->setText(QString::number(cfg.stSysinfoParam[mnc][2].st_extParam.iHeartBeat));
    //ui->lineEdit4GRedialInt->setText(QString::number(cfg.stSysinfoParam[mnc][2].st_extParam.iRedialInterval));
    //ui->lineEdit4GRedialCnt->setText(QString::number(cfg.stSysinfoParam[mnc][2].st_extParam.iRedialCnt));

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


