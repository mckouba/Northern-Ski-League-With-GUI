//
// BlankPage.xaml.cpp
// Implementation of the BlankPage class
//

#include "pch.h"
#include "BlankPage.xaml.h"

#include "DBLite.h"
#include "ImportAndExport.h"
#include "Helpers.h"
#include <locale>
#include <codecvt>
#include <string>
#include <iostream>

using namespace GUIPOC;

using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

BlankPage::BlankPage()
{
	InitializeComponent();
}

void GUIPOC::BlankPage::raceAttribute_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{

	//convert to std::string
	std::wstring temp(disciplineSelect->SelectedItem->ToString()->Data());
	auto selected = make_string(temp);
	//add to array of selected values
	selectedValues[0] = selected;

}


void GUIPOC::BlankPage::browse_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

		filePath->Text = "";
	
		FileOpenPicker^ openPicker = ref new FileOpenPicker();
		openPicker->ViewMode = PickerViewMode::List;
		openPicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
		openPicker->FileTypeFilter->Append(".csv");

		create_task(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
		{

			if (file) {
	
				filePath->Text = file->Path;

			}
			else {
				filePath->Text = "An error has occured";
			}

		});

}





void GUIPOC::BlankPage::gender_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	//convert to std::string
	std::wstring temp(genderSelect->SelectedItem->ToString()->Data());
	auto selected = make_string(temp);
	//add to array of selected values
	selectedValues[1] = selected;
}


void GUIPOC::BlankPage::season_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
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


void GUIPOC::BlankPage::meet_SelectionChagned(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{

	//convert to std::string
	std::wstring temp(meetSelect->SelectedItem->ToString()->Data());
	auto selected = make_string(temp);
	//add to array of selected values
	selectedValues[3] = selected;

}


void GUIPOC::BlankPage::test(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args)
{



}


void GUIPOC::BlankPage::season_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args)
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



void GUIPOC::BlankPage::meetSelect_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args)
{


}


