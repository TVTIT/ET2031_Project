#include "LoansListCSV.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <random>
#include <cmath>
#include "fmt/base.h"
#include "fmt/color.h"
#include "fmt/format.h"
#include "rapidcsv.h"
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

#include "Main.h"
#include "CustomersListCSV.h"

#define LOANS_LIST_FILE Main::DirectoryPath + "\\loans_list.csv"

using namespace std;


rapidcsv::Document LoansListCSV::CSVFile;

//Mã khoản vay
vector<string> LoansListCSV::vLoanIDs;
//CCCD khách hàng
vector<string> LoansListCSV::vCustomerIDs;
//Số tiền cho vay
vector<long long> LoansListCSV::vLoanAmount;
//Ngày vay
vector<string> LoansListCSV::vDate;
//Hạn vay
vector<int> LoansListCSV::vLoanTerm;
//Lãi suất
vector<double> LoansListCSV::vInterestRate;
//Tổng lãi phát sinh
vector<long long> LoansListCSV::vTotalAccuredInterest;
//Tổng tiền đã trả
vector<long long> LoansListCSV::vTotalAmountPaid;
//Tổng dư nợ còn lại
vector<long long> LoansListCSV::vTotalOutstandingBalance;
//Ghi chú
vector<string> LoansListCSV::vNotes;
//Ngày tính lãi gần nhất
vector<string> LoansListCSV::vLastCalDate;
//Lịch sử nộp tiền
vector<string> LoansListCSV::vLoanHistory;

//Số lượng khoản vay hiện tại
int LoansListCSV::loansCount = 0;

/// <summary>
/// Tạo 1 ID ngẫu nhiên cho khoản vay với 6 ký tự bao gồm chữ viết hoa và số
/// </summary>
/// <returns></returns>
string LoansListCSV::GenerateID()
{
	const string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string randomID;

	// Thiết bị sinh số ngẫu nhiên
	random_device rd;
	mt19937 generator(rd());
	uniform_int_distribution<> distribution(0, characters.size() - 1);

	for (int i = 0; i < 6; i++) 
	{
		randomID += characters[distribution(generator)];
	}

	return randomID;
}

/// <summary>
/// Kiểm tra mã khoản vay đã tồn tại hay không
/// </summary>
/// <param name="ID">ID khoản vay cần kiểm tra</param>
/// <returns>Khoản vay tồn tại hay không</returns>
bool LoansListCSV::IsLoanIDExist(string ID)
{
	for (int i = 0; i < loansCount; i++)
	{
		if (vLoanIDs[i] == ID)
			return true;
	}
	return false;
}

/// <summary>
/// Load file CSV chứa dữ liệu khoản vay và lưu vào các vector
/// </summary>
void LoansListCSV::Load()
{
	ifstream file;
	file.open(LOANS_LIST_FILE);
	//Nếu file CSV không tồn tại, tạo file CSV mới
	if (!file)
	{
		LoansListCSV::CreateNewFile();
		CSVFile = rapidcsv::Document(LOANS_LIST_FILE, rapidcsv::LabelParams(0, -1));
		return;
	}
	file.close();

	CSVFile = rapidcsv::Document(LOANS_LIST_FILE, rapidcsv::LabelParams(0, -1));

	vLoanIDs = CSVFile.GetColumn<string>(0);
	vCustomerIDs = CSVFile.GetColumn<string>(1);
	vLoanAmount = CSVFile.GetColumn<long long>(2);
	vDate = CSVFile.GetColumn<string>(3);
	vLoanTerm = CSVFile.GetColumn<int>(4);
	vInterestRate = CSVFile.GetColumn<double>(5);
	vTotalAccuredInterest = CSVFile.GetColumn<long long>(6);
	vTotalAmountPaid = CSVFile.GetColumn<long long>(7);
	vTotalOutstandingBalance = CSVFile.GetColumn<long long>(8);
	vNotes = CSVFile.GetColumn<string>(9);
	vLastCalDate = CSVFile.GetColumn<string>(10);
	vLoanHistory = CSVFile.GetColumn<string>(11);

	loansCount = vLoanIDs.size();
}

/// <summary>
/// Tạo file CSV mới
/// </summary>
void LoansListCSV::CreateNewFile()
{
	rapidcsv::Document doc(string(), rapidcsv::LabelParams(-1, -1)); //không nhận cột, ô nào làm label
	vector<string> v = { "Mã khoản vay","Số CCCD khách hàng","Số tiền vay","Ngày vay","Thời hạn vay","Lãi suất",
		"Tổng lãi phát sinh","Tổng tiền đã trả","Tổng dư nợ còn lại","Ghi chú","Ngày tính lãi gần nhất", "Lịch sử nộp tiền"};
	doc.InsertRow<string>(0, v);

	doc.Save(LOANS_LIST_FILE);
}

void LoansListCSV::Save()
{
	rapidcsv::Document doc = rapidcsv::Document(string(), rapidcsv::LabelParams(0, -1));
	doc.InsertColumn(0, vLoanIDs, "Mã khoản vay");
	doc.InsertColumn(1, vCustomerIDs, "Số CCCD khách hàng");
	doc.InsertColumn(2, vLoanAmount, "Số tiền vay");
	doc.InsertColumn(3, vDate, "Ngày vay");
	doc.InsertColumn(4, vLoanTerm, "Thời hạn vay");
	doc.InsertColumn(5, vInterestRate, "Lãi suất");
	doc.InsertColumn(6, vTotalAccuredInterest, "Tổng lãi phát sinh");
	doc.InsertColumn(7, vTotalAmountPaid, "Tổng tiền đã trả");
	doc.InsertColumn(8, vTotalOutstandingBalance, "Tổng dư nợ còn lại");
	doc.InsertColumn(9, vNotes, "Ghi chú");
	doc.InsertColumn(10, vLastCalDate, "Ngày tính lãi gần nhất");
	doc.InsertColumn(11, vLoanHistory, "Lịch sử nộp tiền");

	doc.Save(LOANS_LIST_FILE);

	LoansListCSV::Load();
}

