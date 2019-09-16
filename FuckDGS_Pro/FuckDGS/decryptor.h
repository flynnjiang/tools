#ifndef DECRYPTOR_H
#define DECRYPTOR_H

#include <QString>
#include <QStringList>
#include <QVector>

class Decryptor
{
public:
    Decryptor(const QString &programName, const QStringList &fileTypes);
    ~Decryptor();

    static bool isEncryptFile(const QString filePath);

    QStringList getSupportFileTypes();
    bool decryptFile(const QString filePath);


private:
    QString programName;
    QStringList supportFileTypes;
};




#endif // DECRYPTOR_H
