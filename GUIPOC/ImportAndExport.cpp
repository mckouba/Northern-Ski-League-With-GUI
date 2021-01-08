#include "pch.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "ImportAndExport.h"
#include "DBLite.h"
#include "Helpers.h"


struct Athlete
{
	int bib;
	std::string name;
	std::string team;
	float time;
	int points;
	std::string gender;
	std::string season;

	Athlete() {
		bib = 0;
		name = "";
		team = "";
		time = 0.0;
		points = 0;
		gender = "";
		season = "";
	};

	Athlete(int b, std::string n, std::string te, float ti, std::string g, std::string se) {
		bib = b;
		name = n;
		team = te;
		time = ti;
		points = 0;
		gender = g;
		season = se;
	}

	void setPoints(int pts) {
		points = pts;
	}
};

int insertIntoDatabase(std::vector<Athlete> input, std::string type, int meet_num) {

	DBLite sqldb;
	//go thorugh each athlete and put them in the athlete table
	for (auto it = input.cbegin(); it != input.cend(); ++it) {
		//insert the athlete into the athlete table
		Athlete temp = *it;

		sqldb.insertDataAthletes(std::to_string(temp.bib).c_str(), temp.name.c_str(), temp.team.c_str(), temp.gender.c_str(), temp.season.c_str());


		if (type.compare("Slalom") == 0) {
			//insert as an sl meet
			sqldb.insertDataMeets_sl(std::to_string(temp.bib).c_str(), std::to_string(temp.time).c_str(), std::to_string(temp.points).c_str(), temp.season.c_str(), std::to_string(meet_num).c_str());

		}
		else if (type.compare("Giant Slalom") == 0) {
			//input results as a gs race


		}
		else {
			//input results as a sg race


		}
	}
	sqldb.closeDB();
	return 1;
}

//heler function to compare the times between two athletes
bool compareAthletes(const Athlete& a, const Athlete& b) {
	return a.time < b.time;
}

//import and export data
//takes the command line input as args
int processData(int bibC, int nameC, int teamC, int timeC, std::string inputFile , std::string gend, std::string raceT, std::string seas, int meet_num) {

	//keep track of input cols of data
	int bibCol, nameCol, timeCol, teamCol;

	//vector to sort the athlete data
	std::vector<Athlete> athleteVector;

		//default set of column numbers
		//in csv will look like
		//251, 37.11, matt kouba, ashwabay
		bibCol = bibC;
		nameCol = nameC;
		timeCol = timeC;
		teamCol = teamC;

	//file pointer
	std::fstream input;

	//open file
	input.open(inputFile.c_str(), std::ios::in);

	//to store input strings
	std::vector<std::string> rows;
	//strings to store the input
	std::string line, word, temp;


	while (!input.eof()) {

		rows.clear();

		//read row of data
		std::getline(input, line);

		//to breakup words
		std::stringstream s(line);

		while (std::getline(s, word, ',')) {

			rows.push_back(word);

		}
		//TODO: implement a filter for times greater that 60s which have minute markers
		//checks to see if the first character in the time 'string' is a number or not
		//used to differentiate DNF, DNS
		if (!isdigit(rows[timeCol].at(0))) {

			if (rows[timeCol].compare("DNF") == 0) {
				//reset the time value for this person to 99999.0 which will be DNF
				rows[timeCol] = "99999.0";
			}
			else if (rows[timeCol].compare("DNS")) {
				//reset the time value for this person to be 88888.0 which will be DNS
				rows[timeCol] = "88888.0";
			}

		}


		if (gend.compare("Womens") == 0) {
			//convert to "F"
			gend = "F";
		}
		else if (gend.compare("Mens") == 0) {
			//convert to M
			gend = "M";
		}


		Athlete tempA = { stoi(rows[bibCol]), rows[nameCol].c_str(), rows[teamCol].c_str(), stof(rows[timeCol]), gend, seas};

		
		athleteVector.push_back(tempA);

	}

	input.close();

	//sort the athlete vector
	std::sort(athleteVector.begin(), athleteVector.end(), compareAthletes);
	
	

	//calculate points
	int i = 1;
	for (int j = 0; j < athleteVector.size(); j++) {



		int maxPts = athleteVector.size() + 1;

		//put corect points for dnf and dns
		//TODO: could change later for custom dns and dnf values
		if (athleteVector.at(j).time == 99999.0 || athleteVector.at(j).time == 88888.0) {
			athleteVector.at(j).setPoints(athleteVector.size() + 1);
		}
		else {
			athleteVector.at(j).setPoints(i);
		}
		i++;
	}

	//time to do database stuff
	//todo: move to after user aproval
	insertIntoDatabase(athleteVector, raceT, meet_num);
	

	return 0;
}

std::vector<std::string> getSeasonsData() {

	auto location = getCurrentLocation();

	std::string dir = location + "\\seasons_temp";

	//file pointer
	std::fstream input;

	//open file
	input.open(dir.c_str(), std::ios::in);

	//to store input strings
	std::vector<std::string> rows;
	//strings to store the input
	std::string line, word, temp;

	std::vector<std::string> out;

	line = " ";

	while (line != "") {
		line = "";
		rows.clear();

		//read row of data
		std::getline(input, line);

		//get each item and put it at the end of the output list
		std::string t = line;
		//ensure blank items do not get added
		if (line.compare("") != 0) {
			out.push_back(t);
		}
	}

	input.close();
	//remove the temp file
	DeleteFileA(dir.c_str());


	return out;
}


std::vector<std::string> getMeet_Data() {

	auto location = getCurrentLocation();

	std::string dir = location + "\\meet_data_temp";

	//file pointer
	std::fstream input;

	//open file
	input.open(dir.c_str(), std::ios::in);

	//to store input strings
	std::vector<std::string> rows;
	//strings to store the input
	std::string line, word, temp;

	std::vector<std::string> out;

	line = " ";

	while (line != "") {
		line = "";
		rows.clear();

		//read row of data
		std::getline(input, line);
		
		//get each item and put it at the end of the output list
		std::string t = line;
		
		//to breakup words
		std::stringstream s(line);

		while (std::getline(s, word, ',')) {

			rows.push_back(word);

		}if (line.compare("") != 0) {
			//make selection string look like "Meet #[meet_id], [location], [date]
			//example "Meet #1, Mt. Ashwabay, 2-21-2020"
			std::string temp = "Meet #" + rows[0] + ", " + rows[2] + ", " + rows[1];
			out.push_back(temp);
		}
		
	}

	input.close();
	//remove the temp file
	DeleteFileA(dir.c_str());

	return out;

}

std::vector<std::string> getFirstRow(std::string path) {

	//file pointer
	std::fstream input;

	//open file
	input.open(path.c_str(), std::ios::in);

	//to store input strings
	std::vector<std::string> rows;
	//strings to store the input
	std::string line, word, temp;

	std::vector<std::string> out;
	line = "";
	rows.clear();

	//read row of data
	std::getline(input, line);

	//get each item and put it at the end of the output list
	std::string t = line;

	//to breakup words
	std::stringstream s(line);

	while (std::getline(s, word, ',')) {

		rows.push_back(word);

	}
	if (line.compare("") != 0) {

		//push back the first four csv values
		out.push_back(rows[0]);
		out.push_back(rows[1]);
		out.push_back(rows[2]);
		out.push_back(rows[3]);
	}

	input.close();

	return out;



}