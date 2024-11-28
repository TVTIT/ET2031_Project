#pragma once
#include "rapidcsv.h"

using namespace std;

class LoansListCSV
{
public:
	static void Load();
	static void Interface();
	static void CalculateInterestAllRow();

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

	static void AddLoan();
	static void FindLoanByCCCD();
	static void FindLoanByID();
};

