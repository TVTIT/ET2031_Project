#include <string>
#include <vector>
#include "fmt/base.h"
#include "fmt/color.h"
#include "rapidcsv.h"
#include "CustomersListCSV.h"
#include "Main.h"
#include "LoansListCSV.h"

#define CUSTOMERS_LIST_FILE Main::DirectoryPath + "\\customers_list.csv"

using namespace std;

rapidcsv::Document CustomersListCSV::CSVFile;

//Tên khách hàng
vector<string> CustomersListCSV::vNames;
//Ngày sinh khách hàng
vector<string> CustomersListCSV::vBirthdates;
//SĐT khách hàng
vector<string> CustomersListCSV::vPhoneNumbers;
//Địa chỉ khách hàng
vector<string> CustomersListCSV::vAddresses;
//CMND/CCCD khách hàng
vector<string> CustomersListCSV::vCustomerIDs;
//Nhóm khách hàng
vector<string> CustomersListCSV::vCustomerGroups;
//Ghi chú
vector<string> CustomersListCSV::vNotes;

int CustomersListCSV::customersCount = 0;

void CustomersListCSV::Load()
{
    ifstream file;
    file.open(CUSTOMERS_LIST_FILE);
    if (!file)
    {
        CustomersListCSV::CreateNewFile();
        CSVFile = rapidcsv::Document(CUSTOMERS_LIST_FILE, rapidcsv::LabelParams(0, -1));
        return;
    }
    file.close();

	CSVFile = rapidcsv::Document(CUSTOMERS_LIST_FILE, rapidcsv::LabelParams(0, -1));

    vNames = CSVFile.GetColumn<string>(0);
    vBirthdates = CSVFile.GetColumn<string>(1);
    vPhoneNumbers = CSVFile.GetColumn<string>(2);
    vAddresses = CSVFile.GetColumn<string>(3);
    vCustomerIDs = CSVFile.GetColumn<string>(4);
    vCustomerGroups = CSVFile.GetColumn<string>(5);
    vNotes = CSVFile.GetColumn<string>(6);

    customersCount = vCustomerIDs.size();
}

void CustomersListCSV::CreateNewFile()
{
    rapidcsv::Document doc(string(), rapidcsv::LabelParams(-1, -1)); //không nhận cột, ô nào làm label
    vector<string> v = { "Họ tên","Ngày sinh","SĐT","Địa chỉ","Số CCCD / CMND","Nhóm khách hàng","Ghi chú"};
    doc.InsertRow<string>(0, v);

    doc.Save(CUSTOMERS_LIST_FILE);
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
    bool isCustomerAvail = false;
    int index = -1;

    for (int i = 0; i < customersCount; i++)
    {
        if (CCCD == vCustomerIDs[i])
        {
            isCustomerAvail = true;
            index = i;
            fmt::println("Đã tìm thấy khách hàng!\n");
            fmt::println("Tên khách hàng: " + vNames[i]);
            fmt::println("Ngày sinh: " + vBirthdates[i]);
            fmt::println("SĐT: " + vPhoneNumbers[i]);
            fmt::println("Địa chỉ: " + vAddresses[i]);
            fmt::println("Số CCCD/CMND: " + CCCD);
            fmt::println("Nhóm khách hàng: " + vCustomerGroups[i]);
            fmt::println("Ghi chú: {0}\n", vNotes[i]);

            break;
        }
    }
    
    if (isCustomerAvail)
    {
        fmt::println("\nCác thao tác với khách hàng");
        fmt::println("[1] Sửa thông tin khách hàng");
        fmt::println("[2] Xoá khách hàng");
        fmt::println("[3] Thêm khoản vay với khách hàng trên");
        fmt::println("[4] Liệt kê các khoản vay của khách hàng trên");
        fmt::println("[5] Quay lại màn hình khách hàng");

        fmt::print("\nNhập lựa chọn của bạn: ");
        string userInput = Main::UnicodeInput();
        if (userInput == "1")
        {
            CustomersListCSV::EditCustomerInfo(index);
        }
        else if (userInput == "2")
        {
            CustomersListCSV::RemoveCustomer(index);
        }
        else if (userInput == "3")
        {
            LoansListCSV::AddLoan(CCCD);
            return;
        }
        else if (userInput == "4")
        {
            Main::ClearScreen();
            LoansListCSV::FindLoanByCCCD(CCCD);
            return;
        }
        else if (userInput == "5")
        {
            CustomersListCSV::Interface();
            return;
        }
    }
    else
    {
        fmt::println("Không tìm thấy khách hàng!");
    }

    Main::PauseAndBack();
    CustomersListCSV::Interface();
}

