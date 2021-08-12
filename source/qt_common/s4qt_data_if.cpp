#include "common/s4logger.h"
#include "trade/s4_history_trade.h"
#include "jsonTypes/s4_history_trade_t_dbTbl.h"
#include "jsonTypes/dayK_dsx_raw_t_dbTbl_ro.h"
#include "datasource/ds_dayK_db_ifeng.h"
#include "datasource/ds_dayK_db_dsx.h"
#include "qt_common/s4qt_data_if.h"

CREATE_LOCAL_LOGGER("qt_data_if")

namespace S4 {
namespace QT{

const S4::stkInfo_t* s4qt_data_if::getInfo(const std::string & stkName, const struct S4::stkInfoReq_t& infoReq)
{
    if (!_pData_if){
        _pData_if = std::make_shared<data_if_t>(1);
        
        std::filesystem::path db_root = glb_conf::pInstance()->db().root;

		try
		{
			std::filesystem::path db_dayK_dsx = db_root / glb_conf::pInstance()->db().dayK_lclx;
			std::shared_ptr<datasource_t> pDs_dayK_db = std::make_shared<ds_dayK_db_dsx_t>(db_dayK_dsx);
			_pData_if->setDatasource(timeMode_t::tDAY, pDs_dayK_db);
		}
		catch (...)
		{
			std::filesystem::path db_dayK_ifeng = db_root / glb_conf::pInstance()->db().dayK_ifeng;
			std::shared_ptr<datasource_t> pDs_dayK_db_ifeng = std::make_shared<ds_dayK_db_ifeng_t>(db_dayK_ifeng);
			_pData_if->setDatasource(timeMode_t::tDAY, pDs_dayK_db_ifeng);
		}


    }

	if (_pData_if->getNowLib() == nullptr || _pData_if->getNowLib()->count(stkName) == 0) {
	    auto glb_codelist = glb_conf::pInstance()->codelist();
        auto reqlist = std::make_shared<codelist_t>();
        code_prop_t instument_code = glb_conf::pInstance()->codelist()->at(mktCodeStr_to_mktCodeInt(stkName));
        reqlist->insert(std::pair<mktCodeI_t, code_prop_t>(mktCodeStr_to_mktCodeInt(stkName), instument_code));
		_pData_if->preload(reqlist, infoReq);
		while (!_pData_if->preloadReady())
		{
			process_sleep(0.01);
		}
		_pData_if->usePreload(true);
	}

	if (_pData_if->getNowLib()->count(stkName) == 0) {
		LCL_WARN("no such instrument: {:}", stkName);
		return nullptr;
	}

	const S4::stkInfo_t* info = _pData_if->getNowLib()->get(stkName);
	LCL_INFO("getInfo {:} successd: {:} ~ {:} = {:}", stkName, info->pDayKQ->front()->_date, info->pDayKQ->back()->_date, info->pDayKQ->size());
    return info;
}

void s4qt_data_if::loadOrdres(const std::string & stkName, const std::string & stgName, const std::string & table_name, std::vector<s4_history_trade_t>& history_trade_data)
{

	glb_conf_ctx_t::db_t db = glb_conf::pInstance()->db();
	std::filesystem::path db_root_path = db.root;
	std::filesystem::path db_history_path = db_root_path / db.history_trade;
	sqlite::DB_t history_db(db_history_path.string());
	
	std::string condition = " WHERE mktCodeStr = '" + stkName + "'";
	if (stgName.size()) {
		condition += " AND stgName = '" + stgName + "'";
	}
	history_db.read_table<S4::sqlite::s4_history_trade_t_dbTbl>(table_name, history_trade_data, condition);

	LCL_INFO("loadOrdres stk={:} stg={:} tbl={:} done, size={:}", stkName, stgName, table_name, history_trade_data.size());

}

}//namespace QT
}//namespace S4