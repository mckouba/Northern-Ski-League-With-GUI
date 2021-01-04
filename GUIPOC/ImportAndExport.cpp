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

	Athlete() {
		bib = 0;
		name = "";
		team = "";
		time = 0.0;
		points = 0;
		gender = "";
	};

	Athlete(int b, std::string n, std::string te, float ti, char* g) {
		bib = b;
		name = n;
		team = te;
		time = ti;
		points = 0;
		gender = g;
	}
};

int insertIntoDatabase(std::vector<Athlete> input, std::string type) {

	DBLite sqldb;
	//go thorugh each athlete and put them in the athlete table
	for (auto it = input.cbegin(); it != input.cend(); ++it) {

		Athlete temp = *it;

		sqldb.insertDataAthletes(std::to_string(temp.bib).c_str(), temp.name.c_str(), temp.team.c_str(), temp.gender.c_str());

	}
	if (type.compare("sl") == 0) {
		//input results as a slalom race


	}
	else if (type.compare("gs") == 0) {
		//input results as a gs race


	}
	else {
		//input results as a sg race


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
int processData(int argc, char** argv) {

	//keep track of input cols of data
	int bibCol, nameCol, timeCol, teamCol;

	//vector to sort the athlete data
	std::vector<Athlete> athleteVector;


	//verify valid input
	if (argc == 1) {
		printf("\n Usage: SortingPOC [File Location, \"help\"] [[bib column] [time column] [name column] [team column]]");
		return 0;
	}

	//correctly identify input
	if (argc == 2) {

		if (argv[1] == "help" || argv[1] == "Help") {

			printf("\nwill implement later");
			//TODO: implement help menu
			return 0;

		}

		//default set of column numbers
		//in csv will look like
		//251, 37.11, matt kouba, ashwabay
		bibCol = 0;
		nameCol = 2;
		timeCol = 1;
		teamCol = 3;
	}
	else {

		//take in user defined input columns
		bibCol = atoi(argv[2]);
		timeCol = atoi(argv[3]);
		nameCol = atoi(argv[4]);
		teamCol = atoi(argv[5]);
	}

	//get gender of the race
	char* gender = argv[6];
	char* raceType = argv[7];

	//input file location
	char* inputFile = argv[1];

	//file pointer
	std::fstream input;

	//open file
	input.open(inputFile, std::ios::in);

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



		Athlete tempA = { stoi(rows[bibCol]), rows[nameCol].c_str(), rows[teamCol].c_str(), stof(rows[timeCol]), argv[6] };


		athleteVector.push_back(tempA);

	}

	input.close();

	//sort the athlete vector
	std::sort(athleteVector.begin(), athleteVector.end(), compareAthletes);
	//open output file
	std::fstream out;

	out.open("output.csv", std::ios::out);

	int i = 1;
	for (auto it = athleteVector.cbegin(); it != athleteVector.cend(); ++it) {


		//make temp athlete object
		Athlete temp = *it;

		//put corect points for dnf and dns
		//TODO: could change later for custom dns and dnf values
		if (temp.time == 99999.0 || temp.time == 88888.0) {
			temp.points = athleteVector.size() + 1;
		}
		else {
			temp.points = i;
		}
		out << temp.bib << "," << temp.name << "," << temp.team << "," << temp.time << "," << temp.points << std::endl;

		i++;
	}
	//close output
	out.close();

	//time to do database stuff
	//todo: move to after user aproval
	if (insertIntoDatabase(athleteVector, raceType) == 0) {

		std::cout << "Data has been successfully inserted into the database\n";

	}
	else {
		std::cout << "Something has gone wrong while inserting into database.\n";
		return 1;
	}
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

	while (!input.eof()) {

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