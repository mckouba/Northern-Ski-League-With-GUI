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

	seasonSelect->IsEnabled = false;

	editDate->IsEnabled = false;
	editLocation->IsEnabled = false;

	meetSelect->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	editMeetInfo->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	newMeetInfo->Visibility = Windows::UI::Xaml::Visibility::Collapsed;


	if (make_string(editOrAdd->SelectedItem->ToString()->Data()).compare("Edit") == 0) {


		//enable season select
		seasonSelect->IsEnabled = true; 
		
		
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
		seasonSelect->IsEnabled = true;

		//enable the add new meet stackpanel
		newMeetInfo->Visibility = Windows::UI::Xaml::Visibility::Visible;


		applyChanges->Visibility = Windows::UI::Xaml::Visibility::Visible;
		applyChanges->Content = "Add Meet";

		//if switching from edit to add we need to disable the edit boxes so if
		//it is changed back to edit there cannot be any fields editable when they
		//should not be
		
		if (editDate->IsEnabled.ToString()->Length() == 4) {

			editDate->IsEnabled = false;
			editLocation->IsEnabled = false;

			adddate->IsEnabled = true;
			location->IsEnabled = true;
			meetNumber->IsEnabled = true;

		}
	}

	meetSelect->SelectedIndex = -1;

}

void GUIPOC::Inser_Meet::selectSeason_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args)
{
	DBLite sql;
	sql.getData("seasons");

	std::vector<std::string> seasons = getSeasonsData();

	sql.closeDB();

	//populate the meet number combobox with 1-6
	for (int i = 1; i <= 6; i++) {

		meetNumber->Items->Append(i);

	}

	for (auto it = seasons.cbegin(); it != seasons.cend(); ++it) {

		//weird conversion via helper method found on stackoverflow
		std::string temp = *it;

		String^ out = convertFromString(temp);

		//add string to the dropdown
		seasonSelect->Items->Append(out);

		//remove the existing meet numbers from the meet number combobox






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

		//move this to the seasonSelecte_SelectionChanged method so it is updated when the season is changed
		//get index of the item to be removed
		unsigned int index;

		Platform::Object^ test = std::stoi(temp.substr(6, 1));

		meetNumber->Items->IndexOf(test, &index);

		meetNumber->Items->RemoveAt(index);

	}
	//if there were no meets in this season say so and allow user to add a meet easily.
	if (meetSelect->Items->Size == 0) {

		meetSelect->Items->Append("Add a meet.");

	}
	//allow meet selection
	meetSelect->IsEnabled = true;
	//allow the addition of another meet
	location->IsEnabled = true;
	adddate->IsEnabled = true;
	meetNumber->IsEnabled = true;
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

	//unhides buttons if season is selected
	if (editOrAdd->SelectedIndex == 1) {

	}
	else if(editOrAdd->SelectedIndex == 0 && meetSelect->SelectedIndex != -1) {
		editDate->IsEnabled = true;
		editLocation->IsEnabled = true;
	}
}


void GUIPOC::Inser_Meet::confirmChanges_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	DBLite db;

	
	
	if (editOrAdd->SelectedIndex == 0) {
		//this means it is an edit
		std::wstring date(editDate->Text->ToString()->Data());
		std::wstring location(editLocation->Text->ToString()->Data());
		int meet_id = (meetSelect->SelectedIndex + 1);
		db.updateDataMeet_Data(std::to_string(meet_id).c_str(), make_string(date).c_str() , make_string(location).c_str());
	}
	else {
		//this means it is a new addition
		std::wstring date(adddate->Text->ToString()->Data());
		std::wstring location(location->Text->ToString()->Data());
		std::wstring meet_id(meetNumber->SelectedItem->ToString()->Data());
		db.insertNewDataMeet_Data(make_string(meet_id).c_str(), make_string(date).c_str(), make_string(location).c_str());

	}

	db.closeDB();

	meetSelect->SelectedIndex = -1;
	adddate->Text = "";
	location->Text = "";
	
	applyChanges->Flyout->Hide();
	//reset the items that are displayed in the meet select box
	int selectedIndex = seasonSelect->SelectedIndex;

	seasonSelect->SelectedIndex = -1;
	seasonSelect->SelectedIndex = selectedIndex;

}


void GUIPOC::Inser_Meet::meetNumber_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args)
{







}
