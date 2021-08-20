#include "qt_sqlite_viewer/ddwidget.h"
#include "ui_ddwidget.h"


namespace S4{
namespace QT{
    
DDwidget::DDwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DDwidget)
{
    ui->setupUi(this);
}

DDwidget::~DDwidget()
{
    delete ui;
}


void DDwidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain")){
        qDebug() << "dragEnterEvent";
        event->acceptProposedAction();
    }
}

void DDwidget::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasUrls())
    {
        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();

        QString dbPath = urlList.first().toLocalFile();

        if(fileCanBeOpened(dbPath)){
            emit filePathRecived(dbPath);
        }
    }
}

void DDwidget::mousePressEvent(QMouseEvent *)
{
    qDebug() << "mousePressEvent";
    QString dbPath = QFileDialog::getOpenFileName(nullptr,tr("Open file"), "/home", tr("Any files (*)"));


    if(fileCanBeOpened(dbPath)){
        ui->label->setText(dbPath);
        emit filePathRecived(dbPath);
    }
}


} // namespace QT
} // namespace S4