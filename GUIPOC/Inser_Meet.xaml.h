//
// Inser_Meet.xaml.h
// Declaration of the Inser_Meet class
//

#pragma once

#include "Inser_Meet.g.h"

namespace GUIPOC
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Inser_Meet sealed
	{
	public:
		Inser_Meet();
	private:
		void listBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void editOrAdd_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void selectSeason_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args);
		void seasonSelect_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void meetSelect_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void confirmChanges_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void meetNumber_Loading(Windows::UI::Xaml::FrameworkElement^ sender, Platform::Object^ args);
		void sbOrSki_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}