void CustomersListCSV::EditCustomerInfo(int index)
{
    Main::ClearScreen();
    fmt::println("[1] Tên khách hàng: " + vNames[index]);
    fmt::println("[2] Ngày sinh: " + vBirthdates[index]);
    fmt::println("[3] SĐT: " + vPhoneNumbers[index]);
    fmt::println("[4] Địa chỉ: " + vAddresses[index]);
    fmt::println("[5] Số CCCD/CMND: " + vCustomerIDs[index]);
    fmt::println("[6] Nhóm khách hàng: " + vCustomerGroups[index]);
    fmt::println("[7] Ghi chú: {0}\n", vNotes[index]);
    fmt::println("[8] Quay lại màn hình khách hàng");

    fmt::print("\nNhập phần bạn muốn chỉnh sửa: ");
    string userChoice = Main::UnicodeInput();

    if (userChoice == "1")
    {
        fmt::print("Nhập tên khách hàng mới: ");
        vNames[index] = Main::UnicodeInput();
    }
    else if (userChoice == "2")
    {
        fmt::print("Nhập ngày sinh mới: ");
        vBirthdates[index] = Main::UnicodeInput();
    }
    else if (userChoice == "3")
    {
        fmt::print("Nhập SĐT mới: ");
        vPhoneNumbers[index] = Main::UnicodeInput();
    }
    else if (userChoice == "4")
    {
        fmt::print("Nhập địa chỉ mới: ");
        vAddresses[index] = Main::UnicodeInput();
    }
    else if (userChoice == "5")
    {
        fmt::print("Nhập số CMND/CCCD mới: ");
        vCustomerIDs[index] = Main::UnicodeInput();
    }
    else if (userChoice == "6")
    {
        fmt::print("Nhập nhóm khách hàng mới: ");
        vCustomerGroups[index] = Main::UnicodeInput();
    }
    else if (userChoice == "7")
    {
        fmt::print("Nhập ghi chú mới: ");
        vNotes[index] = Main::UnicodeInput();
    }
    else if (userChoice == "8")
    {
        CustomersListCSV::Interface();
        return;
    }
    else
    {
        fmt::println("Lựa chọn không hợp lệ");
        Main::PauseAndBack();
        CustomersListCSV::EditCustomerInfo(index);
        return;
    }
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
    fmt::println("Mọi dữ liệu bao gồm tất cả các khoản vay của khách hàng đều sẽ bị xoá");
    fmt::print("Nhấn Y để xoá, N để huỷ: ");
    string userInput = Main::UnicodeInput();

    if (userInput == "y" || userInput == "Y")
    {
        LoansListCSV::RemoveCustomerLoan(CCCD);
        CSVFile.RemoveRow(index);
        CSVFile.Save();
        CustomersListCSV::Load();
        fmt::println("Xoá khách hàng thành công");
    }

    Main::PauseAndBack();
    CustomersListCSV::Interface();
}

void CustomersListCSV::RemoveCustomer(int index)
{
    fmt::println("Bạn có muốn xoá khách hàng {0} với số CMND/CCCD là {1}", vNames[index], vCustomerIDs[index]);
    fmt::println("Mọi dữ liệu bao gồm tất cả các khoản vay của khách hàng đều sẽ bị xoá");
    fmt::print("Nhấn Y để xoá, N để huỷ: ");
    string userInput = Main::UnicodeInput();

    if (userInput == "y" || userInput == "Y")
    {
        LoansListCSV::RemoveCustomerLoan(vCustomerIDs[index]);
        CSVFile.RemoveRow(index);
        CSVFile.Save();
        CustomersListCSV::Load();
        fmt::println("Xoá khách hàng thành công");
    }
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