/// <summary>
/// Lấy ngày tháng năm hiện tại trên máy
/// </summary>
/// <returns>dd/MM/yyyy</returns>
string LoansListCSV::GetCurrentDate()
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

	// Chuyển đổi sang cấu trúc tm
	std::tm local_time;
	localtime_s(&local_time, &now_time_t);

	std::ostringstream oss;
	oss << std::put_time(&local_time, "%d/%m/%Y");

	return oss.str();
}

/// <summary>
/// Tính toán khoảng cách tháng giữa 2 ngày
/// </summary>
/// <param name="date1"></param>
/// <param name="date2"></param>
/// <returns></returns>
int LoansListCSV::CalculateMonthDifference(string date1, string date2)
{
	tm tm1 = {}, tm2 = {};

	// Chuyển chuỗi ngày sang std::tm với định dạng dd/mm/yyyy
	istringstream ss1(date1);
	istringstream ss2(date2);

	ss1 >> get_time(&tm1, "%d/%m/%Y");
	ss2 >> get_time(&tm2, "%d/%m/%Y");

	// Tính tháng và năm
	int year1 = tm1.tm_year + 1900;
	int month1 = tm1.tm_mon + 1;
	int day1 = tm1.tm_mday;

	int year2 = tm2.tm_year + 1900;
	int month2 = tm2.tm_mon + 1;
	int day2 = tm2.tm_mday;

	// Tính tổng số tháng
	int totalMonths = (year2 - year1) * 12 + (month2 - month1);

	// Tính phần tháng dựa trên ngày
	int daysInMonth1 = 31;
	switch (month1) {
	case 4: case 6: case 9: case 11:
		daysInMonth1 = 30;
		break;
	case 2:
		daysInMonth1 = (year1 % 4 == 0 && (year1 % 100 != 0 || year1 % 400 == 0)) ? 29 : 28;
		break;
	}

	int dayFraction = static_cast<int>(day2 - day1) / daysInMonth1;
	//int exactMonths = totalMonths + dayFraction;
	return totalMonths + dayFraction;
}

/// <summary>
/// Hiển thị số tiền dưới dạng dễ đọc
/// Ví dụ: 100.000 thay vì 100000
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
string LoansListCSV::PreviewMoney(long long input)
{
	string str_input = to_string(input);
	if (str_input.size() < 5) return str_input;

	string output = "";
	int i = str_input.size() - 1;

	do
	{
		output.insert(0, "." + str_input.substr(i - 2, 3));
		i -= 3;
	} while (i >= 3);
	output.insert(0, str_input.substr(0, i + 1));
	return output;
}

/// <summary>
/// Hàm để nhập số tiền
/// Có thể nhập dạng 1.000.000 hoặc 10.00.000 hoặc 1000000
/// Hàm sẽ tự động loại bỏ dấu chấm
/// </summary>
/// <returns>Input người dùng đã bỏ dấu chấm</returns>
string LoansListCSV::InputMoney()
{
	string userInput = Main::UnicodeInput();
	for (int i = 0; i < userInput.size(); i++)
	{
		if (userInput[i] == '.')
			userInput.erase(i, 1);
	}
	return userInput;
}

/// <summary>
/// Tính toán lãi phát sinh của từng khoản vay
/// </summary>
void LoansListCSV::CalculateInterestAllRow()
{
	//Nếu số khoản vay < 1 thì không tính nữa
	if (loansCount < 1) return;
	for (int i = 0; i < loansCount; i++)
	{
		//Nếu khoản vay đã trả hết thì không tính khoản vay này nữa
		if (vTotalOutstandingBalance[i] <= 0) continue;
		int monthDifferenceLastCal = LoansListCSV::CalculateMonthDifference(vLastCalDate[i], LoansListCSV::GetCurrentDate());
		//Nếu khoảng cách tháng giữa ngày tính gần nhất và ngày hiện tại < 1 thì không tính khoản vay này nữa
		if (monthDifferenceLastCal < 1) continue;

		//Sử dụng cpp_dec_float_50 trong thư viện boost/multiprecision để tính toán chính xác
		boost::multiprecision::cpp_dec_float_50 rate(vInterestRate[i]);
		boost::multiprecision::cpp_dec_float_50 currentMoney(vTotalOutstandingBalance[i]);
		boost::multiprecision::cpp_dec_float_50 futureMoney = currentMoney * pow((1 + rate / 1200), monthDifferenceLastCal);

		long long ll_futureMoney = boost::multiprecision::round(futureMoney).convert_to<long long>();
		vTotalAccuredInterest[i] += ll_futureMoney - vTotalOutstandingBalance[i];
		vTotalOutstandingBalance[i] = ll_futureMoney;
		vLastCalDate[i] = LoansListCSV::GetCurrentDate();
	}
	
	LoansListCSV::Save();
}

