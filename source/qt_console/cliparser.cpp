#include "qt_console/cliparser.h"
#include "common/s4mktCode.h"
#include "types/s4convertors.h"

cliparser::cliparser(QWidget *parent, const QString &welcomeText)
{
	setCmdColor(Qt::blue);
	setOutColor(Qt::black);
	// 将信号 mySignal() 与槽 mySlot() 相关联
	connect(this, SIGNAL(execCommand(const QString &)), SLOT(handlCommand(const QString &)));

}

void cliparser::handlCommand(const QString &command)
{
	QStringList split_cmd = command.split(" ", QString::SkipEmptyParts);//.toLower()
	if (!split_cmd.size()) {
		printCommandExecutionResults("");
		return;
	}

	if (split_cmd[0] == "ld") {
		handlCommand_load(split_cmd);
	}
    else if (split_cmd[0] == "on"){
		handlCommand_on(split_cmd);
    }
    else if (split_cmd[0] == "cr"){
		handlCommand_cross(split_cmd);
    }
	else {
		printCommandExecutionResults("Error: unknow command!");
	}


	//printCommandExecutionResults(command+" done!");
}


void cliparser::handlCommand_load(QStringList& split_cmd)
{
	bool stg_found = false;
	bool tbl_found = false;
    bool on_found = false;
    std::vector<QStringList> split_on;

	if (split_cmd.size() < 2) {
		printCommandExecutionResults("Error: ld command format error!");
		return;
	}

	_stkName = split_cmd[1];
	std::string mktCodeStr = _stkName.toStdString();
	//S4::mktCodeI_t mktCodeInt;
	try{
		//mktCodeInt = 
		S4::mktCodeStr_to_mktCodeInt(mktCodeStr);
	}catch(...){
		try{
			mktCodeStr = S4::pureCodeStr_to_mktCodeStr(mktCodeStr);
		}catch(...){
			printCommandExecutionResults(_stkName + " : unsupported instrument code!");
			return;
		}
	}
	_stkName.fromStdString(mktCodeStr);

	for (int i = 1; i < split_cmd.size(); ++i) {
		if (split_cmd[i].startsWith("stg=")) {
			stg_found = 1;
			_stgName = split_cmd[i].section('=', 1, 1);
		}
		else if (split_cmd[i].startsWith("tbl=")) {
			tbl_found = 1;
			_orderTblName = split_cmd[i].section('=', 1, 1);
		}else if (split_cmd[i].startsWith("on=")) {
            on_found = 1;
            split_on.emplace_back(split_cmd[i].split("=", QString::SkipEmptyParts));
		}
	}

	if (!stg_found)
		_stgName = "";
	if (!tbl_found)
		_orderTblName = "";


	emit signal_load(mktCodeStr, _stgName.toStdString(), _orderTblName.toStdString());

	QString log = "load(";
	log += _stkName + ", " + _stgName + ", " + _orderTblName+") emited";

	printCommandExecutionResults(log);

    if (on_found){
		for (auto& on : split_on) {
			handlCommand_on(on);
			handlCommand_cross(on);   //bind cross with on
		}
    }
}


void cliparser::handlCommand_on(QStringList& split_cmd)
{

	if (split_cmd.size() < 2) {
		printCommandExecutionResults("Error: ld command format error!");
		return;
	}

    int date;
    try{
        date = S4::IntConvertor::convert(split_cmd[1].toStdString());
    }catch(const std::exception& e){
        printCommandExecutionResults(split_cmd[1] + " : convert to date failed=" + e.what());
        return;
    }

    if (date <0)
        date = 0;
    emit signal_centerOn_day(date);
	QString log = "centerOn_day(" + QString::number(date) + ") emited";
	printCommandExecutionResults(log);
}


void cliparser::handlCommand_cross(QStringList& split_cmd)
{

	if (split_cmd.size() < 2) {
		printCommandExecutionResults("Error: ld command format error!");
		return;
	}

    int date;
    try{
        date = S4::IntConvertor::convert(split_cmd[1].toStdString());
    }catch(const std::exception& e){
        printCommandExecutionResults(split_cmd[1] + " : convert to date failed=" + e.what());
        return;
    }

    if (date <0)
        date = 0;
    emit signal_crossOn_day(date);
	QString log = "crossOn_day(" + QString::number(date) + ") emited";
	printCommandExecutionResults(log);
}