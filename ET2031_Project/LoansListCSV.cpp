#include "LoansListCSV.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <random>
#include <cmath>
#include "fmt/base.h"
#include "rapidcsv.h"
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

#include "Main.h"
#include "CustomersListCSV.h"

using namespace std;


rapidcsv::Document LoansListCSV::CSVFile;

vector<string> LoansListCSV::vLoanIDs;
vector<string> LoansListCSV::vCustomerIDs;
vector<long long> LoansListCSV::vLoanAmount;
vector<string> LoansListCSV::vDate;
vector<int> LoansListCSV::vLoanTerm;
vector<double> LoansListCSV::vInterestRate;
vector<long long> LoansListCSV::vTotalAccuredInterest;
vector<long long> LoansListCSV::vTotalAmountPaid;
vector<long long> LoansListCSV::vTotalOutstandingBalance;
vector<string> LoansListCSV::vNotes;
vector<string> LoansListCSV::vLastCalDate;

int LoansListCSV::loansCount;

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

bool LoansListCSV::IsLoanIDExist(string ID)
{
	for (int i = 0; i < loansCount; i++)
	{
		if (vLoanIDs[i] == ID)
			return true;
	}
	return false;
}

void LoansListCSV::Load()
{
	CSVFile = rapidcsv::Document(Main::DirectoryPath + "\\loans_list.csv", rapidcsv::LabelParams(0, -1));

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

	loansCount = vLoanIDs.size();
}



void LoansListCSV::Save()
{
	for (int i = 10; i >= 0; i--)
	{
		CSVFile.RemoveColumn(i);
	}
	CSVFile.InsertColumn(0, vLoanIDs, "Mã khoản vay");
	CSVFile.InsertColumn(1, vCustomerIDs, "Số CCCD khách hàng");
	CSVFile.InsertColumn(2, vLoanAmount, "Số tiền vay");
	CSVFile.InsertColumn(3, vDate, "Ngày vay");
	CSVFile.InsertColumn(4, vLoanTerm, "Thời hạn vay");
	CSVFile.InsertColumn(5, vInterestRate, "Lãi suất");
	CSVFile.InsertColumn(6, vTotalAccuredInterest, "Tổng lãi phát sinh");
	CSVFile.InsertColumn(7, vTotalAmountPaid, "Tổng tiền đã trả");
	CSVFile.InsertColumn(8, vTotalOutstandingBalance, "Tổng dư nợ còn lại");
	CSVFile.InsertColumn(9, vNotes, "Ghi chú");
	CSVFile.InsertColumn(10, vLastCalDate, "Ngày tính lãi gần nhất");

	CSVFile.Save();

	LoansListCSV::Load();
}

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

void LoansListCSV::CalculateInterestAllRow()
{
	for (int i = 0; i < loansCount; i++)
	{
		int monthDifferenceLastCal = LoansListCSV::CalculateMonthDifference(vLastCalDate[i], LoansListCSV::GetCurrentDate());
		int monthDifferenceDateAdded = LoansListCSV::CalculateMonthDifference(vDate[i], LoansListCSV::GetCurrentDate());
		if (monthDifferenceDateAdded >= vLoanTerm[i])
		{
			//int monthDiffDateAddedLastCal = LoansListCSV::CalculateMonthDifference(vDate[i], vLastCalDate[i]);
			monthDifferenceLastCal = vLoanTerm[i] - LoansListCSV::CalculateMonthDifference(vDate[i], vLastCalDate[i]);
		}
		if (monthDifferenceLastCal < 1) continue;

		boost::multiprecision::cpp_dec_float_50 rate(vInterestRate[i] / 1200);
		boost::multiprecision::cpp_dec_float_50 currentMoney(vTotalOutstandingBalance[i]);
		boost::multiprecision::cpp_dec_float_50 futureMoney = currentMoney * pow((1 + rate), monthDifferenceLastCal);

		long long ll_futureMoney = futureMoney.convert_to<long long>();
		vTotalAccuredInterest[i] += ll_futureMoney - vTotalOutstandingBalance[i];
		vTotalOutstandingBalance[i] = ll_futureMoney;
		vLastCalDate[i] = LoansListCSV::GetCurrentDate();
	}
	
	LoansListCSV::Save();
}