/// <summary>
/// Xoá tất cả khoản vay của khách hàng
/// </summary>
/// <param name="CCCD"></param>
void LoansListCSV::RemoveCustomerLoan(string CCCD)
{
	bool loanAvail = false;
	int countDeleted = 0;
	for (int i = 0; i < loansCount; i++)
	{
		if (vCustomerIDs[i] == CCCD)
		{
			CSVFile.RemoveRow(i - countDeleted);
			countDeleted++; //Mỗi lần xoá thì mất 1 dòng trong file CSV -> lệch với index của vector
			loanAvail = true;
		}
	}
	if (loanAvail)
	{
		CSVFile.Save();
		LoansListCSV::Load();
	}
}

/// <summary>
/// Chỉnh sửa khoản vay
/// </summary>
/// <param name="index">index của vector chứa dữ liệu khoản vay</param>
void LoansListCSV::EditLoan(int index)
{
	Main::ClearScreen();
	fmt::println("Mã khoản vay: {0}", vLoanIDs[index]);
	fmt::println("Số CCCD của khách hàng: {0}", vCustomerIDs[index]);
	fmt::println("Số tiền vay: {0} đồng", PreviewMoney(vLoanAmount[index]));
	fmt::println("Ngày vay: {0}", vDate[index]);
	fmt::println("Thời hạn vay: {0} tháng", vLoanTerm[index]);
	fmt::println("Lãi suất: {0}%", vInterestRate[index]);
	fmt::println("Tổng lãi phát sinh: {0} đồng", PreviewMoney(vTotalAccuredInterest[index]));
	fmt::println("Tổng tiền đã trả: {0} đồng", PreviewMoney(vTotalAmountPaid[index]));
	fmt::println("Tổng tiền dư nợ còn lại: {0} đồng\n", PreviewMoney(vTotalOutstandingBalance[index]));

	fmt::println("[1] Thời hạn vay");
	fmt::println("[2] Thêm số tiền đã trả");
	fmt::println("[3] Ghi chú");
	fmt::println("[4] Quay lại màn hình khoản vay");
	fmt::print("Nhập phần bạn muốn chỉnh sửa: ");

	string userInput = Main::UnicodeInput();
	if (userInput == "1")
	{
		try
		{
			fmt::print("Nhập thời hạn vay theo tháng: ");
			vLoanTerm[index] = stoi(Main::UnicodeInput());
			fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Cập nhật thời hạn vay thành công");
			fmt::println("");
			LoansListCSV::Save();
			Main::PauseAndBack();
			LoansListCSV::Interface();
			return;
		}
		catch (const std::exception&)
		{
			LoansListCSV::Load();
			Main::DataInputInvalid();
			LoansListCSV::EditLoan(index);
			return;
		}
	}
	else if (userInput == "2")
	{
		try
		{
			fmt::print("Nhập số tiền muốn thêm: ");
			string moneyAdded = LoansListCSV::InputMoney();
			vTotalAmountPaid[index] += stoll(moneyAdded);
			vTotalOutstandingBalance[index] = vLoanAmount[index] + vTotalAccuredInterest[index] - vTotalAmountPaid[index];
			vLastCalDate[index] = LoansListCSV::GetCurrentDate();
			vLoanHistory[index].append(fmt::format("{0};{1}|", vLastCalDate[index], moneyAdded));
			fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Thêm số tiền đã trả thành công");
			fmt::println("");
			fmt::println("Tổng tiền đã trả hiện tại: {0} đồng", LoansListCSV::PreviewMoney(vTotalAmountPaid[index]));
			LoansListCSV::Save();
			Main::PauseAndBack();
			LoansListCSV::Interface();
			return;
		}
		catch (const std::exception&)
		{
			LoansListCSV::Load();
			Main::DataInputInvalid();
			LoansListCSV::EditLoan(index);
			return;
		}
		
	}
	else if (userInput == "3")
	{
		fmt::print("Nhập ghi chú: ");
		vNotes[index] = Main::UnicodeInput();
		fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Cập nhật ghi chú thành công");
		fmt::println("");
		LoansListCSV::Save();
		Main::PauseAndBack();
		LoansListCSV::Interface();
		return;
	}
	else
	{
		Main::PauseAndBack();
		LoansListCSV::Interface();
		return;
	}
}

/// <summary>
/// Xoá khoản vay
/// </summary>
/// <param name="index">index của vector chứa dữ liệu khoản vay</param>
void LoansListCSV::RemoveLoan(int index)
{
	fmt::println("Bạn có chắc chắn muốn xoá khoản vay trên");
	fmt::print("Nhập Y để xoá, N để huỷ: ");
	string userInput = Main::UnicodeInput();
	
	if (userInput == "Y" || userInput == "y")
	{
		CSVFile.RemoveRow(index);
		CSVFile.Save();
		LoansListCSV::Load();

		fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Xoá khoản vay thành công");
		fmt::println("");
	}
	Main::PauseAndBack();
	LoansListCSV::Interface();
}

/// <summary>
/// Hiển thị lịch sử đóng tiền của khoản vay
/// </summary>
/// <param name="index">index của vector chứa dữ liệu khoản vay</param>
void LoansListCSV::ShowLoanHistory(int index)
{
	fmt::println("Lịch sử nộp tiền của khoản vay trên:\n");

	vector<string> LoanHistory_splited = Main::SplitString(vLoanHistory[index], '|');
	if (LoanHistory_splited.size() < 1)
	{
		fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::gray), "Khoản vay trên chưa có giao dịch nộp tiền nào");
		fmt::println("");
		return;
	}

	for (int i = 0; i < LoanHistory_splited.size(); i++)
	{
		vector<string> vDate_Money = Main::SplitString(LoanHistory_splited[i], ';');
		fmt::println("Ngày: {0}", vDate_Money[0]);
		fmt::println("Số tiền nộp thêm: {0}\n", LoansListCSV::PreviewMoney(stoll(vDate_Money[1])));
	}
	fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::gray), "Hết lịch sử giao dịch");
	fmt::println("");
}

