#include "pch.h"
#include <stdio.h>

#include "sqlite3.h"
#include "DBLite.h"
#include "ImportAndExport.h"

//will take input as argv[1] will be file name, argv[2] will be bib, argv[3] will be time, argv[4] will be name, argv[5] will be team, argv[6] will be the gender of the race, argv[7] will be race discipline
int main(int argc, char** argv) {
	if (argv[1] != 0) {

		processData(argc, argv);

	}
	else {
		//all database things are done through a seperate class

		DBLite sqldb;
		sqldb.getData("Athletes");

		sqldb.closeDB();

	}

	system("pause");
}