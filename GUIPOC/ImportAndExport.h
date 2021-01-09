#include <string>
#include <vector>
//struct to handle the athelete information
struct Athlete;
//function to insert and update the database
int insertIntoDatabase(std::vector<Athlete> input, std::string type, int meet_num, int skiOSb);
//function to process the race data
int processData(int bibC, int nameC, int teamC, int timeC, std::string inputFile, std::string gend, std::string raceT, std::string seas, int meet_num, int skiOSb);
//dedicated function to get the data that was retrived from the seasons table in the database
std::vector<std::string> getSeasonsData();
//returns a vector of the strings that will be used in the meets combobox
std::vector<std::string> getMeet_Data();
//helper to transform std::string to Platform::String^
//Platform::String^ convertFromString(const std::string& input);

std::vector<std::string> getFirstRow(std::string path);