#define FMT_HEADER_ONLY
#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>
#include <fcntl.h>
#include "rapidcsv.h"
#include "fmt/base.h"
#include "fmt/color.h"

#include "Main.h"
#include "CustomersListCSV.h"
#include "LoansListCSV.h"


using namespace std;

string Main::DirectoryPath;

/// <summary>
/// Lấy đường dẫn của file .exe và lưu vào string Main::DirectoryPath
/// Source: ChatGPT
/// </summary>
void Main::GetPath()
{
    char PathWithExe[MAX_PATH];
    GetModuleFileNameA(NULL, PathWithExe, MAX_PATH);
    string str_PathWithExe(PathWithExe);
    Main::DirectoryPath = str_PathWithExe.substr(0, str_PathWithExe.find_last_of("\\/"));
}

/// <summary>
/// Lấy input từ bàn phím, cho phép ký tự có dấu (Unicode)
/// </summary>
/// <returns></returns>
string Main::UnicodeInput()
{
    //Hàm _setmode: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setmode?view=msvc-170
    //Chỉnh chế độ UTF-16 để hỗ trợ tiếng Việt có dấu, sau đó đặt lại chế độ ban đầu để tránh xung đột
    _setmode(_fileno(stdin), _O_U16TEXT);
    wstring w_userInput;
    getline(wcin, w_userInput);
    _setmode(_fileno(stdin), _O_TEXT);

    //Convert wstring sang string (Source: ChatGPT)
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, w_userInput.c_str(), -1, nullptr, 0, nullptr, nullptr);
    string s_userInput(size_needed - 1, 0);  // Trừ đi 1 để bỏ ký tự \0
    WideCharToMultiByte(CP_UTF8, 0, w_userInput.c_str(), -1, &s_userInput[0], size_needed, nullptr, nullptr);
    return s_userInput;
}

/// <summary>
/// Khởi tạo console: chỉnh sửa kích thước console, cho phép output có màu và đặt title
/// </summary>
void Main::InitializeConsole()
{
    //Chỉnh sửa kích thước console
    //Source: https://cplusplus.com/forum/beginner/1481/
    HWND console = GetConsoleWindow();
    RECT console_rect;
    GetWindowRect(console, &console_rect);
    MoveWindow(console, console_rect.left, console_rect.top, 800, 600, TRUE);

    //Hỗ trợ output console bằng màu
    //Source: https://github.com/fmtlib/fmt/issues/1794#issuecomment-874161111
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    //Đặt tiêu đề cho console
    wstring consoleTitle = L"Phần mềm Quản lý hệ thống vay nợ ";
    consoleTitle.append(VERSION_LABEL);

    SetConsoleTitleW(consoleTitle.c_str());
}

/// <summary>
/// Xoá hết chữ trên console thay vì phải gọi system("cls") thiếu bảo mật
/// Source: https://stackoverflow.com/a/39762968
/// </summary>
void Main::ClearScreen()
{
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR)' ',
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
        hStdOut,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);
}

/// <summary>
/// Chia string thành các vector<string> bằng 1 ký tự phân tách
/// Source: https://stackoverflow.com/a/10058725
/// </summary>
/// <param name="input">string cần chia</param>
/// <param name="pattern">Ký tự phân tách</param>
/// <returns></returns>
vector<string> Main::SplitString(string input, char pattern)
{
    stringstream sstream(input);
    string segment;
    vector<string> input_splited;

    while (getline(sstream, segment, pattern))
    {
        input_splited.push_back(segment);
    }
    return input_splited;
}

/// <summary>
/// Kiểm tra ngày tháng xem có đúng định dạng dd/MM/yyyy không
/// </summary>
/// <param name="date">string để kiểm tra</param>
/// <returns></returns>
bool Main::ValidateDate(string date)
{
    vector<string> date_splited = SplitString(date, '/');
    if (date_splited.size() != 3) return false;

    int day, month, year;

    try
    {
        day = stoi(date_splited[0]);
        month = stoi(date_splited[1]);
        year = stoi(date_splited[2]);
    }
    catch (const std::exception&)
    {
        return false;
    }

    if (day <= 0 || month <= 0 || year <= 0) return false;

    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;

    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        if (day > 31)
            return false;
        break;
    case 4: case 6: case 9: case 11:
        if (day > 30)
            return false;
        break;
    case 2:
        if (isLeapYear && day > 29)
            return false;
        if (!isLeapYear && day > 28)
            return false;
        break;
    default:
        return false;
    }
    return true;

}

void Main::DataInputInvalid()
{
    fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::red), "Dữ liệu đầu vào không hợp lệ");
    fmt::println("");
    Main::PauseAndBack();
}

void Main::PauseAndBack()
{
    fmt::println("Nhấn phím Enter để quay lại...");
    cin.get();
}

void Main::PauseAndExit()
{
	fmt::println("Nhấn phím Enter để thoát...");
	cin.get();
    exit(1);
}

/// <summary>
/// Hiển thị giao diện chính
/// </summary>
void Main::Interface()
{
    Main::ClearScreen();

    fmt::println("                            _   _ _       __               ___ ______ ");
    fmt::println("                           | \\ | | |     /_/              / _ \\____  |");
    fmt::println("                           |  \\| | |__   ___  _ __ ___   | | | |  / / ");
    fmt::println("                           | . ` | '_ \\ / _ \\| '_ ` _ \\  | | | | / /  ");
    fmt::println("                           | |\\  | | | | (_) | | | | | | | |_| |/ /   ");
    fmt::println("                           |_| \\_|_| |_|\\___/|_| |_| |_|  \\___//_/    ");
    
    fmt::println("                _______________________________________________________________\n");

    fmt::println("                        [1] Khách hàng\n");

    fmt::println("                        [2] Khoản vay\n");
    
    fmt::println("                        [3] Thoát");

    fmt::println("                _______________________________________________________________\n");

    fmt::print("Nhập lựa chọn của bạn: ");

    string userInput;
    getline(cin, userInput);

    if (userInput == "1")
    {
        CustomersListCSV::Interface();
    }
    else if (userInput == "2")
    {
        LoansListCSV::Interface();
    }
    else if (userInput == "3")
    {

    }
    else
    {
        fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::red), "Lựa chọn không hợp lệ!");
        fmt::println("");
        Main::PauseAndBack();
        Main::Interface();
    }
}

/// <summary>
/// Thứ tự chạy hàm main:
/// Lấy đường dẫn của phần mềm
/// Load 2 file CSV chứa dữ liệu khách hàng và khoản vay
/// Tính toán lãi của khoản vay
/// Khởi tạo console và hiện giao diện chính
/// </summary>
/// <returns></returns>
int main()
{
    Main::GetPath();
    CustomersListCSV::Load();
    LoansListCSV::Load();
    LoansListCSV::CalculateInterestAllRow();

    Main::InitializeConsole();
    Main::Interface();
}


