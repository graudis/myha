// #include "stdafx.h"
// 
// #include "cdbworker.h"
// #include "center.h"
// 
// CDBWorker::CDBWorker() : pg_conn_(NULL)
// {
// 	on_error_state_ = ON_DB_ERROR_CONTINUE;
// }
// 
// CDBWorker::~CDBWorker()
// {
// }
// 
// /*
//  *	DBMS에 접속 시도
//  */
// tBOOL CDBWorker::init(TDatabaseInfo& gamedb_info)
// {
// 	if( pg_conn_ != NULL )
// 		return cFALSE;
// 
// 	pg_conn_ = PQsetdbLogin(gamedb_info.host_.c_str(), gamedb_info.port_.c_str(), NULL, NULL,
// 		gamedb_info.dbname_.c_str(), gamedb_info.id_.c_str(), gamedb_info.password_.c_str());
// 
// 	if(PQstatus(pg_conn_) == CONNECTION_OK)
// 	{
// 		return cTRUE;
// 	}
// 	else
// 	{
// 		LOG_ERROR("can't connect to db. host[%s] / ip[%s] / dbname[%s] / id[%s] / password[%s]",
// 			gamedb_info.host_.c_str(), gamedb_info.port_.c_str(), gamedb_info.dbname_.c_str(),
// 			gamedb_info.id_.c_str(), gamedb_info.password_.c_str());
// 
// 		LOG_ERROR( "could not connect to database message: %s", PQerrorMessage( pg_conn_ ) );
// 		clear();
// 		return cFALSE;
// 	}
// }
// 
// /*
//  *	DBMS 접속을 종료한다.
//  */
// void CDBWorker::clear()
// {
// 	if( pg_conn_ != NULL )
// 	{
// 		PQfinish(pg_conn_);
// 		pg_conn_ = NULL;
// 	}
// }
// 
// /*
//  *	select 전용 쿼리 실행
//  */
// PGresult* CDBWorker::select_exec(const tCHAR* sql)
// {
// 	PGresult* result = PQexec( pg_conn_, sql );
// 	if( result == NULL )
// 	{
// 		LOG_ERROR( "query error : query[%s] / error[%s]", sql, PQerrorMessage(pg_conn_) );
// 
// 		if( PQstatus( pg_conn_ ) == CONNECTION_BAD )
// 			kill( getpid(), SIGTERM );
// 		return NULL;
// 	}
// 
// 	ExecStatusType status_type = PQresultStatus( result );
// 	if( status_type == PGRES_BAD_RESPONSE ||
// 		status_type == PGRES_NONFATAL_ERROR ||
// 		status_type == PGRES_FATAL_ERROR )
// 	{
// 		LOG_ERROR( "query error : query[%s] / error[%s]", sql, PQerrorMessage(pg_conn_) );
// 		PQclear( result );
// 
// 		if( PQstatus( pg_conn_ ) == CONNECTION_BAD )
// 			kill( getpid(), SIGTERM );
// 		return NULL;
// 	}
// 
// 	if( status_type != PGRES_TUPLES_OK )
// 	{
// 		LOG_ERROR( "query error : query[%s] / error[%s]", sql, PQerrorMessage(pg_conn_) );
// 		PQclear( result );
// 
// //		if( on_error_state_ == ON_DB_ERROR_STOP )
// //			kill( getpid(), SIGTERM );
// 		return NULL;
// 	}
// 
// 	return result;
// }
// 
// /*
//  *	command(update/delete/insert) 전용 쿼리 실행
//  */
// tBOOL CDBWorker::command_exec(const tCHAR* sql)
// {
// 	PGresult* result = PQexec( pg_conn_, sql );
// 	if( result == NULL )
// 	{
// 		LOG_ERROR( "query error : query[%s] / error[%s]", sql, PQerrorMessage(pg_conn_) );
// 
// 		if( PQstatus( pg_conn_ ) == CONNECTION_BAD )
// 			kill( getpid(), SIGTERM );
// 		return cFALSE;
// 	}
// 
// 	boost::shared_ptr<PGresult> _auto_clear(result, PQclear);
// 
// 	ExecStatusType status_type = PQresultStatus( result );
// 	if( status_type == PGRES_BAD_RESPONSE ||
// 		status_type == PGRES_NONFATAL_ERROR ||
// 		status_type == PGRES_FATAL_ERROR )
// 	{
// 		LOG_ERROR( "query error : query[%s] / error[%s]", sql, PQerrorMessage(pg_conn_) );
// 		if( PQstatus( pg_conn_ ) == CONNECTION_BAD )
// 			kill( getpid(), SIGTERM );
// 		return cFALSE;
// 	}
// 
// 	if( status_type != PGRES_COMMAND_OK && status_type != PGRES_TUPLES_OK )
// 	{
// 		LOG_ERROR( "query error : query[%s] / error[%s]", sql, PQerrorMessage(pg_conn_) );
// //		if( on_error_state_ == ON_DB_ERROR_STOP )
// //			kill( getpid(), SIGTERM );
// 		return NULL;
// 	}
// 
// 	return cTRUE;
// }
