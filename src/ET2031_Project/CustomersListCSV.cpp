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
//CCCD khách hàng
vector<string> CustomersListCSV::vCustomerIDs;
//Ghi chú
vector<string> CustomersListCSV::vNotes;

//Số lượng khách hàng hiện tại
int CustomersListCSV::customersCount = 0;

/// <summary>
/// Load file CSV chứa dữ liệu khách hàng và lưu vào các vector
/// </summary>
void CustomersListCSV::Load()
{
    ifstream file;
    file.open(CUSTOMERS_LIST_FILE);
    //Nếu file CSV không tồn tại, tạo file CSV mới
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
    vNotes = CSVFile.GetColumn<string>(5);

    customersCount = vCustomerIDs.size();
}

/// <summary>
/// Tạo file CSV mới nếu không có sẵn
/// </summary>
void CustomersListCSV::CreateNewFile()
{
    rapidcsv::Document doc(string(), rapidcsv::LabelParams(-1, -1)); //không nhận cột, ô nào làm label
    vector<string> v = { "Họ tên","Ngày sinh","SĐT","Địa chỉ","Số CCCD","Ghi chú"};
    doc.InsertRow<string>(0, v);

    doc.Save(CUSTOMERS_LIST_FILE);
}

/// <summary>
/// Lưu file CSV với dữ liệu đang có trong các vector
/// </summary>
void CustomersListCSV::Save()
{
    rapidcsv::Document doc = rapidcsv::Document(string(), rapidcsv::LabelParams(0, -1));
    doc.InsertColumn(0, vNames, "Họ tên");
    doc.InsertColumn(1, vBirthdates, "Ngày sinh");
    doc.InsertColumn(2, vPhoneNumbers, "SĐT");
    doc.InsertColumn(3, vAddresses, "Địa chỉ");
    doc.InsertColumn(4, vCustomerIDs, "Số CCCD");
    doc.InsertColumn(5, vNotes, "Ghi chú");

    doc.Save(CUSTOMERS_LIST_FILE);

    CustomersListCSV::Load();
}

bool CustomersListCSV::ValidateCCCD(string CCCD)
{
    if (CCCD.size() != 12) return false;
    try
    {
        //Convert sang long long để xem có phải số không
        if (stoll(CCCD) < 0)
            throw exception();
    }
    catch (const std::exception&)
    {
        return false;
    }

    return true;
}

/// <summary>
/// Kiểm tra xem số CCCD có tồn tại không
/// </summary>
/// <param name="CCCD">CCCD của khách hàng</param>
/// <returns>CCCD có tồn tại không</returns>
bool CustomersListCSV::IsIDNumberAvailable(string CCCD)
{
    if (!ValidateCCCD(CCCD)) return false;
	for (int i = 0; i < customersCount; i++)
	{
		if (CCCD == vCustomerIDs[i])
		{
            return true;
		}
	}
    return false;
}

/// <summary>
/// Kiểm tra xem số CCCD có tồn tại không và trả về tên khách hàng nếu tồn tại
/// </summary>
/// <param name="CCCD">CCCD của khách hàng</param>
/// <param name="customerName">Tên khách hàng nếu CCCD tồn tại</param>
/// <returns>CCCD có tồn tại không</returns>
bool CustomersListCSV::IsIDNumberAvailable(string CCCD, string& customerName)
{
    if (!ValidateCCCD(CCCD)) return false;
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

/// <summary>
/// Kiểm tra xem số CCCD có tồn tại không và trả về tên khách hàng nếu tồn tại
/// </summary>
/// <param name="CCCD">CCCD của khách hàng</param>
/// <param name="customerName">Tên khách hàng nếu CCCD tồn tại</param>
/// <param name="index">index trong các vector nếu CCCD tồn tại</param>
/// <returns></returns>
bool CustomersListCSV::IsIDNumberAvailable(string CCCD, string& customerName, int& index)
{
    if (!ValidateCCCD(CCCD)) return false;
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

/// <summary>
/// Thêm khách hàng
/// </summary>
void CustomersListCSV::AddCustomer()
{
    Main::ClearScreen();

    //string userInput;
    fmt::print("Nhập số CCCD khách hàng: ");
    string CCCD = Main::UnicodeInput();
    if (!ValidateCCCD(CCCD))
    {
        Main::DataInputInvalid();
        CustomersListCSV::Interface();
        return;
    }
    string customerName;
    if (IsIDNumberAvailable(CCCD, customerName))
    {
        fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::red), "CCCD trùng với khách hàng {0}", customerName);
        fmt::println("");
        Main::PauseAndBack();
        CustomersListCSV::Interface();
        return;
    }

    vCustomerIDs.push_back(CCCD);

    fmt::print("Nhập tên khách hàng: ");
    vNames.push_back(Main::UnicodeInput());

	fmt::print("Nhập ngày sinh khách hàng (dd/MM/yyyy): ");
    vBirthdates.push_back(Main::UnicodeInput());

    fmt::print("Nhập SĐT khách hàng: ");
    vPhoneNumbers.push_back(Main::UnicodeInput());
    
    fmt::print("Nhập địa chỉ khách hàng: ");
    vAddresses.push_back(Main::UnicodeInput());
        
    fmt::print("Nhập ghi chú: ");
    vNotes.push_back(Main::UnicodeInput());

    CustomersListCSV::Save();

    fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Thêm khách hàng thành công");
    fmt::println("");
    Main::PauseAndBack();
    CustomersListCSV::Interface();
}

