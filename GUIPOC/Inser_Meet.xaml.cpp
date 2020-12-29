//
// Inser_Meet.xaml.cpp
// Implementation of the Inser_Meet class
//

#include "pch.h"
#include "Inser_Meet.xaml.h"

#include "Helpers.h"
#include "DBLite.h"
#include "ImportAndExport.h"

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

	}
	else {
		//only other option is add

		//enable the season select
		seasonSelect->IsEnabled = "True";

		//enable the add new meet stackpanel
		newMeetInfo->Visibility = Windows::UI::Xaml::Visibility::Visible;


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

	meetSelect->IsEnabled = "True";
}


void GUIPOC::Inser_Meet::meetSelect_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{


	auto temp = editDate->Date;



}
