#pragma once

#include "qt_console/qconsole.h"


class cliparser : public QConsole {
	Q_OBJECT

public:
	cliparser(QWidget *parent = NULL, const QString &welcomeText = "");

signals:
	void signal_load(const std::string& stkName, const std::string& stgName, const std::string& orderTblName);
	
    void signal_centerOn_day(int date);
    void signal_crossOn_day(int date);
public slots:
	void handlCommand(const QString &command);

private:
	void handlCommand_load(QStringList& split_cmd);
	void handlCommand_on(QStringList& split_cmd);
	void handlCommand_cross(QStringList& split_cmd);
private:
	QString _stkName = "";
	QString _stgName = "";
	QString _orderTblName = "";
};