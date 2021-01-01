//
// Inser_Meet.xaml.cpp
// Implementation of the Inser_Meet class
//

#include "pch.h"
#include "Inser_Meet.xaml.h"

#include "Helpers.h"
#include "DBLite.h"
#include "ImportAndExport.h"
#include "MainPage.xaml.h"

using namespace GUIPOC;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

Inser_Meet::Inser_Meet()
{
	InitializeComponent();
}


void GUIPOC::Inser_Meet::editOrAdd_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	//make sure only things needed to be displayed are displayed

	seasonSelect->IsEnabled = "False";
	meetSelect->IsEnabled = "False";

	meetSelect->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	editMeetInfo->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	newMeetInfo->Visibility = Windows::UI::Xaml::Visibility::Collapsed;


	if (make_string(editOrAdd->SelectedItem->ToString()->Data()).compare("Edit") == 0) {


		//enable season select
		seasonSelect->IsEnabled = "True";
		//enable meet slect
		meetSelect->IsEnabled = "True"; 
		//make meet slect visible
		meetSelect->Visibility = Windows::UI::Xaml::Visibility::Visible;
		//make meet info panel visible
		editMeetInfo->Visibility = Windows::UI::Xaml::Visibility::Visible;

		applyChanges->Visibility = Windows::UI::Xaml::Visibility::Visible;
		applyChanges->Content = "Apply Changes";

	}
	else {
		//only other option is add

		//enable the season select
		seasonSelect->IsEnabled = "True";

		//enable the add new meet stackpanel
		newMeetInfo->Visibility = Windows::UI::Xaml::Visibility::Visible;


		applyChanges->Visibility = Windows::UI::Xaml::Visibility::Visible;
		applyChanges->Content = "Add Meet";


	}


}


void GUIPOC::Inser_Meet::selectSeason_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args)
{

	DBLite sql;
	sql.getData("seasons");

	std::vector<std::string> seasons = getSeasonsData();

	sql.closeDB();

	for (auto it = seasons.cbegin(); it != seasons.cend(); ++it) {

		//weird conversion via helper method found on stackoverflow
		std::string temp = *it;

		String^ out = convertFromString(temp);

		//add string to the dropdown
		seasonSelect->Items->Append(out);

	}



}


void GUIPOC::Inser_Meet::seasonSelect_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	//remove old items
	meetSelect->Items->Clear();
	//remove the old entry if there is one
	if (meetSelect->SelectedIndex != -1) {

		//set to the default unselected item
		meetSelect->SelectedIndex = -1;


	}


	//convert to std::string
	std::wstring temp(seasonSelect->SelectedItem->ToString()->Data());
	auto selected = make_string(temp);
	//add to array of selected values
	selectedValues[2] = selected;

	DBLite sql;
	sql.getData("meet_data");

	std::vector<std::string> meet_data = getMeet_Data();

	sql.closeDB();

	for (auto it = meet_data.cbegin(); it != meet_data.cend(); ++it) {

		//weird conversion via helper method found on stackoverflow
		std::string temp = *it;

		String^ out = convertFromString(temp);

		//add string to the dropdown
		meetSelect->Items->Append(out);

	}
	//if there were no meets in this season say so and allow user to add a meet easily.
	if (meetSelect->Items->Size == 0) {

		meetSelect->Items->Append("Add a meet.");

	}
	meetSelect->IsEnabled = "True";
}


void GUIPOC::Inser_Meet::meetSelect_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{



	//convert to std::string
	std::wstring temp(meetSelect->SelectedItem->ToString()->Data());
	auto selected = make_string(temp);
	//add to array of selected values

	//if the selected option is add a meet
	if (selected.compare("Add a meet") == 0){

		editOrAdd->SelectedIndex = 1;
		//reset current option
		meetSelect->SelectedIndex = -1;

	}
	if (editOrAdd->SelectedIndex == 0 && selected.compare("") != 0) {
		//get the index of the last comma which preceeds the date in the combobox string
		int test = selected.find_last_of(',');
		//get the date from the combobox string
		std::string dateStr = selected.substr(test + 2, selected.length());

		//set the textbox string to the date that is in the meet select combox
		editDate->Text = convertFromString(dateStr);

		std::string locationString = selected.substr(selected.find(',') + 2, (test - selected.find(',') - 2));
		editLocation->Text = convertFromString(locationString);
	}
	if (selected.compare("") == 0) {
		editDate->Text = "";
		editLocation->Text = "";
	}
}


void GUIPOC::Inser_Meet::confirmChanges_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	DBLite db;
	std::wstring date(editDate->Text->ToString()->Data());
	std::wstring location(editLocation->Text->ToString()->Data());
	int meet_id = (meetSelect->SelectedIndex + 1);
	
	
	if (editOrAdd->SelectedIndex == 0) {
		//this means it is an edit
		db.updateDataMeet_Data(std::to_string(meet_id).c_str(), make_string(date).c_str() , make_string(location).c_str());
	}
	else {
		//this means it is a new addition
		db.insertNewDataMeet_Data(std::to_string(meet_id).c_str(), make_string(date).c_str(), make_string(location).c_str());

	}

	db.closeDB();

}
