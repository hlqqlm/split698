/*
This program is licensed under the GPLv2 License:
Copyright 2023 Huanglin <hlqqlm@qq.com>


This file is part of Split698.

Split698 is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

Split698 is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Split698. If not, see <https://www.gnu.org/licenses/>.


20230730-----------------------------------------------------------------------
Huanglin创建.
*/

#define PRINT_FILEID_TABLE_EN		(0)
#define P2_PROCESS_EN				(1)

#include <iostream>

#include "cli11/CLI11.hpp"

#include "qos/qlibc.h"
#include "qos/fileid.h"
#include "qos/qcp.h"
#include "qos/qstr.h"

#if P2_PROCESS_EN > 0
#include "dlt698/p2_process.h"
#endif

#include "main.xcp.h"




static const FileidItem kFileidItems[] = {
#include "id_filename_table_uniq.h"
	    //#include "../libctlcomm/id_filename_table_uniq.h"
	    //#include "../libctl698/id_filename_table_uniq.h"
};
static const int kFileidNum = sizeof(kFileidItems) / sizeof(kFileidItems[0]);
static const FileidTable kFileidTable = {kFileidItems, kFileidNum};
const FileidTable *MainFileidTable() { return &kFileidTable; }



// only keep 0~9 a~f A~F in a string
static std::string Filter(const std::string &to) 
{
	std::string ret;
	for(std::string::const_iterator it = to.begin(); it != to.end(); ++it)
	{
		if((*it >= '0' && *it <= '9') || (*it >= 'a' && *it <= 'f') || (*it >= 'A' && *it <= 'F'))
		{
			ret += *it;
		}
	}
	return ret;
}


int main(int argc, char **argv)
{
	cp_t cp = 0;

	CLI::App app{"App description"};

	//std::string filename = "default";
	//app.add_option("-f,--file", filename, "A help string");

	bool fileid_table_en = false;
	//app.add_option("-f,--fileid_table_en", fileid_table_en, "Print fileid table");
	app.add_flag("-f,--fileid_table,!--no_fileid_table", fileid_table_en, "Print fileid table");

	bool count_en = false;
	app.add_flag("-c,--count,!--no_count", count_en, "Print frame count");

	// std::vector<double> v1;
	//app.add_option(std::vector<double>,int>("--vs",v1);
	// app.add_option("--vs",v1);

	CLI11_PARSE(app, argc, argv);

	// app.count("--file");

	// if (PRINT_FILEID_TABLE_EN) {
	if (fileid_table_en) {
		FileidTablePrint(kFileidItems, kFileidNum);
	}

#if P2_PROCESS_EN > 0
	P2Process p2_process = kP2ProcessDef;
	ifed(P2ProcessOpen(&p2_process));
#endif
	//std::string line = "683500430502000020114310f42b10001105020003001002002000020020040200000110087406efe4e8900cda5d3ec56df93d3a2b8b16";
	// std::string line = "685500430503000000000010baa71000310503085004020002202102001c07e509180000001c07e50918173b3b5403000103002021020000001002000000200200000110f0d55698838e08158f013b87d49a8cd72f6316";
	std::string line;

	//while (std::getline(std::cin, line) && !line.empty()) {
	// while (!line.empty()) {
	while (std::getline(std::cin, line)) {
		const std::string only_hex_char = Filter(line) ;
		const std::string mem = HexStr2Mem(only_hex_char);
		printf_hex_ex("rx: ", LINEND, mem.data(), mem.size(), "");
		
#if P2_PROCESS_EN > 0
		char tx_buf[4096];
		const int tx_buf_size = sizeof(tx_buf);
		cp = P2ProcessInput(&p2_process, mem.data(), mem.size(), tx_buf, tx_buf_size);
#endif

		//std::cout << "mem is: " << HexStr(mem) << std::endl;
		// std::cout << line << std::endl;
		// line = "";
	}
			  

#if P2_PROCESS_EN > 0
	ifed(P2ProcessClose(&p2_process));
#endif

	const int result = (0 == cp) ? 0 : -1;
    return result;
}
