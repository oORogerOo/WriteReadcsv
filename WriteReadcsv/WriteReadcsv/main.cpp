#include "StdAfx.h"
#include <iostream>
#include <string.h>
#include "cmdlineparser.h"
#include "tinycsv.h"



using namespace std;
ccutils::cmdline::CmdLineParser parser;


bool Exefunction()
{
	int flagCol = -1;
	int TargetCol = -1;

	if (parser.hasKey("s") != 0)//采用类型定位 -s str -s r/c/rc 
	{
		if (parser.getValStr("p", 0) == "" || parser.getValStr("s", 0) == "" || parser.getValStr("s", 1) == "")
		{
			cout << "Param error,Please check." << endl;
			ProcessLog(_T("Param error,Please check."), LOGDEBUG);
			return FALSE;
		}

		//加载csv文件
		TinyCSV csv(parser.getValStr("p", 0));

		//判断列表是否为空
		if (!csv.isValid())
		{
			//csv为空
			ProcessLog(_T("csv is empty"), LOGDEBUG);
			return FALSE;
		}

		//找到在那一列
		int i = 0;
		int j = 0;
		bool breakflag = FALSE;
		for (i = 0; i < csv.rowCount(); ++i)
		{
			for (j = 0; j < csv.colCount(); ++j)
			{
				if (csv[i][j] == parser.getValStr("s", 0))
				{
					breakflag=TRUE;
					break;
				}
			}
			if (breakflag == TRUE)
				break;
		}

		if (parser.getValStr("s", 1) == "r")
		{
			cout << i << endl;
		}
		else if (parser.getValStr("s", 1) == "c")
		{
			cout << j << endl;
		}
		else if (parser.getValStr("s", 1) == "rc")
		{
			cout << i * 10 + j << endl;
		}
		return TRUE;


	}
	else if (parser.hasKey("t") == 0)//采用行列定位方式
	{
		//命令完整性检查
		if (parser.hasKey("p") == 0 || (parser.hasKey("w") == 0 && parser.hasKey("r") == 0) || parser.hasKey("row") == 0 || parser.hasKey("col") == 0)
		{
			cout << "Param error,Please check." << endl;
			ProcessLog(_T("Param error,Please check."), LOGDEBUG);
			return FALSE;
		}
		//加载csv文件
		TinyCSV csv(parser.getValStr("p", 0));

		//判断列表是否为空
		if (!csv.isValid())
		{
			//csv为空
			ProcessLog(_T("csv is empty"), LOGDEBUG);
			return FALSE;
		}

		
		if (!(atoi(parser.getValStr("row", 0).c_str()) < csv.rowCount() && atoi(parser.getValStr("col", 0).c_str()) < csv.colCount()))
		{
			//row,col超出范围
			ProcessLog(_T("row or col out of range."), LOGDEBUG);
			return FALSE;
		}
		//检查是否有flag列
		if (parser.hasKey("f") != 0 && parser.hasKey("fv") != 0)
		{
			for (int j = 0; j < csv.colCount(); ++j)
			{
				if (csv[0][j] == parser.getValStr("f", 0))
				{
					//获得type所在的列
					flagCol = j;
					break;
				}
			}
		}


		if (parser.hasKey("r") != 0)//读
		{
			if (flagCol!=-1)
			{
				if (csv[atoi(parser.getValStr("row", 0).c_str())][flagCol] == parser.getValStr("fv", 0))
				{
					//等于flag说明此数据已经使用
					ProcessLog(_T("data used."), LOGDEBUG);
					return FALSE;
				}
				else
				{
					cout << csv.getString(atoi(parser.getValStr("row", 0).c_str()), atoi(parser.getValStr("col", 0).c_str())) << endl;
					csv.setString(atoi(parser.getValStr("row", 0).c_str()), flagCol, parser.getValStr("fv", 0));
					if (csv.save(parser.getValStr("p", 0)))
					{
						return TRUE;
					}
				}
			}
			else
			{
				cout << csv.getString(atoi(parser.getValStr("row", 0).c_str()), atoi(parser.getValStr("col", 0).c_str())) << endl;
			}
		}
		else if (parser.hasKey("w") != 0 && parser.hasKey("v"))//写
		{
			if (flagCol != -1)
			{
				if (csv[atoi(parser.getValStr("row", 0).c_str())][flagCol] == parser.getValStr("fv", 0))
				{
					//等于flag说明此数据已经使用
					ProcessLog(_T("data used."), LOGDEBUG);
					return FALSE;
				}
				else
				{
					csv.setString(atoi(parser.getValStr("row", 0).c_str()), atoi(parser.getValStr("col", 0).c_str()), parser.getValStr("v", 0));
					csv.setString(atoi(parser.getValStr("row", 0).c_str()), flagCol, parser.getValStr("fv", 0));
					if (csv.save(parser.getValStr("p", 0)))
					{
						return TRUE;
					}
				}
			}
			else
			{
				csv.setString(atoi(parser.getValStr("row", 0).c_str()), atoi(parser.getValStr("col", 0).c_str()), parser.getValStr("v", 0));
				if (csv.save(parser.getValStr("p", 0)))
				{
					return TRUE;
				}
			}
		}


	}
	else if (parser.hasKey("t") != 0)//采用类型定位
	{
		//命令完整性检查
		if (parser.hasKey("p") == 0 || (parser.hasKey("w") == 0 && parser.hasKey("r") == 0) || parser.hasKey("t") == 0 || parser.hasKey("f") == 0 && parser.hasKey("fv") == 0)
		{
			cout << "Param error,Please check." << endl;
			ProcessLog(_T("Param error,Please check."), LOGDEBUG);
			return FALSE;
		}
		//加载csv文件
		TinyCSV csv(parser.getValStr("p", 0));

		//判断列表是否为空
		if (!csv.isValid())
		{
			//csv为空
			ProcessLog(_T("csv is empty"), LOGDEBUG);
			return FALSE;
		}

		for (int i = 0; i < csv.colCount(); ++i)
		{
			if (csv[0][i] == parser.getValStr("f", 0))
			{
				//获得type所在的列
				flagCol = i;
			}
			else if (csv[0][i] == parser.getValStr("t", 0))
			{
				TargetCol = i;
			}
		}



		if (parser.hasKey("r") != 0)//读
		{
			if (flagCol != -1)
			{
				//查找未被使用的行
				for (int j = 1; j < csv.rowCount(); ++j)
				{
					if (csv[j][flagCol] != parser.getValStr("fv", 0))
					{
						//找到未使用的一行记录
						cout << csv.getString(j, TargetCol) << endl;
						csv.setString(j, flagCol, parser.getValStr("fv", 0));
						if (csv.save(parser.getValStr("p", 0)))
						{
							return TRUE;
						}
					}
				}
			}
			else
			{
				return FALSE;
			}
		}
		else if (parser.hasKey("w") != 0 && parser.hasKey("v"))//写
		{
			if (flagCol != -1)
			{
				//查找未被使用的行
				for (int j = 1; j < csv.rowCount(); ++j)
				{
					if (csv[j][flagCol] != parser.getValStr("fv", 0))
					{
						//找到未使用的一行记录
						csv.setString(j, TargetCol, parser.getValStr("v", 0));
						csv.setString(j, flagCol, parser.getValStr("fv", 0));
						if (csv.save(parser.getValStr("p", 0)))
						{
							return TRUE;
						}
					}
				}
			}
			else
			{
				return FALSE;
			}
		}

	}

	return FALSE;
}

