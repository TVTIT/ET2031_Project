#include <string>
#include <vector>
#include "fmt/base.h"
#include "fmt/color.h"
#include "rapidcsv.h"
#include "CustomersListCSV.h"
#include "Main.h"

using namespace std;

rapidcsv::Document CustomersListCSV::CSVFile;

vector<string> CustomersListCSV::vNames;
vector<string> CustomersListCSV::vBirthdates;
vector<string> CustomersListCSV::vPhoneNumbers;
vector<string> CustomersListCSV::vAddresses;
vector<string> CustomersListCSV::vCustomerIDs;
vector<string> CustomersListCSV::vCustomerGroups;
vector<string> CustomersListCSV::vNotes;

int CustomersListCSV::customersCount;

void CustomersListCSV::Load()
{
	CSVFile = rapidcsv::Document(Main::DirectoryPath + "\\customers_list.csv", rapidcsv::LabelParams(0, -1));

    vNames = CSVFile.GetColumn<string>(0);
    vBirthdates = CSVFile.GetColumn<string>(1);
    vPhoneNumbers = CSVFile.GetColumn<string>(2);
    vAddresses = CSVFile.GetColumn<string>(3);
    vCustomerIDs = CSVFile.GetColumn<string>(4);
    vCustomerGroups = CSVFile.GetColumn<string>(5);
    vNotes = CSVFile.GetColumn<string>(6);

    customersCount = vCustomerIDs.size();
}

void CustomersListCSV::Save()
{
    for (int i = 6; i >= 0; i--)
    {
        CSVFile.RemoveColumn(i);
    }
    CSVFile.InsertColumn(0, vNames, "Họ tên");
    CSVFile.InsertColumn(1, vBirthdates, "Ngày sinh");
    CSVFile.InsertColumn(2, vPhoneNumbers, "SĐT");
    CSVFile.InsertColumn(3, vAddresses, "Địa chỉ");
    CSVFile.InsertColumn(4, vCustomerIDs, "Số CCCD/CMND");
    CSVFile.InsertColumn(5, vCustomerGroups, "Nhóm khách hàng");
    CSVFile.InsertColumn(6, vNotes, "Ghi chú");

    CSVFile.Save();

    CustomersListCSV::Load();
}

bool CustomersListCSV::IsIDNumberAvailable(string CCCD)
{
	for (int i = 0; i < customersCount; i++)
	{
		if (CCCD == vCustomerIDs[i])
		{
            return true;
		}
	}
    return false;
}

bool CustomersListCSV::IsIDNumberAvailable(string CCCD, string& customerName)
{
    for (int i = 0; i < customersCount; i++)
    {
        if (CCCD == vCustomerIDs[i])
        {
            customerName = vNames[i];
            return true;
        }
    }
    return false;
}

bool CustomersListCSV::IsIDNumberAvailable(string CCCD, string& customerName, int& index)
{
    for (int i = 0; i < customersCount; i++)
    {
        if (CCCD == vCustomerIDs[i])
        {
            customerName = vNames[i];
            index = i;
            return true;
        }
    }
    return false;
}

void CustomersListCSV::AddCustomer()
{
    Main::ClearScreen();

    //string userInput;
    fmt::print("Nhập tên khách hàng: ");
    vNames.push_back(Main::UnicodeInput());

	fmt::print("Nhập ngày sinh khách hàng (dd/MM/yyyy): ");
    vBirthdates.push_back(Main::UnicodeInput());

    fmt::print("Nhập SĐT khách hàng: ");
    vPhoneNumbers.push_back(Main::UnicodeInput());
    
    fmt::print("Nhập địa chỉ khách hàng: ");
    vAddresses.push_back(Main::UnicodeInput());
    
    fmt::print("Nhập số CCCD/CMND khách hàng: ");
    vCustomerIDs.push_back(Main::UnicodeInput());
    
    fmt::print("Nhập nhóm khách hàng (1, 2, 3, 4): ");
    vCustomerGroups.push_back(Main::UnicodeInput());
    
    fmt::print("Nhập ghi chú: ");
    vNotes.push_back(Main::UnicodeInput());

    CustomersListCSV::Save();

    fmt::println("Thêm khách hàng thành công");
    Main::PauseAndBack();
    CustomersListCSV::Interface();
}

void CustomersListCSV::FindByIDNumber()
{
    Main::ClearScreen();
    fmt::print("Nhập số CCCD/CMND của khách hàng: ");
    string CCCD = Main::UnicodeInput();

    for (int i = 0; i < customersCount; i++)
    {
        if (CCCD == CustomersListCSV::vCustomerIDs[i])
        {
            fmt::println("Đã tìm thấy khách hàng!\n");
            fmt::println("Tên khách hàng: " + vNames[i]);
            fmt::println("Ngày sinh: " + vBirthdates[i]);
            fmt::println("SĐT: " + vPhoneNumbers[i]);
            fmt::println("Địa chỉ: " + vAddresses[i]);
            fmt::println("Số CCCD/CMND: " + CCCD);
            fmt::println("Nhóm khách hàng: " + vCustomerGroups[i]);
            fmt::println("Ghi chú: {0}\n", vNotes[i]);

            Main::PauseAndBack();
            CustomersListCSV::Interface();
            return;
        }
    }
    fmt::println("Không tìm thấy khách hàng!");
    Main::PauseAndBack();
    CustomersListCSV::Interface();
}

void CustomersListCSV::RemoveCustomer()
{
    Main::ClearScreen();
    fmt::print("Nhập CCCD/CMND của khách hàng: ");
    string CCCD = Main::UnicodeInput();
    string customerName;
    int index = 0;
    if (!CustomersListCSV::IsIDNumberAvailable(CCCD, customerName, index))
    {
        fmt::println("Không tìm thấy khách hàng với số CCCD/CMND trên");
        Main::PauseAndBack();
        CustomersListCSV::Interface();
    }
    
    fmt::println("Bạn có muốn xoá khách hàng {0} với số CMND/CCCD là {1}", customerName, CCCD);
    fmt::print("Nhấn Y để xoá, N để huỷ: ");
    string userInput = Main::UnicodeInput();
    
    if (userInput == "y" || userInput == "Y")
    {
        CSVFile.RemoveRow(index);
        CSVFile.Save();
        CustomersListCSV::Load();
        fmt::println("Xoá khách hàng thành công");
    }

    Main::PauseAndBack();
    CustomersListCSV::Interface();
}

void CustomersListCSV::Interface()
{
	Main::ClearScreen();
	fmt::println("[1] Thêm khách hàng");
	fmt::println("[2] Tìm khách hàng theo số CCCD/CMND");
	fmt::println("[3] Xoá khách hàng");
    fmt::println("[4] Quay lại màn hình chính");
    fmt::print("Nhập lựa chọn của bạn: ");

    string userInput;
    getline(cin, userInput);

    if (userInput == "1")
    {
        CustomersListCSV::AddCustomer();
    }
    else if (userInput == "2")
    {
        CustomersListCSV::FindByIDNumber();
    }
    else if (userInput == "3")
    {
        CustomersListCSV::RemoveCustomer();
    }
    else if (userInput == "4")
    {
        Main::Interface();
    }
    else
    {
        fmt::println("Lựa chọn không hợp lệ!");
        Main::PauseAndBack();
        CustomersListCSV::Interface();
    }
}
