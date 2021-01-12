# Northern-Ski-League-With-GUI

To edit this you will also need to install the Windows 10 SDK 10.0.18362.0 or above from the visual studio installer.

You will also need to install the sqlite3 uwp installer.  The link to the install is: https://www.sqlite.org/download.html
You will need to download the Universal Windows Platform installer, sqlite-uwp-xxxxxxx.vsix where xxxxxxx is the version number.

To use this you need to move the ski.db file located in the /database File/ directory to the directory where this app is installed.  It is generally installed in the directory similar to
C:\Users\[your username]\appdata\local\Packages\[string of numbers]\LocalState\

There is currently a database that is being used for testing located in the /database File/Database Testing/ folder.

[your username] is your username
[string of numbers] is a random combination of numbers that will be generated when the app is installed.  To determine it is the correct location the folder specified above will 
have a test.db file in it as I will make the app create a default empty database.


Leaving this doc here so I don't forget how to merge the master with another branch

https://docs.microsoft.com/en-us/azure/devops/repos/git/pulling?view=azure-devops&tabs=visual-studio




++-----------------------------------Database location-----------------------++

The location of the test database is in \Database File\Database Testing\Ski.db
This is a database that has data populated in the Athletes, meet_data, meets, seasons tables

IMPORTANT NOTE FOR THE ATHLETES AND MEETS TABLE:
Athletes.total_points, meets.total_points, meets.total_time are all calculated columns so DO NOT update these directly,
they update themselves when data is entered into the sl_, gs_, sg_, columns.

The SQL statements that have table definitions and statements that currently work are located in \Database File\Useful SQL Commands.txt
The update statement that is currently in progress is at the bottom of the document.

As a side note for the Useful SQL Commands.txt file.  The update statement command at the bottom has a WHERE caluse that restricts the command to 
bib 527 who has not attended meet 2. 
