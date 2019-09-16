#include "decryptor.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QProcess>


#if 0
const static char g_enhdr[36] = {
    0x17, 0xDA, 0x5F, 0xA0, 0x16, 0x33, 0xCD, 0x9A, 0xBC, 0x5B, 0x6C, 0xBF, 0x14, 0xAA, 0x1E, 0x33,
    0x76, 0x66, 0xBA, 0x66, 0x7E, 0x0B, 0xAC, 0xC1, 0xC0, 0xC7, 0xB1, 0xA1, 0x9F, 0x32, 0x5D, 0x61,
    0x29, 0x2B, 0x3A, 0x22
};
#else
const static char g_enhdr[4] = {
    0x17, 0xDA, 0x5F, 0xA0
};
#endif

bool Decryptor::isEncryptFile(const QString filePath)
{
    QFile file(filePath);
    char buffer[sizeof(g_enhdr)];
    qint64 len = 0;

    memset(buffer, 0, sizeof(buffer));
    file.open(QIODevice::ReadOnly);
    len = file.read(buffer, sizeof(buffer));
    file.close();

    if (len != sizeof(buffer))
        return false;

    return 0 == memcmp(buffer, g_enhdr, sizeof(buffer)) ? true : false;
}


Decryptor::Decryptor(const QString &programName, const QStringList &fileTypes)
{
    this->programName = programName;
    this->supportFileTypes = fileTypes;
}

Decryptor::~Decryptor()
{
}

QStringList Decryptor::getSupportFileTypes()
{
    return this->supportFileTypes;
}

bool Decryptor::decryptFile(const QString filePath)
{
    QFileInfo fileInfo(filePath);
    int err = -1;

    //qDebug() << "正在解密：" << filePath;

    if (! fileInfo.exists())
        return false;

    if (! Decryptor::isEncryptFile(filePath)) {
        qDebug() << "文件未加密：" << filePath;
        return false;
    }

    err = QProcess::execute(this->programName + " -f \"" + filePath + "\"\r\n");

#if 0
    QProcess process(this);
    process.setWorkingDirectory(fileInfo.path());
    process.start("cmd.exe");

    QString cmd = this->programName + " -f \"" + filePath + "\"\r\n";
    process.write(QTextCodec::codecForName("GB18030")->fromUnicode(cmd).constData());

    process.write("exit\r\n");
    process.waitForFinished();
    process.close();
#endif

    if (err < 0)
        return false;

    return true;
}