/// <summary>
/// Tính toán số tiền cố định phải trả từng tháng để kịp trả hết nợ đúng hạn
/// </summary>
/// <param name="index">index của vector chứa thông tin khoản vay</param>
void LoansListCSV::CalculateMonthlyAmount(int index)
{
	fmt::println("");
	//Khoản vay đã trả hết nợ
	if (vTotalOutstandingBalance[index] <= 0)
	{
		fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Khoản vay này đã trả hết nợ");
		fmt::println("\n");
		Main::PauseAndBack();
		LoansListCSV::Interface();
		return;
	}

	int monthLeft = vLoanTerm[index] - CalculateMonthDifference(vDate[index], GetCurrentDate());

	//Nếu khoản vay hết hạn
	if (monthLeft < 1)
	{
		fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Khoản vay này đã hết hạn");
		fmt::println("\n");
		fmt::print("Nhập số tháng để trả hết nợ cho khoản vay này: ");
		try
		{
			monthLeft = stoi(Main::UnicodeInput());
			if (monthLeft < 1)
			{
				throw exception();
			}
		}
		catch (const std::exception&)
		{
			Main::DataInputInvalid();
			LoansListCSV::Interface();
			return;
		}
		fmt::println("");
	}

	boost::multiprecision::cpp_dec_float_50 totalOutstandingBalance(vTotalOutstandingBalance[index]);
	boost::multiprecision::cpp_dec_float_50 interestRate(vInterestRate[index]);
	boost::multiprecision::cpp_dec_float_50 moneyMonthly = (totalOutstandingBalance * (interestRate / 1200)) / (1 - pow((1 + interestRate / 1200), -monthLeft));
	long long ll_moneyMonthly = boost::multiprecision::round(moneyMonthly).convert_to<long long>();

	fmt::println("Số tiền vay: {0} đồng", PreviewMoney(vLoanAmount[index]));
	fmt::println("Ngày vay: {0}", vDate[index]);
	fmt::println("Thời hạn vay: {0} tháng", vLoanTerm[index]);
	fmt::println("Phải trả hết nợ trong {0} tháng", monthLeft);
	fmt::println("Tổng lãi phát sinh (hiện tại): {0} đồng", PreviewMoney(vTotalAccuredInterest[index]));
	fmt::println("Tổng dư nợ còn lại: {0} đồng\n", PreviewMoney(vTotalOutstandingBalance[index]));

	fmt::print("Để trả hết số nợ còn lại trong ");
	fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::gray), "{0} tháng", monthLeft);
	fmt::print(", bạn phải trả ");
	fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::gray), "{0} đồng", PreviewMoney(ll_moneyMonthly));
	fmt::println(" mỗi tháng\n");

	fmt::println("Số tiền vay (1): {0} đồng", PreviewMoney(vLoanAmount[index]));
	fmt::println("Tổng số tiền đã trả (hiện tại) (2): {0} đồng", PreviewMoney(vTotalAmountPaid[index]));
	fmt::println("Tổng số tiền sẽ trả sau {0} tháng (3): {1} * {0} = {2} đồng", monthLeft, PreviewMoney(ll_moneyMonthly), PreviewMoney(ll_moneyMonthly * monthLeft));
	fmt::println("Tổng số tiền sẽ trả sau khi hết nợ ((4) = (2) + (3): {0} đồng", PreviewMoney(vTotalAmountPaid[index] + ll_moneyMonthly * monthLeft));
	fmt::println("Tổng lãi phát sinh sau khi trả hết nợ ((5) = (4) - (1): {0} đồng\n", PreviewMoney(vTotalAmountPaid[index] + ll_moneyMonthly * monthLeft - vLoanAmount[index]));

	Main::PauseAndBack();
	LoansListCSV::Interface();
}

/// <summary>
/// Tính số tiền phải trả cố định hàng tháng cho khoản vay mới nhất
/// </summary>
void LoansListCSV::CalculateMonthlyAmount()
{
	int index = loansCount - 1;
	int monthLeft = vLoanTerm[index];

	boost::multiprecision::cpp_dec_float_50 totalOutstandingBalance(vTotalOutstandingBalance[index]);
	boost::multiprecision::cpp_dec_float_50 interestRate(vInterestRate[index]);
	boost::multiprecision::cpp_dec_float_50 moneyMonthly = (totalOutstandingBalance * (interestRate / 1200)) / (1 - pow((1 + interestRate / 1200), -monthLeft));
	long long ll_moneyMonthly = boost::multiprecision::round(moneyMonthly).convert_to<long long>();

	fmt::print("Để trả hết số tiền vay trong ");
	fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::gray), "{0} tháng", monthLeft);
	fmt::print(", bạn phải trả ");
	fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::gray), "{0} đồng", PreviewMoney(ll_moneyMonthly));
	fmt::println(" mỗi tháng\n");

	fmt::println("Số tiền vay (1): {0} đồng", PreviewMoney(vLoanAmount[index]));
	fmt::println("Tổng số tiền sẽ trả sau khi hết nợ (2): {1} * {0} = {2} đồng", monthLeft, PreviewMoney(ll_moneyMonthly), PreviewMoney(ll_moneyMonthly * monthLeft));
	fmt::println("Tổng lãi phát sinh sau khi trả hết nợ ((3) = (2) - (1): {0} đồng\n", PreviewMoney(vTotalAmountPaid[index] + ll_moneyMonthly * monthLeft - vLoanAmount[index]));
}

