//
// BlankPage.xaml.h
// Declaration of the BlankPage class
//

#pragma once

#include "BlankPage.g.h"

namespace GUIPOC
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class BlankPage sealed
	{
	public:
		BlankPage();
	private:
		void browse_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void raceAttribute_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void gender_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void season_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void meet_SelectionChagned(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void test(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args);
		void season_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args);
		void meetSelect_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args);
		//		void updateMeetBox();
		void insertConfirm_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void col1_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void col2_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void col3_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void col4_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void col1_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args);
		int GUIPOC::BlankPage::findCol(std::string input);
	};
}
