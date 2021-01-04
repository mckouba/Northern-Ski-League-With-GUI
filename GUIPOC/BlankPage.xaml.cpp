﻿//
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
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

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

//global constant
//I know it's bad but I'm lazy and it works
Platform::Object^ prev[5];


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

				//make the columns visible
				colSelection1->Visibility = Windows::UI::Xaml::Visibility::Visible;
				colSelection2->Visibility = Windows::UI::Xaml::Visibility::Visible;
				colSelection3->Visibility = Windows::UI::Xaml::Visibility::Visible;
				colSelection4->Visibility = Windows::UI::Xaml::Visibility::Visible;


				//create_task(FileIO::ReadTextAsync(file)).then([this](Platform::String^ outputText)) {

				//});

				//inputFile;
				


				std::vector<std::string> out;

				if (out.size() == 4) {
					col1_text->Text = convertFromString(out.at(0));

					col2_text->Text = convertFromString(out.at(1));
					col3_text->Text = convertFromString(out.at(2));
					col4_text->Text = convertFromString(out.at(3));
				}



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

	//make the confirm visible
	insertConfirm->Visibility = Windows::UI::Xaml::Visibility::Visible;

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




void GUIPOC::BlankPage::insertConfirm_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{


	


}


void GUIPOC::BlankPage::col1_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	//this is so stupid
	//either I'm stupid or the way that the equals() method works reguarding items from a combobox is stupid and will always return 0 when comparing
	//prev[0] and prev[4] even though they are the same object

	//ensure that the 'Value of this column' item is not removed
	if (col1->SelectedItem->ToString()->Length() != 20) {


		//remove items from other comboboxes
		unsigned int index;
		//remove from second col
		col2->Items->IndexOf(col1->SelectedItem, &index);
		col2->Items->RemoveAt(index);
		//remove from third col
		col3->Items->IndexOf(col1->SelectedItem, &index);
		col3->Items->RemoveAt(index);
		//remove from fourth col
		col4->Items->IndexOf(col1->SelectedItem, &index);
		col4->Items->RemoveAt(index);


	}
	//if the first item is still the select value of column option
	if(prev[0]->ToString()->Length() != prev[4]->ToString()->Length()){
		//time to replace the item that was just unselected
		col2->Items->InsertAt(0, prev[0]);
		col3->Items->InsertAt(0, prev[0]);
		col4->Items->InsertAt(0, prev[0]);
	}

	prev[0] = col1->SelectedItem;
}

void GUIPOC::BlankPage::col2_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	//ensure that the 'Value of this column' item is not removed
	if (col2->SelectedItem->ToString()->Length() != 20) {
		//remove items from other comboboxes
		unsigned int index;
		//remove from second col
		col1->Items->IndexOf(col2->SelectedItem, &index);
		col1->Items->RemoveAt(index);
		//remove from third col
		col3->Items->IndexOf(col2->SelectedItem, &index);
		col3->Items->RemoveAt(index);
		//remove from fourth col
		col4->Items->IndexOf(col2->SelectedItem, &index);
		col4->Items->RemoveAt(index);


	}
	if(prev[1]->ToString()->Length() != prev[4]->ToString()->Length()) {
		col1->Items->InsertAt(0, prev[1]);
		col3->Items->InsertAt(0, prev[1]);
		col4->Items->InsertAt(0, prev[1]);
	}
	prev[1] = col2->SelectedItem;
}


void GUIPOC::BlankPage::col3_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	//ensure that the 'Value of this column' item is not removed
	if (col3->SelectedItem->ToString()->Length() != 20) {
		//remove items from other comboboxes
		unsigned int index;
		//remove from second col
		col1->Items->IndexOf(col3->SelectedItem, &index);
		col1->Items->RemoveAt(index);
		//remove from second col
		col2->Items->IndexOf(col3->SelectedItem, &index);
		col2->Items->RemoveAt(index);
		//remove from fourth col
		col4->Items->IndexOf(col3->SelectedItem, &index);
		col4->Items->RemoveAt(index);

	}
	if(prev[2]->ToString()->Length() != prev[4]->ToString()->Length()) {

		col1->Items->InsertAt(0, prev[2]);
		col2->Items->InsertAt(0, prev[2]);
		col4->Items->InsertAt(0, prev[2]);

	}

	prev[2] = col3->SelectedItem;
}


void GUIPOC::BlankPage::col4_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	//ensure that the 'Value of this column' item is not removed
	if (col4->SelectedItem->ToString()->Length() != 20) {

		//remove items from other comboboxes
		unsigned int index;
		//remove from second col
		col1->Items->IndexOf(col4->SelectedItem, &index);
		col1->Items->RemoveAt(index);
		//remove from third col
		col3->Items->IndexOf(col4->SelectedItem, &index);
		col3->Items->RemoveAt(index);
		//remove from second col
		col2->Items->IndexOf(col4->SelectedItem, &index);
		col2->Items->RemoveAt(index);

	}
	if (prev[3]->ToString()->Length() != prev[4]->ToString()->Length()) {

		col1->Items->InsertAt(0, prev[3]);
		col3->Items->InsertAt(0, prev[3]);
		col2->Items->InsertAt(0, prev[3]);

	}

	prev[3] = col4->SelectedItem;
}


void GUIPOC::BlankPage::col1_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args)
{

	//initilize so that the item is not null
	prev[0] = col1->Items->GetAt(col1->Items->Size - 1);
	prev[1] = col2->Items->GetAt(col2->Items->Size - 1);
	prev[2] = col3->Items->GetAt(col3->Items->Size - 1);
	prev[3] = col4->Items->GetAt(col4->Items->Size - 1);
	
	//this is the 'value of this column' object
	prev[4] = col1->Items->GetAt(col1->Items->Size - 1);

}
