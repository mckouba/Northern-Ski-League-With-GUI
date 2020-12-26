//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "new_race_view1.xaml.h"
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


// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}

void GUIPOC::MainPage::nav_Item_Invoked(Windows::UI::Xaml::Controls::NavigationView^ sender, Windows::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args)
{


	auto navitemtag = args->InvokedItemContainer->Tag->ToString();
	if (navitemtag == "b") {
		Windows::UI::Xaml::Interop::TypeName temp = (BlankPage::typeid);
		contentFrame->Navigate(temp);
	}



}