/// <summary>
/// Tính ngày đóng tiền gần nhất của khoản vay
/// </summary>
/// <param name="index">index của vector chứa thông tin khoản vay</param>
/// <returns></returns>
string LoansListCSV::CalculateMostRecentPay(int index)
{
	if (vLoanHistory[index].empty())
	{
		return vDate[index];
	}
	vector<string> history_splited = Main::SplitString(vLoanHistory[index], '|');
	vector<string> v_DateMoney = Main::SplitString(history_splited[history_splited.size() - 1], ';');
	return v_DateMoney[0];
}

/// <summary>
/// Thêm khoản vay (gọi từ LoansListCSV::Interface())
/// </summary>
void LoansListCSV::AddLoan()
{
	Main::ClearScreen();

	fmt::print("Nhập số CCCD của khách hàng: ");
	string CCCD = Main::UnicodeInput();

	LoansListCSV::AddLoan(CCCD);
}

/// <summary>
/// Thêm khoản vay (gọi từ class CustomerListCSV)
/// </summary>
/// <param name="CCCD"></param>
void LoansListCSV::AddLoan(string CCCD)
{
	string customerName = string();
	if (CustomersListCSV::IsIDNumberAvailable(CCCD, customerName))
	{
		vCustomerIDs.push_back(CCCD);
		fmt::println("Đang thêm khoản vay cho khách hàng {0}\n", customerName);
	}
	else
	{
		fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Số CCCD không tồn tại, hãy thêm khách hàng trước khi thêm khoản vay");
		fmt::println("");
		Main::PauseAndBack();
		LoansListCSV::Interface();
	}

	fmt::print("Nhập số tiền vay: ");

	try
	{
		vLoanAmount.push_back(stoll(LoansListCSV::InputMoney()));
	}
	catch (const std::exception&)
	{
		LoansListCSV::Load();
		Main::DataInputInvalid();
		LoansListCSV::Interface();
	}

	fmt::print("Nhập ngày vay: ");
	string date = Main::UnicodeInput();
	if (Main::ValidateDate(date))
	{
		vDate.push_back(date);
	}
	else
	{
		LoansListCSV::Load();
		Main::DataInputInvalid();
		LoansListCSV::Interface();
	}

	fmt::print("Nhập thời hạn vay (tính theo tháng): ");

	try
	{
		vLoanTerm.push_back(stoi(Main::UnicodeInput()));
	}
	catch (const std::exception&)
	{
		LoansListCSV::Load();
		Main::DataInputInvalid();
		LoansListCSV::Interface();
	}

	fmt::print("Nhập lãi suất theo %: ");

	try
	{
		vInterestRate.push_back(stod(Main::UnicodeInput()));
	}
	catch (const std::exception&)
	{
		LoansListCSV::Load();
		Main::DataInputInvalid();
		LoansListCSV::Interface();
	}

	fmt::print("Nhập ghi chú: ");
	vNotes.push_back(Main::UnicodeInput());

	string ID = "";
	do
	{
		ID = LoansListCSV::GenerateID();
	} while (LoansListCSV::IsLoanIDExist(ID));

	vLoanIDs.push_back(ID);
	vTotalAccuredInterest.push_back(0);
	vTotalAmountPaid.push_back(0);
	vTotalOutstandingBalance.push_back(vLoanAmount[loansCount]);
	vLastCalDate.push_back(date);
	vLoanHistory.push_back("");
	LoansListCSV::Save();

	fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Khoản vay đã được tạo thành công với mã khoản vay: {0}", ID);
	fmt::println("\n");

	LoansListCSV::CalculateMonthlyAmount();

	Main::PauseAndBack();
	LoansListCSV::Interface();
}

/// <summary>
/// Tìm khoản vay theo CCCD (gọi từ LoansListCSV::Interface())
/// </summary>
void LoansListCSV::FindLoanByCCCD()
{
	Main::ClearScreen();
	fmt::print("Nhập số CCCD của khách hàng: ");
	string CCCD = Main::UnicodeInput();
	LoansListCSV::FindLoanByCCCD(CCCD);
}

/// <summary>
/// Tìm khoản vay theo CCCD (gọi từ class CustomerListCSV)
/// </summary>
/// <param name="CCCD"></param>
void LoansListCSV::FindLoanByCCCD(string CCCD)
{
	string customerName;
	if (!CustomersListCSV::IsIDNumberAvailable(CCCD, customerName))
	{
		fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Không tìm thấy khách hàng với số CCCD trên");
		fmt::println("");
		Main::PauseAndBack();
		LoansListCSV::Interface();
		return;
	}
	fmt::println("Đang tìm những khoản vay của khách hàng {0}...\n", customerName);
	bool isLoanAvail = false;
	for (int i = 0; i < loansCount; i++)
	{
		if (vCustomerIDs[i] == CCCD)
		{
			isLoanAvail = true;
			fmt::println("Mã khoản vay: {0}", vLoanIDs[i]);
			fmt::println("Số tiền vay: {0} đồng", PreviewMoney(vLoanAmount[i]));
			fmt::println("Ngày vay: {0}", vDate[i]);
			fmt::println("Thời hạn vay: {0} tháng", vLoanTerm[i]);
			fmt::println("Lãi suất: {0}%", vInterestRate[i]);
			fmt::println("Tổng lãi phát sinh: {0} đồng", PreviewMoney(vTotalAccuredInterest[i]));
			fmt::println("Tổng tiền đã trả: {0} đồng", PreviewMoney(vTotalAmountPaid[i]));
			fmt::println("Tổng tiền dư nợ còn lại: {0} đồng\n", PreviewMoney(vTotalOutstandingBalance[i]));
		}
	}
	if (!isLoanAvail) fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::red), "Không tìm thấy khoản vay nào"); fmt::println("");
	Main::PauseAndBack();
	LoansListCSV::Interface();
}

