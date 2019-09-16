#include <QApplication>
#include <QCommandLineParser>
#include <QtDebug>
#include <QFileDialog>
#include <QProcess>
#include <QTextCodec>
#include <QThread>


bool decryptFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);

    if (! fileInfo.exists())
    {
        return false;
    }

    QString tmpName = fileInfo.completeBaseName() + "_FUCKED";
    QString tmpPath = fileInfo.path() + "/" + tmpName;

    QString decName = fileInfo.completeBaseName() + "_FUCKED." + fileInfo.suffix();
    QString decPath = fileInfo.path() + "/" + decName;

    QFile(tmpPath).remove();
    if (! QFile::copy(filePath, tmpPath))
        return false;

    QThread::msleep(300);

    QString cmd = "DEL \"" + fileInfo.fileName() + "\"\r\n"
                + "MOVE /Y \"" + tmpName + "\" \"" + fileInfo.fileName() + "\"\r\n";
    QProcess process(NULL);
    process.setWorkingDirectory(fileInfo.path());
    process.start("cmd.exe");
    process.write(QTextCodec::codecForName("GB18030")->fromUnicode(cmd).constData());
    process.write("exit\r\n");
    process.waitForFinished();
    process.close();

    return true;
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineOption op1("f");
    op1.setValueName("fpath");

    QCommandLineParser parser;
    parser.addOption(op1);
    parser.process(a);

    return decryptFile(parser.value(op1)) ? 0 : -1;
}
