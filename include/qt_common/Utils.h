#ifndef UTILS_H
#define UTILS_H
#include <QFile>
#include <QDebug>
#include <QString>

namespace S4{

namespace QT{

inline bool fileCanBeOpened(const QString &path){
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()){
        qDebug() << "File can't be opened, path: " << path;
        return false;
    }

    return  true;
}

}
//1000.0 -> 1,000.0
std::string fmtStringNumComma(const std::string& num);

inline void
test_fmtStringNumComma(void){
    printf ("%s\n", fmtStringNumComma("0.31415").c_str());
    printf ("%s\n", fmtStringNumComma("3.1415").c_str());
    printf ("%s\n", fmtStringNumComma("31.415").c_str());
    printf ("%s\n", fmtStringNumComma("314.15").c_str());
    printf ("%s\n", fmtStringNumComma("3141.5").c_str());
    printf ("%s\n", fmtStringNumComma("31415").c_str());
    printf ("%s\n", fmtStringNumComma("-0.31415").c_str());
    printf ("%s\n", fmtStringNumComma("-3.1415").c_str());
    printf ("%s\n", fmtStringNumComma("-31.415").c_str());
    printf ("%s\n", fmtStringNumComma("-314.15").c_str());
    printf ("%s\n", fmtStringNumComma("-3141.5").c_str());
    printf ("%s\n", fmtStringNumComma("-31415").c_str());
    printf ("%s\n", fmtStringNumComma("0").c_str());

}

}
#endif // UTILS_H