/// <summary>
/// Thay đổi số CCCD trên tất cả khoản vay hiện tại của khách hàng
/// </summary>
/// <param name="oldCCCD">CCCD cũ</param>
/// <param name="newCCCD">CCCD mới</param>
void LoansListCSV::EditLoanCCCD(string oldCCCD, string newCCCD)
{
	for (int i = 0; i < loansCount; i++)
	{
		if (vCustomerIDs[i] == oldCCCD)
			vCustomerIDs[i] = newCCCD;
	}
	LoansListCSV::Save();
}

/// <summary>
/// Tìm khoản vay theo mã khoản vay
/// </summary>
void LoansListCSV::FindLoanByID()
{
	Main::ClearScreen();
	fmt::print("Nhập mã khoản vay: ");
	string loanID = Main::UnicodeInput();
	fmt::println("");

	bool isLoanAvail = false;
	int index = 0;
	for (int i = 0; i < loansCount; i++)
	{
		if (loanID == vLoanIDs[i])
		{
			isLoanAvail = true;
			index = i;
			fmt::println("Mã khoản vay: {0}", vLoanIDs[i]);
			fmt::println("Số CCCD của khách hàng: {0}", vCustomerIDs[i]);
			fmt::println("Số tiền vay: {0} đồng", PreviewMoney(vLoanAmount[i]));
			fmt::println("Ngày vay: {0}", vDate[i]);
			fmt::println("Thời hạn vay: {0} tháng", vLoanTerm[i]);
			fmt::println("Lãi suất: {0}%", vInterestRate[i]);
			fmt::println("Tổng lãi phát sinh: {0} đồng", PreviewMoney(vTotalAccuredInterest[i]));
			fmt::println("Tổng tiền đã trả: {0} đồng", PreviewMoney(vTotalAmountPaid[i]));
			fmt::println("Tổng tiền dư nợ còn lại: {0} đồng\n", PreviewMoney(vTotalOutstandingBalance[i]));
			break;
		}
	}
	if (!isLoanAvail)
	{
		fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Không tìm thấy khoản vay nào");
		fmt::println("");
		Main::PauseAndBack();
		LoansListCSV::Interface();
		return;
	}

	fmt::println("\nCác thao tác với khoản vay");
	fmt::println("[1] Sửa khoản vay");
	fmt::println("[2] Xoá khoản vay");
	fmt::println("[3] Liệt kê lịch sử nộp tiền của khoản vay");
	fmt::println("[4] Tính toán số tiền cố định phải trả hàng tháng");
	fmt::println("[5] Quay lại màn hình khoản vay");
	fmt::print("Nhập lựa chọn của bạn: ");

	string userInput = Main::UnicodeInput();
	if (userInput == "1") 
	{
		LoansListCSV::EditLoan(index);
	}
	else if (userInput == "2")
	{
		LoansListCSV::RemoveLoan(index);
	}
	else if (userInput == "3")
	{
		LoansListCSV::ShowLoanHistory(index);
	}
	else if (userInput == "4")
	{
		LoansListCSV::CalculateMonthlyAmount(index);
	}

	Main::PauseAndBack();
	LoansListCSV::Interface();
}

/// <summary>
/// Liệt kê các khoản vay hết hạn
/// </summary>
void LoansListCSV::ShowLoansExpired()
{
	Main::ClearScreen();
	fmt::println("Đang tìm kiếm các khoản vay hết hạn...\n");
	
	string currDate = LoansListCSV::GetCurrentDate();
	vector<int> indexExpried = vector<int>();
	for (int i = 0; i < loansCount; i++)
	{
		int monthDifference = LoansListCSV::CalculateMonthDifference(vDate[i], currDate);
		if (monthDifference >= vLoanTerm[i])
		{
			fmt::println("Mã khoản vay: {0}", vLoanIDs[i]);
			fmt::println("Số CCCD của khách hàng: {0}", vCustomerIDs[i]);
			fmt::println("Số tiền vay: {0} đồng", PreviewMoney(vLoanAmount[i]));
			fmt::println("Ngày vay: {0}", vDate[i]);
			fmt::println("Thời hạn vay: {0} tháng", vLoanTerm[i]);
			fmt::println("Lãi suất: {0}%", vInterestRate[i]);
			fmt::println("Tổng lãi phát sinh: {0} đồng", PreviewMoney(vTotalAccuredInterest[i]));
			fmt::println("Tổng tiền đã trả: {0} đồng", PreviewMoney(vTotalAmountPaid[i]));
			fmt::println("Tổng tiền dư nợ còn lại: {0} đồng\n", PreviewMoney(vTotalOutstandingBalance[i]));
			indexExpried.push_back(i);
		}
	}
	if (indexExpried.size() == 0)
	{
		fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Không tìm thấy khoản vay nào hết hạn");
		fmt::println("");
		Main::PauseAndBack();
		LoansListCSV::Interface();
		return;
	}

	fmt::println("Bạn có muốn xoá toàn bộ các khoản vay đã hết hạn không");
	fmt::print("Nhấn Y để xoá, N để huỷ: ");
	string userInput = Main::UnicodeInput();

	if (userInput == "y" || userInput == "Y")
	{
		fmt::println("\nĐang xoá các khoản vay trên...");
		int countDeleted = 0;
		for (int i = 0; i < indexExpried.size(); i++)
		{
			CSVFile.RemoveRow(indexExpried[i] - countDeleted);
			countDeleted++;
		}
		CSVFile.Save();
		LoansListCSV::Load();
		fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Đã xoá thành công");
		fmt::println("");
	}

	Main::PauseAndBack();
	LoansListCSV::Interface();
}

