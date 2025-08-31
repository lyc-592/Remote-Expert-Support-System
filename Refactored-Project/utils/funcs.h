#ifndef FUNCS_H
#define FUNCS_H

#include <QCryptographicHash>

// 密码加密函数：为避免密码在传输过程中泄露，所有输入的密码都应该经过一次这个函数；因此数据库中用户密码也经过了哈希
static const QString encryptPassword(const QString &password)
{
    // 使用SHA-256加密密码
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}
#endif // FUNCS_H