void help()
{
	cout << "Usage: " << endl;
	cout << "-w write:              write value to csv." << endl;
	cout << "-r read:               read value from csv." << endl;
	cout << "-p path:               csv path." << endl;
	cout << "-row:                  set write/read row." << endl;
	cout << "-column:               set write/read column." << endl;
	cout << "-v value:              write value." << endl;
	cout << "-s scan:               find the element,and return row/col." << endl;
	//cout << "-t type:               read/write which column=type & flag is OK" << endl;
	//cout << "-f flagColumn:         flag status column." << endl;
	//cout << "-fv flagvalue:         flag status value." << endl;
	cout << "-h help:               show help message." << endl;
	cout << "example: " << endl;
	cout << "xxx.exe -p \"C:\\test.csv\" -r -row 3 -column 3" << endl;
	//WriteReadcsv.exe -p "TestDate.csv" -r -row 2 -col 0 -f "flag"
	//WriteReadcsv.exe -p "TestDate.csv" -w -row 8 -col 14 -v "test"
	//WriteReadcsv.exe -p "TestDate.csv" -w -t "SN" -v "TEST" -f "flag" -fv "invalid"
	//WriteReadcsv.exe -p "TestDate.csv" -r -t "SN" -f "flag" -fv "invalid"
	//WriteReadcsv.exe -p "MacData.csv" -s MAC1 -s c
}

int main(int argc, char* argcv[])
{

	if (argc > 1)
	{
		parser.parse(argc, argcv);
		for (unsigned int i = 0; i < parser.getParamsCount(); i++)
		{
			if (parser.getKeyName(i) == "h" || parser.getKeyName(i) == "help")
			{
				help();
				return TRUE;
			}
		}
	}
	else
	{
		help();
		return TRUE;
	}
	
	//Exec function
	return Exefunction();
}