/// <summary>
/// Liệt kê các khoản vay đã trả hết nợ
/// </summary>
void LoansListCSV::ShowLoansPaidOff()
{
	Main::ClearScreen();
	fmt::println("Đang tìm kiếm các khoản đã trả hết nợ...\n");

	string currDate = LoansListCSV::GetCurrentDate();
	vector<int> indexPaidOff = vector<int>();
	for (int i = 0; i < loansCount; i++)
	{
		if (vTotalOutstandingBalance[i] <= 0)
		{
			fmt::println("Mã khoản vay: {0}", vLoanIDs[i]);
			fmt::println("Số CCCD của khách hàng: {0}", vCustomerIDs[i]);
			fmt::println("Số tiền vay: {0} đồng", PreviewMoney(vLoanAmount[i]));
			fmt::println("Ngày vay: {0}", vDate[i]);
			fmt::println("Thời hạn vay: {0} tháng", vLoanTerm[i]);
			fmt::println("Lãi suất: {0}%", vInterestRate[i]);
			fmt::println("Tổng lãi phát sinh: {0} đồng", PreviewMoney(vTotalAccuredInterest[i]));
			fmt::println("Tổng tiền đã trả: {0} đồng", PreviewMoney(vTotalAmountPaid[i]));
			fmt::println("Tổng tiền dư nợ còn lại: {0} đồng\n", PreviewMoney(vTotalOutstandingBalance[i]));
			indexPaidOff.push_back(i);
		}
	}
	if (indexPaidOff.size() == 0)
	{
		fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Không tìm thấy khoản vay nào đã trả hết nợ");
		fmt::println("");
		Main::PauseAndBack();
		LoansListCSV::Interface();
		return;
	}

	fmt::println("Bạn có muốn xoá toàn bộ các khoản vay đã trả hết nợ không");
	fmt::print("Nhấn Y để xoá, N để huỷ: ");
	string userInput = Main::UnicodeInput();

	if (userInput == "y" || userInput == "Y")
	{
		fmt::println("\nĐang xoá các khoản vay trên...");
		int countDeleted = 0;
		for (int i = 0; i < indexPaidOff.size(); i++)
		{
			CSVFile.RemoveRow(indexPaidOff[i] - countDeleted);
			countDeleted++;
		}
		CSVFile.Save();
		LoansListCSV::Load();
		fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::green), "Đã xoá thành công");
		fmt::println("");
	}

	Main::PauseAndBack();
	LoansListCSV::Interface();
}

/// <summary>
/// Liệt kê các khoản vay lâu chưa đóng tiền
/// </summary>
void LoansListCSV::ShowLoansOverDue()
{
	Main::ClearScreen();
	fmt::println("Nhập số tháng ở dưới để liệt kê các khoản vay có lần đóng tiền gần nhất\nđã quá số tháng đó (tính đến ngày hôm nay)");
	fmt::println("Phần mềm sẽ không liệt kê các khoản vay đã trả hết nợ\n");
	fmt::print("Nhập số tháng: ");

	int monthInput = 0;
	try
	{
		monthInput = stoi(Main::UnicodeInput());
		if (monthInput < 0)
		{
			throw exception();
		}
	}
	catch (const std::exception&)
	{
		Main::DataInputInvalid();
		LoansListCSV::Interface();
		return;
	}
	fmt::println("");

	bool isAvail = false;
	for (int i = 0; i < loansCount; i++)
	{
		string dateCal = LoansListCSV::CalculateMostRecentPay(i);
		int monthDifference = CalculateMonthDifference(dateCal, GetCurrentDate());
		if (monthDifference >= monthInput)
		{
			isAvail = true;
			fmt::println("Mã khoản vay: {0}", vLoanIDs[i]);
			fmt::println("Số CCCD của khách hàng: {0}", vCustomerIDs[i]);
			fmt::println("Số tiền vay: {0} đồng", PreviewMoney(vLoanAmount[i]));
			fmt::println("Ngày vay: {0}", vDate[i]);
			fmt::println("Thời hạn vay: {0} tháng", vLoanTerm[i]);
			fmt::println("Lãi suất: {0}%", vInterestRate[i]);
			fmt::println("Tổng lãi phát sinh: {0} đồng", PreviewMoney(vTotalAccuredInterest[i]));
			fmt::println("Tổng tiền đã trả: {0} đồng", PreviewMoney(vTotalAmountPaid[i]));
			fmt::println("Tổng tiền dư nợ còn lại: {0} đồng", PreviewMoney(vTotalOutstandingBalance[i]));
			fmt::println("Ngày nộp tiền gần nhất: {0}", dateCal);
			fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Đã quá hạn: {0} tháng", monthDifference);
			fmt::println("\n");
		}
	}

	if (!isAvail)
	{
		fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Không có khoản vay nào quá {0} tháng chưa nộp tiền", monthInput);
		fmt::println("");
	}

	Main::PauseAndBack();
	LoansListCSV::Interface();
}

