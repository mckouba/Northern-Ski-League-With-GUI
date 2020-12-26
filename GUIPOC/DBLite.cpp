#include "pch.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>

#include "DBLite.h"
#include "Helpers.h"


//TODO: figure out how to deal with people leaving and their bib being used by someone else
//just make a new database every season?
DBLite::DBLite() {

	auto location = getCurrentLocation();

	std::string dbLocation = location + "\\test.db";

	rc = sqlite3_open_v2(dbLocation.c_str(), &db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);

	checkDBErrors();
}


//callback functions
//oputput will be in athlete_temp
//formatted as:
//bib, name, team
int athleteCallback(void* NotUsed, int argc, char** argv, char** azColName) {
	//will print to temp file that can be read by other part of program
//slow but it will do for now
	auto location = getCurrentLocation();

	std::string dir = location + "\\athlete_temp";

	std::ofstream output(dir.c_str(), std::ios::app);

	if (output.is_open()) {
		//write to csv
		output << argv[0] << "," << argv[1] << "," << argv[2] << std::endl;
		//close file
		output.close();
	}

	return 0;
}
//output will be in 'meet_data_temp'
//formatted as:
//meet_id, date, location, season
int meet_dataCallback(void* NotUsed, int argc, char** argv, char** azColName) {
	//will print to temp file that can be read by other part of program
//slow but it will do for now
	auto location = getCurrentLocation();

	std::string dir = location + "\\meet_data_temp";

	std::ofstream output(dir.c_str(), std::ios::app);

	if (output.is_open()) {
		//write to csv
		output << argv[0] << "," << argv[1] << "," << argv[2] << "," << argv[3] << std::endl;
		//close file
		output.close();
	}

	return 0;
}
//output will be in meets_temp
//formatted as:
//bib, sl_time, gs_time, sg_time, sl_points, gs_points, sg_points, total_points, total_time, season, meet_id
int meetsCallback(void* NotUsed, int argc, char** argv, char** azColName) {
	//will print to temp file that can be read by other part of program
//slow but it will do for now
	auto location = getCurrentLocation();

	std::string dir = location + "\\meets_temp";

	std::ofstream output(dir.c_str(), std::ios::app);

	if (output.is_open()) {
		//write to csv
		output << argv[0] << "," << argv[1] << "," << argv[2] << "," << argv[3] << "," << argv[4] << "," << argv[5] << "," << argv[6] << "," << argv[7] << "," << argv[8] << "," << argv[9] << "," << argv[10] << std::endl;
		//close file
		output.close();
	}

	return 0;
}
//output will bein season_temp
//formatted as:
//bib, sl_points, gs_points, sg_points, total_points, season
int seasonCallback(void* NotUsed, int argc, char** argv, char** azColName) {
	//will print to temp file that can be read by other part of program
//slow but it will do for now
	auto location = getCurrentLocation();

	std::string dir = location + "\\season_temp";

	std::ofstream output(dir.c_str(), std::ios::app);

	if (output.is_open()) {
		//write to csv
		output << argv[0] << "," << argv[1] << "," << argv[2] << argv[3] << "," << argv[4] << "," << argv[5] << "," << std::endl;
		//close file
		output.close();
	}

	return 0;
}
//output will bein seasons_temp
//formatted as:
//season
int seasonsCallback(void* NotUsed, int argc, char** argv, char** azColName) {
	//will print to temp file that can be read by other part of program
//slow but it will do for now
	auto location = getCurrentLocation();

	std::string dir = location + "\\seasons_temp";

	std::ofstream output(dir.c_str(), std::ios::app);

	if (output.is_open()) {
		//write to csv
		output << argv[0] << std::endl;
		//close file
		output.close();
	}

	return 0;
}
void DBLite::checkDBErrors() {
	if (rc) {
		std::string error = sqlite3_errmsg(db);
		std::cout << "DB Error: " << sqlite3_errmsg(db) << std::endl;
	}
}



void DBLite::insertDataAthletes(const char* bib, const char* name, const char* team, const char* gender) {

	char* query;
	int n;

	//build string 
	n = snprintf(NULL, 0, "INSERT INTO Athletes ('bib', 'name', 'team', 'gender') VALUES ( %s, '%s', '%s', '%s');", bib, name, team, gender);

	query = (char*)malloc(n + 1);

	n = snprintf(query, n + 1, "INSERT INTO Athletes ('bib', 'name', 'team', 'gender') VALUES ( %s, '%s', '%s', '%s');", bib, name, team, gender);

	//prepare query
	sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

	//test it
	rc = sqlite3_step(stmt);
	checkDBErrors();

	//finalize usage
	sqlite3_finalize(stmt);

	//free up space
	free(query);

}

void DBLite::insertDataMeets_sl(char* bib, char* sl_time, char* sl_points, char* season, char* meet_id) {}
void DBLite::insertDataMeets_gs(char* bib, char* gs_time, char* gs_points, char* season, char* meet_id) {}
void DBLite::insertDataMeets_sg(char* bib, char* sg_time, char* sg_points, char* season, char* meet_id) {}

void DBLite::insertDataSeason(char* bib, char* sl_points, char* gs_points, char* sg_points, char* season) {}

void DBLite::inertDataMeet_Data(char* meet_id, char* date, char* location) {}

void DBLite::getData(const char* table) {
	//todo: callbackfunctions are not getting called
	char* query;
	int n;

	n = snprintf(NULL, 0, "SELECT * FROM %s", table);

	query = (char*)malloc(n + 1);

	n = snprintf(query, n + 1, "SELECT * FROM %s", table);

	//search through array of valid tables for closest match
	if (validTables[0].compare(table) == 0) {
		//matches Athletes

		sqlite3_exec(db, query, athleteCallback, 0, &zErrMsg);
	}
	else if (validTables[1].compare(table) == 0) {
		//matches meet_data

		

		n = snprintf(NULL, 0, "SELECT * FROM %s WHERE season='%s'", table, selectedValues[2].c_str());

		query = (char*)malloc(n + 1);

		n = snprintf(query, n + 1, "SELECT * FROM %s WHERE season='%s'", table, selectedValues[2].c_str());

		sqlite3_exec(db, query, meet_dataCallback, 0, &zErrMsg);
	}
	else if (validTables[2].compare(table) == 0) {
		//matches meets
		sqlite3_exec(db, query, meetsCallback, 0, &zErrMsg);
	}
	else if(validTables[3].compare(table) == 0) {
		//must match seasons
		sqlite3_exec(db, query, seasonCallback, 0, &zErrMsg);
	}
	else if(validTables[4].compare(table) == 0) {
		//must match seasons
		sqlite3_exec(db, query, seasonsCallback, 0, &zErrMsg);
		checkDBErrors();
	}
}

void DBLite::closeDB() {
	sqlite3_close(db);
}

