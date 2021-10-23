// Copyright 2021 Fe-Ti
// finance ftp stats
//#include <filesystem>
//#include <iostream>
//#include <list>
//#include <map>
//#include <regex>
//#include <string>
//
// namespace stdfs = std::filesystem;
// const std::regex PATTERN(
//  "balance_[0-9]{8}_[0-9]{4}(0[0-9]|1[0-2])([0-2][0-9]|3[01]).txt",
//  std::regex::egrep);
//
// const uint32_t acc_pos = 8;
// const uint32_t acc_len = 8;
// const uint32_t date_pos = 17;
// const uint32_t date_len = 8;
//
// std::list<stdfs::path>
// ls(const stdfs::path& desired_path = stdfs::current_path())
//{
//    std::list<stdfs::path> dir_contents;
//    for(auto& item : stdfs::directory_iterator(desired_path)) {
//        dir_contents.push_back(stdfs::relative(item.path()).string());
//    }
//    return dir_contents;
//}
//
//// template<typename T>
//// std::ostream&
//// operator<<(std::ostream& out, const std::list<stdfs::path>& lst)
////{
////    for(const stdfs::path& i : lst)
////        out << i.string() << std::endl;
////    return out;
////}
//
// void
// cd_rel(const stdfs::path& desired_path)
//{
//    stdfs::current_path(stdfs::current_path() / desired_path);
//}
//
// void
// go_back()
//{
//    cd_rel(stdfs::path(".."));
//}
//
// void
// parse_balance_string(
//  const std::string& b_filename,
//  const std::string& broker,
//  std::map<std::string, std::map<std::string, std::pair<uint32_t, uint32_t>>>&
//    statistics)
//{
//    std::string account = b_filename.substr(acc_pos, acc_len);
//    uint32_t date = std::stoi({ b_filename.substr(date_pos, date_len) });
//
//    try {
//        ++statistics.at(broker).at(account).first;
//        if(statistics.at(broker).at(account).second < date)
//            statistics.at(broker).at(account).second = date;
//    } catch(const std::out_of_range&) {
//        statistics[broker][account] = { 1, date };
//    }
//}
//
// int
// main()
//{
//    cd_rel(stdfs::path("ftp"));
//    std::list<stdfs::path> broker_list = ls();
//    std::list<stdfs::path> report_list;
//
//    std::map<std::string, std::map<std::string, std::pair<uint32_t,
//    uint32_t>>>
//      statistics; // Oh, yeah! That's a perfect sh*!
//    for(stdfs::path& i : broker_list) {
//        cd_rel(i);
//        report_list = ls();
//        for(stdfs::path& j : report_list)
//            if(std::regex_match(j.string(), PATTERN)) {
//                std::cout << i.string() << " " << j.string() << std::endl;
//                parse_balance_string(j.string(), i.string(), statistics);
//            }
//        go_back();
//    }
//    for(auto& i : statistics) {
//        std::cout << "Broker: " << i.first << std::endl;
//        for(auto& j : i.second) {
//            std::cout << "| Account: " << j.first << std::endl;
//            std::cout << "| | Files:       " << j.second.first << std::endl;
//            std::cout << "| |_Most recent: " << j.second.second << std::endl;
//        }
//        std::cout << "|_"<< std::endl;
//    }
//    return 0;
//}
// Copyright 2021 Fe-Ti
// finance ftp stats
#include <filesystem>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <string>

namespace stdfs = std::filesystem;
const std::regex PATTERN(
  "balance_[0-9]{8}_[0-9]{4}(0[0-9]|1[0-2])([0-2][0-9]|3[01]).txt",
  std::regex::egrep);

const uint32_t acc_pos = 8;
const uint32_t acc_len = 8;
const uint32_t date_pos = 17;
const uint32_t date_len = 8;

std::list<stdfs::path>
ls(const stdfs::path& desired_path = stdfs::current_path())
{
    std::list<stdfs::path> dir_contents;
    for(auto& item : stdfs::directory_iterator(desired_path)) {
        dir_contents.push_back(stdfs::relative(item.path()).string());
    }
    return dir_contents;
}

void
cd_rel(const stdfs::path& desired_path)
{
    stdfs::current_path(stdfs::current_path() / desired_path);
}

void
go_back()
{
    cd_rel(stdfs::path(".."));
}

void
parse_balance_string(
  const std::string& b_filename,
  const std::string& broker,
  std::map<std::string, std::map<std::string, std::pair<uint32_t, uint32_t>>>&
    statistics)
{
    std::string account = b_filename.substr(acc_pos, acc_len);
    uint32_t date = std::stoi({ b_filename.substr(date_pos, date_len) });

    try {
        ++statistics.at(broker).at(account).first;
        if(statistics.at(broker).at(account).second < date)
            statistics.at(broker).at(account).second = date;
    } catch(const std::out_of_range&) {
        statistics[broker][account] = { 1, date };
    }
}

int
main()
{
    try {
        cd_rel(stdfs::path("ftp"));
        std::list<stdfs::path> broker_list = ls();
        std::list<stdfs::path> report_list;

        std::map<std::string,
                 std::map<std::string, std::pair<uint32_t, uint32_t>>>
          statistics; // OMG! How many words!
                      // But this results in
                      // statistics[broker][account] = { file count , last date
                      // }

        for(stdfs::path& i : broker_list) {
            cd_rel(i);
            report_list = ls();
            for(stdfs::path& j : report_list)
                if(std::regex_match(j.string(), PATTERN)) {
                    std::cout << i.string() << " " << j.string() << std::endl;
                    parse_balance_string(j.string(), i.string(), statistics);
                }
            go_back();
        }
    } catch(const stdfs::filesystem_error& e) {
        std::cout << e.what() << std::endl;
    }
    for(auto& i : statistics) {
        for(auto& j : i.second) {
            std::cout << "broker:" << i.first << " ";
            std::cout << "account:" << j.first << " ";
            std::cout << "files:" << j.second.first << " ";
            std::cout << "lastdate:" << j.second.second << std::endl;
        }
    }
    return 0;
}
