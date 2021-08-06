#pragma once

#include "data/data_if.h"
#include "broker/read_history.h"

#include <QWidget>
#include <memory>

namespace S4 {
namespace QT{

class data_panel_t
{
public:
	S4::stkInfoReq_t infoReq;
	const S4::stkInfo_t* info;
	std::vector<S4::s4_history_trade_t> history;
};

class s4qt_data_if
{
public:

	const S4::stkInfo_t* getInfo(const std::string & stkName, const struct S4::stkInfoReq_t& infoReq);
	void loadOrdres(const std::string & stkName, const std::string & stgName, const std::string & orderTblName, std::vector<S4::s4_history_trade_t>& history_trade_data);

private:
	std::shared_ptr<data_if_t> _pData_if;

};



}//namespace QT
}//namespace S4