/// <summary>
/// Tính toán tổng số tiền cho vay, lãi phát sinh, dự kiến số tiền thu hồi nợ
/// </summary>
void LoansListCSV::CalculateTotalMoney()
{
	Main::ClearScreen();
	fmt::println("Đang tính toán...\n");

	long long tienChoVay = 0, tongLai = 0;

	for (int i = 0; i < loansCount; i++)
	{
		tienChoVay += vLoanAmount[i];
		
		//Tính tổng lãi
		if (vTotalOutstandingBalance[i] > 0)
		{
			int monthDifference = vLoanTerm[i] - LoansListCSV::CalculateMonthDifference(vDate[i], vLastCalDate[i]);
			if (monthDifference > 0)
			{
				boost::multiprecision::cpp_dec_float_50 rate(vInterestRate[i]);
				boost::multiprecision::cpp_dec_float_50 currentMoney(vTotalOutstandingBalance[i]);
				//Tổng lãi phát sinh dự kiến = tổng lãi phát sinh của số tiền còn nợ cho đến hết hạn khoản vay
				boost::multiprecision::cpp_dec_float_50 futureMoney = currentMoney * (pow((1 + rate / 1200), monthDifference));

				//Tổng lãi = tổng lãi phát sinh đã tính toán + Tổng lãi phát sinh dự kiến
				tongLai += vTotalAccuredInterest[i] + boost::multiprecision::round(futureMoney).convert_to<long long>();
			}
		}
	}

	fmt::println("Số khoản vay hiện tại: {0}", loansCount);
	fmt::println("Tổng số tiền cho vay (1): {0} đồng", PreviewMoney(tienChoVay));
	fmt::println("Tổng lãi phát sinh dự kiến (2): {0} đồng", PreviewMoney(tongLai));
	fmt::println("Dự kiến số tiền sau khi thu hồi nợ ((3) = (1) + (2): {0} đồng\n", PreviewMoney(tienChoVay + tongLai));

	fmt::print(fmt::fg(fmt::color::black) | fmt::bg(fmt::color::yellow), "Lưu ý: Dữ liệu trên chỉ là dự kiến, số liệu có thể thay đổi\nphụ thuộc vào cách trả nợ của người vay");
	fmt::println("\n");

	Main::PauseAndBack();
	LoansListCSV::Interface();
}

/// <summary>
/// Giao diện khoản vay
/// </summary>
void LoansListCSV::Interface()
{
	Main::ClearScreen();
	//fmt::println("[1] Thêm khoản vay");
	//fmt::println("[2] Tìm kiếm khoản vay theo số CCCD/CMMD của khách hàng");
	//fmt::println("[3] Tìm kiếm khoản vay theo mã khoản vay và thao tác trên khoản vay đó");
	//fmt::println("[4] Liệt kê các khoản vay hết hạn");
	//fmt::println("[5] Liệt kê các khoản vay đã trả hết nợ");
	//fmt::println("[6] Liệt kê các khoản vay lâu chưa đóng tiền");
	//fmt::println("[7] Tính toán tổng số tiền cho vay, lãi phát sinh, dự kiến số tiền thu hồi nợ,...");
	//fmt::println("[8] Quay lại màn hình chính");


	fmt::println("     _____________________________________________________________________________________\n");

	fmt::println("       [1] Thêm khoản vay\n");
						 
	fmt::println("       [2] Tìm kiếm khoản vay theo số CCCD/CMMD của khách hàng\n");
						 
	fmt::println("       [3] Tìm kiếm khoản vay theo mã khoản vay và thao tác trên khoản vay đó\n");
						 
	fmt::println("       [4] Liệt kê các khoản vay hết hạn\n");
						 
	fmt::println("       [5] Liệt kê các khoản vay đã trả hết nợ\n");
						 
	fmt::println("       [6] Liệt kê các khoản vay lâu chưa đóng tiền\n");
						 
	fmt::println("       [7] Tính toán tổng số tiền cho vay, lãi phát sinh, dự kiến số tiền thu hồi nợ,...\n");
						 
	fmt::println("       [8] Quay lại màn hình chính");

	fmt::println("     _____________________________________________________________________________________\n");

	fmt::print("Nhập lựa chọn của bạn: ");

	string userInput;
	getline(cin, userInput);

	if (userInput == "1")
	{
		LoansListCSV::AddLoan();
	}
	else if (userInput == "2")
	{
		LoansListCSV::FindLoanByCCCD();
	}
	else if (userInput == "3")
	{
		LoansListCSV::FindLoanByID();
	}
	else if (userInput == "4")
	{
		LoansListCSV::ShowLoansExpired();
	}
	else if (userInput == "5")
	{
		LoansListCSV::ShowLoansPaidOff();
	}
	else if (userInput == "6")
	{
		LoansListCSV::ShowLoansOverDue();
	}
	else if (userInput == "7")
	{
		LoansListCSV::CalculateTotalMoney();
	}
	else if (userInput == "8")
	{
		Main::Interface();
	}
	else
	{
		fmt::print(fmt::fg(fmt::color::white) | fmt::bg(fmt::color::red), "Lựa chọn không hợp lệ!");
		fmt::println("");
		Main::PauseAndBack();
		LoansListCSV::Interface();
	}
}
