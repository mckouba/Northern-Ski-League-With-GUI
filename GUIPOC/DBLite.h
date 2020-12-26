#include "sqlite3.h"
#include <string>

class DBLite {

private:
	//database pointer
	sqlite3* db;

	//error messages
	char* zErrMsg;

	//command result
	int rc;

	//saved sql
	char* sql;

	//compiled sql statement
	sqlite3_stmt* stmt;

	//array of valid table names
	std::string validTables[5] = { "Athletes", "meet_data", "meets", "season", "seasons" };

	void checkDBErrors();

	
public:

	//constructor
	DBLite();
	//insert data into the Athletes table
	void insertDataAthletes(const char* bib, const char* name, const char* team, const char* gender);
	//insert data into the meets table but only SL results
	void insertDataMeets_sl(char* bib, char* sl_time, char* sl_points, char* season, char* meet_id);
	//insert data into the meets table but only GS results
	void insertDataMeets_gs(char* bib, char* gs_time, char* gs_points, char* season, char* meet_id);
	//insert data into the meets table but only SG results
	void insertDataMeets_sg(char* bib, char* sg_time, char* sg_points, char* season, char* meet_id);
	//insert data into the Season table
	void insertDataSeason(char* bib, char* sl_points, char* gs_points, char* sg_points, char* season);
	//insert data into the Meet_data table
	void inertDataMeet_Data(char* meet_id, char* date, char* location);
	//get data from the requested table
	void getData(const char* table);
	//close the open database
	void closeDB();

};