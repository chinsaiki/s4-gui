#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_SnapViewer/s4SnapViewerWidgetDsxDB.h"
#include "qt_SnapViewer/s4SnapInstrument.h"

#include "common/s4logger.h"
#include "qt_common/Utils.h"
#include "jsonTypes/nw_load_instrument_t.h"
#include "db_sqlite/db.h"
#include "data/s4infSnap5x.h"

#include <QSplitter>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QStyleFactory>
#include <QSortFilterProxyModel>
#include <QTableWidget>
#include <QGridLayout>


using namespace std;

namespace S4 {
namespace QT {

//CREATE_LOCAL_LOGGER("qt_SnapViewer")

#define DSX_DB_PREAMBLE QStringLiteral("DSX_")

s4SnapViewerWidgetDsxDB::s4SnapViewerWidgetDsxDB(QWidget *parent) :
    s4SnapViewerWidget(parent)
{   

	_treeView = new QTreeView(this);
	_treeView->setStyle(QStyleFactory::create("windows"));
	_treeView->setSortingEnabled(true);
	_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_treeView->setMaximumWidth(300);

	_tabWidget = new QTabWidget(this);
	_tabWidget->setTabsClosable(true);
	//网格分割
	QGridLayout* pLayout = new QGridLayout(this);
	pLayout->addWidget(_treeView, 0, 0);
	pLayout->addWidget(_tabWidget, 0, 2);
	pLayout->setColumnStretch(0, 1);			//0列的拉伸系数
	pLayout->setColumnStretch(2, 3);			//6列的拉伸系数 (0=不拉伸)

	setLayout(pLayout);

	connect(_tabWidget, &QTabWidget::tabCloseRequested, this, &s4SnapViewerWidgetDsxDB::closeSnapTab);

	connect(_treeView, &QTreeView::doubleClicked, this, &s4SnapViewerWidgetDsxDB::dbTree_doubleClicked);
}

//打开一个DSX sqlite数据库，并把table添加到dbTree中
void s4SnapViewerWidgetDsxDB::onOpenDsxDB()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open snap database"), "../db", tr("DSX sqlite db files (*.db)"));

	if (!fileCanBeOpened(path)) {
		QMessageBox::warning(NULL, "warning", "file is not readable!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}

	QFileInfo fileInfo;
	fileInfo = QFileInfo(path);

	try {

		sqlite::DB_t snap_db(path.toStdString());

		std::set<std::string> dates = snap_db.get_table_list();

		//读取一页，检查数据结构 【这里不做】
		// if (dates.size() == 0) {
		// 	LCL_ERR("No snap to read out!");
		// 	QMessageBox::warning(NULL, "warning", "snap db is empty!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		// 	return;
		// }

		// S4::sqlite::dsx_snap_t_dbTbl snap_tbl;
		// std::vector<dsx_snap_t> snaps;

		// snap_db.read_table_v2(&snap_tbl, dates.back(), snaps);
		// LCL_INFO("{} snaps has been loaded:", snaps.size());

		std::vector<QString> Qdates;
		for (auto& d :dates){
			Qdates.push_back(QString::fromStdString(d));
		}
		newTree(DSX_DB_PREAMBLE + fileInfo.baseName(), Qdates);

		_db_list[(DSX_DB_PREAMBLE + fileInfo.baseName()).toStdString()] = path.toStdString();
	}
	catch (std::exception& e) {
		QMessageBox::warning(NULL, "warning", "load snap db error: " + QString::fromStdString(e.what()) + "!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
}

void s4SnapViewerWidgetDsxDB::dbTree_doubleClicked(const QModelIndex& index) {
	if (!index.parent().isValid()) return;
	//if (!index.parent().parent().isValid()) return;

	QString tableName = index.data().toString();
	QString dbName = index.parent().data().toString();

	QString str;
	str += QStringLiteral("当前选中：%1/%2\n").arg(dbName).arg(tableName);

	if (dbName.indexOf(DSX_DB_PREAMBLE) == 0) {
		openDsxSnapTab(dbName.toStdString(), tableName.toStdString());
	}
}

void s4SnapViewerWidgetDsxDB::openDsxSnapTab(const std::string& db_name, const std::string& table_name)
{
	if (_db_list.count(db_name) == 0) {
		return;
	}

	std::string path = _db_list.at(db_name);

	try {
		sqlite::DB_t snap_db(path);
		std::set<std::string> dates = snap_db.get_table_list();
		if (dates.count(table_name) == 0) {
			return;
		}

		infSnap5xQ_ptr pSnapQ = std::make_shared<infSnap5xQ_t>();
		pSnapQ->fromDB(path, table_name, true);

		std::string snap_tab_name_s = db_name + "-" + table_name;
		QString snap_tab_name(snap_tab_name_s.c_str());

		//create new data
		//QTableView* levels_tv = new QTableView(this);
		//// snapTableModel_level* levels = new snapTableModel_level(5, levels_tv);
		//snapTableModel_snapInfo* levels = new snapTableModel_snapInfo(levels_tv);
		//itemFormatDelegate* delegate = new itemFormatDelegate(levels_tv);
		//levels_tv->setModel(levels);
		//levels_tv->setItemDelegate(delegate);
		//levels_tv->setSelectionBehavior(QAbstractItemView::SelectRows);
		//connect(levels_tv, SIGNAL(clicked(const QModelIndex&)), this, SLOT(nextDsxSnap(const QModelIndex&)));

		snapInstrument* pInstrument = new snapInstrument(5, this);
		openSnapTab(snap_tab_name, pInstrument);

		//存储
		_instrument_info_cargo[snap_tab_name] = { };

		pInstrument->addSnaps(pSnapQ);
	}
	catch (std::exception& e) {
		QMessageBox::warning(NULL, "warning", "read snap table error: " + QString::fromStdString(e.what()) + "!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}


void s4SnapViewerWidgetDsxDB::nextDsxSnap()
{
	int idx = _tabWidget->currentIndex();
	if (idx < 0) return;
	// const QString snap_tab_name = _tabWidget->tabText(idx);


	//QTableView* levels_tv = (QTableWidget*)_tabWidget->currentWidget();
	//QAbstractItemModel* model = levels_tv->model();

	//int row = levels_tv->currentIndex().row();
	//QModelIndex indexCode = model->index(row, 0);
	//const QString side = model->data(indexCode).toString();
	//ui->statusbar->showMessage(side);

	// if (_instrument_info_cargo.count(snap_tab_name)) {
	// 	if (_instrument_info_cargo[snap_tab_name].curse < (int)_instrument_info_cargo[snap_tab_name].snaps.size()) {
	// 		((snapInstrument*)_instrument_view_cargo[snap_tab_name])->addSnaps({ _instrument_info_cargo[snap_tab_name].snaps[_instrument_info_cargo[snap_tab_name].curse++] });
	// 	}
	// }
}

void s4SnapViewerWidgetDsxDB::closeSnapTab(int index)
{
	const QString tabName = _tabWidget->tabText(index);

	auto it = _instrument_info_cargo.find(tabName);
	if (it != _instrument_info_cargo.end()) {
		_instrument_info_cargo.erase(tabName);
	}
	s4SnapViewerWidget::closeSnapTab(index);

}

s4SnapViewerWidgetDsxDB::~s4SnapViewerWidgetDsxDB()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