/// <summary>
/// Tìm khách hàng bằng số CCCD
/// </summary>
void CustomersListCSV::FindByIDNumber()
{
    Main::ClearScreen();
    fmt::print("Nhập số CCCD của khách hàng: ");
    string CCCD = Main::UnicodeInput();
    bool isCustomerAvail = false;
    int index = -1;

    for (int i = 0; i < customersCount; i++)
    {
        if (CCCD == vCustomerIDs[i])
        {
            isCustomerAvail = true;
            index = i;
            fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Đã tìm thấy khách hàng!");
            fmt::println("\n");
            fmt::println("Tên khách hàng: " + vNames[i]);
            fmt::println("Ngày sinh: " + vBirthdates[i]);
            fmt::println("SĐT: " + vPhoneNumbers[i]);
            fmt::println("Địa chỉ: " + vAddresses[i]);
            fmt::println("Số CCCD: " + CCCD);
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
        fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Không tìm thấy khách hàng!");
        fmt::println("");
    }

    Main::PauseAndBack();
    CustomersListCSV::Interface();
}

/// <summary>
/// Thay đổi thông tin khách hàng
/// </summary>
/// <param name="index">index của vector chứa thông tin khách hàng cần thay đổi</param>
void CustomersListCSV::EditCustomerInfo(int index)
{
    Main::ClearScreen();
    fmt::println("[1] Tên khách hàng: " + vNames[index]);
    fmt::println("[2] Ngày sinh: " + vBirthdates[index]);
    fmt::println("[3] SĐT: " + vPhoneNumbers[index]);
    fmt::println("[4] Địa chỉ: " + vAddresses[index]);
    fmt::println("[5] Số CCCD: " + vCustomerIDs[index]);
    fmt::println("[6] Ghi chú: {0}\n", vNotes[index]);
    fmt::println("[7] Quay lại màn hình khách hàng");

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
        fmt::print("Nhập số CCCD mới: ");
        string newCCCD = Main::UnicodeInput();
        //Thay đổi CCCD trên tất cả các khoản vay hiện tại
        LoansListCSV::EditLoanCCCD(vCustomerIDs[index], newCCCD);
        vCustomerIDs[index] = newCCCD;
    }
    else if (userChoice == "6")
    {
        fmt::print("Nhập ghi chú mới: ");
        vNotes[index] = Main::UnicodeInput();
    }
    else if (userChoice == "7")
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
    CustomersListCSV::Save();
    fmt::println("");
    fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Sửa thông tin thành công");
    fmt::println("");
}

/// <summary>
/// Xoá khách hàng đồng thời toàn bộ khoản vay khách hàng đó
/// </summary>
void CustomersListCSV::RemoveCustomer()
{
    Main::ClearScreen();
    fmt::print("Nhập CCCD của khách hàng: ");
    string CCCD = Main::UnicodeInput();
    string customerName;
    int index = 0;
    if (!CustomersListCSV::IsIDNumberAvailable(CCCD, customerName, index))
    {
        fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Không tìm thấy khách hàng với số CCCD trên");
        fmt::println("");
        Main::PauseAndBack();
        CustomersListCSV::Interface();
    }

    fmt::println("Bạn có muốn xoá khách hàng {0} với số CCCD là {1}", customerName, CCCD);
    fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Mọi dữ liệu bao gồm tất cả các khoản vay của khách hàng đều sẽ bị xoá");
    fmt::println("");
    fmt::print("Nhấn Y để xoá, N để huỷ: ");
    string userInput = Main::UnicodeInput();

    if (userInput == "y" || userInput == "Y")
    {
        //Xoá tất cả khoản vay của khách hàng
        LoansListCSV::RemoveCustomerLoan(CCCD);
        CSVFile.RemoveRow(index);
        CSVFile.Save();
        CustomersListCSV::Load();
        fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Xoá khách hàng thành công");
        fmt::println("");
    }

    Main::PauseAndBack();
    CustomersListCSV::Interface();
}

/// <summary>
/// Xoá khách hàng (dùng ở hàm tìm thông tin khách hàng)
/// </summary>
/// <param name="index">index của vector chứa thông tin khách hàng</param>
void CustomersListCSV::RemoveCustomer(int index)
{
    fmt::println("Bạn có muốn xoá khách hàng {0} với số CCCD là {1}", vNames[index], vCustomerIDs[index]);
    fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Mọi dữ liệu bao gồm tất cả các khoản vay của khách hàng đều sẽ bị xoá");
    fmt::println("");
    fmt::print("Nhấn Y để xoá, N để huỷ: ");
    string userInput = Main::UnicodeInput();

    if (userInput == "y" || userInput == "Y")
    {
        LoansListCSV::RemoveCustomerLoan(vCustomerIDs[index]);
        CSVFile.RemoveRow(index);
        CSVFile.Save();
        CustomersListCSV::Load();
        fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Xoá khách hàng thành công");
        fmt::println("");
    }
}

/// <summary>
/// Giao diện khách hàng
/// </summary>
void CustomersListCSV::Interface()
{
	Main::ClearScreen();

    fmt::println("                _______________________________________________________________\n");

    fmt::println("                        [1] Thêm khách hàng\n");

    fmt::println("                        [2] Tìm khách hàng theo số CCCD\n");

    fmt::println("                        [3] Xoá khách hàng\n");

    fmt::println("                        [4] Quay lại màn hình chính");

    fmt::println("                _______________________________________________________________\n");

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
        fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::red), "Lựa chọn không hợp lệ!");
        fmt::println("");
        Main::PauseAndBack();
        CustomersListCSV::Interface();
    }
}
