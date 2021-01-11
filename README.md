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