void LoansListCSV::RemoveCustomerLoan(string CCCD)
{
	bool loanAvail = false;
	for (int i = 0; i < loansCount; i++)
	{
		if (vCustomerIDs[i] == CCCD)
		{
			CSVFile.RemoveRow(i);
			loanAvail = true;
		}
	}
	if (loanAvail)
	{
		CSVFile.Save();
		LoansListCSV::Load();
	}
}

void LoansListCSV::EditLoan(int index)
{
	Main::ClearScreen();
	fmt::println("Mã khoản vay: {0}", vLoanIDs[index]);
	fmt::println("Số CCCD/CMND của khách hàng: {0}", vCustomerIDs[index]);
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
			fmt::println("Cập nhật thời hạn vay thành công");
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
			vTotalAmountPaid[index] += stoll(LoansListCSV::InputMoney());
			vTotalOutstandingBalance[index] = vLoanAmount[index] + vTotalAccuredInterest[index] - vTotalAmountPaid[index];
			vLastCalDate[index] = LoansListCSV::GetCurrentDate();
			fmt::println("Thêm số tiền đã trả thành công");
			fmt::println("Tổng tiền đã trả hiện tại: {0} đồng", vTotalAmountPaid[index]);
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
		fmt::println("Cập nhật ghi chú thành công");
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

		fmt::println("Xoá khoản vay thành công");
	}
	Main::PauseAndBack();
	LoansListCSV::Interface();
}

void LoansListCSV::AddLoan()
{
	Main::ClearScreen();

	fmt::print("Nhập số CCCD/CMND của khách hàng: ");
	string CCCD = Main::UnicodeInput();

	LoansListCSV::AddLoan(CCCD);
}

void LoansListCSV::AddLoan(string CCCD)
{
	if (CustomersListCSV::IsIDNumberAvailable(CCCD))
	{
		vCustomerIDs.push_back(CCCD);
	}
	else
	{
		fmt::println("Số CCCD/CMND không tồn tại, hãy thêm khách hàng trước khi thêm tài sản");
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
	LoansListCSV::Save();

	fmt::println("Khoản vay đã được tạo thành công với mã khoản vay: {0}\n", ID);

	Main::PauseAndBack();
	LoansListCSV::Interface();
}

void LoansListCSV::FindLoanByCCCD()
{
	Main::ClearScreen();
	fmt::print("Nhập số CCCD/CMND của khách hàng: ");
	string CCCD = Main::UnicodeInput();
	LoansListCSV::FindLoanByCCCD(CCCD);
}

void LoansListCSV::FindLoanByCCCD(string CCCD)
{
	string customerName;
	if (!CustomersListCSV::IsIDNumberAvailable(CCCD, customerName))
	{
		fmt::println("Không tìm thấy khách hàng với số CCCD/CMND trên");
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
	if (!isLoanAvail) fmt::println("Không tìm thấy khoản vay nào");
	Main::PauseAndBack();
	LoansListCSV::Interface();
}

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
			fmt::println("Số CCCD/CMND của khách hàng: {0}", vCustomerIDs[i]);
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
		fmt::println("Không tìm thấy khoản vay nào");
		Main::PauseAndBack();
		LoansListCSV::Interface();
		return;
	}

	fmt::println("\nCác thao tác với khoản vay");
	fmt::println("[1] Sửa khoản vay");
	fmt::println("[2] Xoá khoản vay");
	fmt::println("[3] Quay lại màn hình khoản vay");
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

	Main::PauseAndBack();
	LoansListCSV::Interface();
}

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
			fmt::println("Số CCCD/CMND của khách hàng: {0}", vCustomerIDs[i]);
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
		fmt::println("Không tìm thấy khoản vay nào hết hạn");
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
		for (int i = 0; i < indexExpried.size(); i++)
		{
			CSVFile.RemoveRow(indexExpried[i]);
		}
		fmt::println("Đã xoá thành công");
	}

	Main::PauseAndBack();
	LoansListCSV::Interface();
}

void LoansListCSV::Interface()
{
	Main::ClearScreen();
	fmt::println("[1] Thêm khoản vay");
	fmt::println("[2] Tìm kiếm khoản vay theo số CCCD/CMMD của khách hàng");
	fmt::println("[3] Tìm kiếm khoản vay theo mã khoản vay và thao tác trên khoản vay đó");
	fmt::println("[4] Liệt kê các khoản vay hết hạn");
	fmt::println("[5] Quay lại màn hình chính");

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
		Main::Interface();
	}
	else
	{
		fmt::println("Lựa chọn không hợp lệ!");
		Main::PauseAndBack();
		LoansListCSV::Interface();
	}
}
