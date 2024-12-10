#pragma once
#include "rapidcsv.h"

using namespace std;

class LoansListCSV
{
public:
	static void Load();
	static void CreateNewFile();
	static void Interface();
	static void CalculateInterestAllRow();
	static void RemoveCustomerLoan(string CCCD);
	static void AddLoan(string CCCD);
	static void FindLoanByCCCD(string CCCD);
	static void EditLoanCCCD(string oldCCCD, string newCCCD);

private:
	static rapidcsv::Document CSVFile;

	static vector<string> vLoanIDs;
	static vector<string> vCustomerIDs;
	static vector<long long> vLoanAmount;
	static vector<string> vDate;
	static vector<int> vLoanTerm;
	static vector<double> vInterestRate;
	static vector<long long> vTotalAccuredInterest;
	static vector<long long> vTotalAmountPaid;
	static vector<long long> vTotalOutstandingBalance;
	static vector<string> vNotes;
	static vector<string> vLastCalDate;
	static vector<string> vLoanHistory;

	static int loansCount;

	static string GenerateID();
	static bool IsLoanIDExist(string ID);
	static void Save();

	static string GetCurrentDate();
	static int CalculateMonthDifference(string date1, string date2);

	static string PreviewMoney(long long input);
	static string InputMoney();

	static void EditLoan(int index);
	static void RemoveLoan(int index);
	static void ShowLoanHistory(int index);

	static void AddLoan();
	static void FindLoanByCCCD();
	static void FindLoanByID();
	static void ShowLoansExpired();
};

