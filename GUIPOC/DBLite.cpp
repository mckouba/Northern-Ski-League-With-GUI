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

}

void DBLite::connect(std::string type) {

	//open the correct database
	auto location = getCurrentLocation();
	std::string dbLocation;
	if (type.compare("Ski") == 0) {
		dbLocation = location + "\\ski.db";
	}
	else {
		dbLocation = location + "\\snowboard.db";
	}
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

	/*do we want to catch some exception here? */
	
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



void DBLite::insertDataAthletes(const char* bib, const char* name, const char* team, const char* gender, const char* season) {
	char* query;
	int n;

	//check to see if record exists
	n = snprintf(NULL, 0, "SELECT * FROM Athletes WHERE bib=%s and season='%s';", bib, season);

	query = (char*)malloc(n + 1);

	n = snprintf(query, n + 1, "SELECT * FROM Athletes WHERE bib=%s and season='%s';", bib, season);

	sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

	//test it
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	if (rc != 100) {
		//build string 
		n = snprintf(NULL, 0, "INSERT INTO Athletes ('bib', 'name', 'team', 'gender', 'season') VALUES ( %s, '%s', '%s', '%s', '%s');", bib, name, team, gender, season);

		query = (char*)malloc(n + 1);

		n = snprintf(query, n + 1, "INSERT INTO Athletes ('bib', 'name', 'team', 'gender', 'season') VALUES ( %s, '%s', '%s', '%s', '%s');", bib, name, team, gender, season);
		
		sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

		//test it
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else {
	//do nothing as the athlete already exists
	}
	//free up space
	free(query);
}



//basics for inserting data invloves:

/*

We will first execure a select command for the bib that was provivided.

If the command reutrns SQLITE_ROW or rc=100 we know that the result exists and we will just update the record

If it returns anything other than SQLITE_ROW or rc!=100 we will do an INSERT command and create a new recod.

*/
void DBLite::insertDataMeets_sl(const char* bib, const char* sl_time, const char* sl_points, const char* season, const char* meet_id) {
	char* query;
	int n;

	//check to see if record exists
	n = snprintf(NULL, 0, "SELECT * FROM meets WHERE bib=%s and meet_id=%s and season='%s';", bib, meet_id, selectedValues[2].c_str());

	query = (char*)malloc(n + 1);

	n = snprintf(query, n + 1, "SELECT * FROM meets WHERE bib=%s and meet_id=%s and season='%s';", bib, meet_id, selectedValues[2].c_str());

	sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

	//test it
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	if (rc != 100) {
		//build string 
		n = snprintf(NULL, 0, "INSERT INTO meets ('bib', 'sl_time', 'sl_points', 'season', 'meet_id') VALUES ( %s, %s, %s, '%s', %s);", bib, sl_time, sl_points, selectedValues[2].c_str(), meet_id);

		query = (char*)malloc(n + 1);

		n = snprintf(query, n + 1, "INSERT INTO meets ('bib', 'sl_time', 'sl_points', 'season', 'meet_id') VALUES ( %s, %s, %s, '%s', %s);", bib, sl_time, sl_points, selectedValues[2].c_str(), meet_id);

		//prepare query
		sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

		//test it
		rc = sqlite3_step(stmt);
		checkDBErrors();

		//finalize usage
		sqlite3_finalize(stmt);
	}
	else {
		//build string 
		n = snprintf(NULL, 0, "UPDATE meets SET sl_time=%s, sl_points=%s WHERE bib=%s and meet_id=%s and season='%s';", sl_time, sl_points, bib, meet_id, selectedValues[2].c_str());

		query = (char*)malloc(n + 1);

		n = snprintf(query, n + 1, "UPDATE meets SET sl_time=%s, sl_points=%s WHERE bib=%s and meet_id=%s and season='%s';", sl_time, sl_points, bib, meet_id, selectedValues[2].c_str());

		//prepare query
		sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

		//test it
		rc = sqlite3_step(stmt);
		checkDBErrors();

		//finalize usage
		sqlite3_finalize(stmt);
	}
	//free up space
	free(query);
}
void DBLite::insertDataMeets_gs(const char* bib, const char* gs_time, const char* gs_points, const char* season, const char* meet_id) {
	char* query;
	int n;

	//check to see if record exists
	n = snprintf(NULL, 0, "SELECT * FROM meets WHERE bib=%s and meet_id=%s and season='%s';", bib, meet_id, selectedValues[2].c_str());

	query = (char*)malloc(n + 1);

	n = snprintf(query, n + 1, "SELECT * FROM meets WHERE bib=%s and meet_id=%s and season='%s';", bib, meet_id, selectedValues[2].c_str());

	sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

	//test it
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	if (rc != 100) {
		//build string 
		n = snprintf(NULL, 0, "INSERT INTO meets ('bib', 'gs_time', 'gs_points', 'season', 'meet_id') VALUES ( %s, %s, %s, '%s', %s);", bib, gs_time, gs_points, selectedValues[2].c_str(), meet_id);

		query = (char*)malloc(n + 1);

		n = snprintf(query, n + 1, "INSERT INTO meets ('bib', 'gs_time', 'gs_points', 'season', 'meet_id') VALUES ( %s, %s, %s, '%s', %s);", bib, gs_time, gs_points, selectedValues[2].c_str(), meet_id);

		//prepare query
		sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

		//test it
		rc = sqlite3_step(stmt);
		checkDBErrors();

		//finalize usage
		sqlite3_finalize(stmt);
	}
	else {
		//build string 
		n = snprintf(NULL, 0, "UPDATE meets SET gs_time=%s, gs_points=%s WHERE bib=%s and meet_id=%s and season='%s';", gs_time, gs_points, bib, meet_id, selectedValues[2].c_str());

		query = (char*)malloc(n + 1);

		n = snprintf(query, n + 1, "UPDATE meets SET gs_time=%s, gs_points=%s WHERE bib=%s and meet_id=%s and season='%s';", gs_time, gs_points, bib, meet_id, selectedValues[2].c_str());

		//prepare query
		sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

		//test it
		rc = sqlite3_step(stmt);
		checkDBErrors();

		//finalize usage
		sqlite3_finalize(stmt);
	}
	//free up space
	free(query);
}
void DBLite::insertDataMeets_sg(const char* bib, const char* sg_time, const char* sg_points, const char* season, const char* meet_id) {
	char* query;
	int n;

	//check to see if record exists
	n = snprintf(NULL, 0, "SELECT * FROM meets WHERE bib=%s and meet_id=%s and season='%s';", bib, meet_id, selectedValues[2].c_str());

	query = (char*)malloc(n + 1);

	n = snprintf(query, n + 1, "SELECT * FROM meets WHERE bib=%s and meet_id=%s and season='%s';", bib, meet_id, selectedValues[2].c_str());

	sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

	//test it
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	if (rc != 100) {
		//build string 
		n = snprintf(NULL, 0, "INSERT INTO meets ('bib', 'sg_time', 'sg_points', 'season', 'meet_id') VALUES ( %s, %s, %s, '%s', %s);", bib, sg_time, sg_points, selectedValues[2].c_str(), meet_id);

		query = (char*)malloc(n + 1);

		n = snprintf(query, n + 1, "INSERT INTO meets ('bib', 'sg_time', 'sg_points', 'season', 'meet_id') VALUES ( %s, %s, %s, '%s', %s);", bib, sg_time, sg_points, selectedValues[2].c_str(), meet_id);

		//prepare query
		sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

		//test it
		rc = sqlite3_step(stmt);
		checkDBErrors();

		//finalize usage
		sqlite3_finalize(stmt);
	}
	else {
		//build string 
		n = snprintf(NULL, 0, "UPDATE meets SET sg_time=%s, sg_points=%s WHERE bib=%s and meet_id=%s and season='%s';", sg_time, sg_points, bib, meet_id, selectedValues[2].c_str());

		query = (char*)malloc(n + 1);

		n = snprintf(query, n + 1, "UPDATE meets SET sg_time=%s, sg_points=%s WHERE bib=%s and meet_id=%s and season='%s';", sg_time, sg_points, bib, meet_id, selectedValues[2].c_str());

		//prepare query
		sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

		//test it
		rc = sqlite3_step(stmt);
		checkDBErrors();

		//finalize usage
		sqlite3_finalize(stmt);
	}
	//free up space
	free(query);
}

void DBLite::insertDataSeason(char* bib, char* sl_points, char* gs_points, char* sg_points, char* season) {}

void DBLite::insertNewDataMeet_Data(const char* meet_id, const char* date, const char* location) {


	char* query;
	int n;

	//build string 
	n = snprintf(NULL, 0, "INSERT INTO meet_data ('meet_id', 'date', 'location', 'season') VALUES ( %s, '%s', '%s', '%s');", meet_id, date, location, selectedValues[2].c_str());

	query = (char*)malloc(n + 1);

	n = snprintf(query, n+1, "INSERT INTO meet_data ('meet_id', 'date', 'location', 'season') VALUES ( %s, '%s', '%s', '%s');", meet_id, date, location, selectedValues[2].c_str());

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

void DBLite::updateDataMeet_Data(const char* meet_id,const char* date, const char* location) {

	char* query;
	int n;

	const char* season = selectedValues[2].c_str();

	//build string 
	n = snprintf(NULL, 0, "UPDATE meet_data set location='%s', date='%s' where meet_id=%s and season='%s';", location, date, meet_id, season);

	query = (char*)malloc(n + 1);

	n = snprintf(query, n+1, "UPDATE meet_data set location='%s', date='%s' where meet_id=%s and season='%s';", location, date, meet_id, season);

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

void DBLite::updateSeasonResults(std::string season, int numOfMeets) {

	//ensure that the number of meets is correct for each athlete for the season
	verifySeasonResults(season, numOfMeets);


	//time to execute sql command to calculate results

	char* query;
	int n;

	//craft sql statement
	n = snprintf(NULL, 0, " UPDATE Athletes SET sl_points = (SELECT SUM(sl_points) OVER(ORDER BY sl_points ROWS BETWEEN CURRENT ROW AND %s FOLLOWING) FROM meets WHERE meets.bib = Athletes.bib AND meets.season = Athletes.season LIMIT 1), gs_points = (SELECT SUM(gs_points) OVER(ORDER BY gs_points ROWS BETWEEN CURRENT ROW AND %s FOLLOWING) FROM meets WHERE meets.bib = Athletes.bib AND meets.season = Athletes.season LIMIT 1), sg_points = (SELECT SUM(sg_points) OVER(ORDER BY sg_points ROWS BETWEEN CURRENT ROW AND %s FOLLOWING) FROM meets WHERE meets.bib = Athletes.bib AND meets.season = Athletes.season LIMIT 1) WHERE season = '%s'; ", numOfMeets, numOfMeets, numOfMeets, selectedValues[2].c_str());

	query = (char*)malloc(n + 1);

	n = snprintf(query, n + 1, " UPDATE Athletes SET sl_points = (SELECT SUM(sl_points) OVER(ORDER BY sl_points ROWS BETWEEN CURRENT ROW AND %s FOLLOWING) FROM meets WHERE meets.bib = Athletes.bib AND meets.season = Athletes.season LIMIT 1), gs_points = (SELECT SUM(gs_points) OVER(ORDER BY gs_points ROWS BETWEEN CURRENT ROW AND %s FOLLOWING) FROM meets WHERE meets.bib = Athletes.bib AND meets.season = Athletes.season LIMIT 1), sg_points = (SELECT SUM(sg_points) OVER(ORDER BY sg_points ROWS BETWEEN CURRENT ROW AND %s FOLLOWING) FROM meets WHERE meets.bib = Athletes.bib AND meets.season = Athletes.season LIMIT 1) WHERE season = '%s'; ", numOfMeets, numOfMeets, numOfMeets, selectedValues[2].c_str());

	sqlite3_prepare(db, query, strlen(query), &stmt, NULL);

	//test it
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);


	//free up space
	free(query);
}

void DBLite::verifySeasonResults(std::string season, int numOfMeets) {






}

void DBLite::insertDNSData(std::string season, int bib, int meetNum) {

	//get the number of athletes at that meet for the given gender
	//gender can be gotten with a sql query



}