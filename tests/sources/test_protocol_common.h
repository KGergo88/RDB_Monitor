//==============================================================================//
//                                                                              //
//    RDB Monitor                                                               //
//    Copyright (C) 2018  András Gergő Kocsis                                   //
//                                                                              //
//    This program is free software: you can redistribute it and/or modify      //
//    it under the terms of the GNU General Public License as published by      //
//    the Free Software Foundation, either version 3 of the License, or         //
//    (at your option) any later version.                                       //
//                                                                              //
//    This program is distributed in the hope that it will be useful,           //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of            //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
//    GNU General Public License for more details.                              //
//                                                                              //
//    You should have received a copy of the GNU General Public License         //
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.    //
//                                                                              //
//==============================================================================//


#ifndef TEST_PROTOCOL_COMMON_H
#define TEST_PROTOCOL_COMMON_H


#include <fstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include <QString>
#include <QDir>
#include <QModelIndex>
#include <QCoreApplication>


/*struct ProtocolCallbackMocks {
    testing::MockFunction<std::vector<QModelIndex>(std::vector<DefaultDiagram> &)> diagram_collector;
    testing::MockFunction<void(const QModelIndex &, const DefaultDiagram &)> diagram_updater;
    testing::MockFunction<void(const std::string &)> error_reporter;
};*/

class ProtocolCallbackMocks
{
public:
    MOCK_METHOD(std::vector<QModelIndex>, diagram_collector, (std::vector<DefaultDiagram>&), ());
    MOCK_METHOD(void, diagram_updater, (const QModelIndex&, const DefaultDiagram&), ());
    MOCK_METHOD(void, error_reporter, (const std::string&), ());
};

struct TestProtocolParameter
{
    TestProtocolParameter(const QString& new_file_name,
                          const int& new_expected_correct_diagrams) : file_name(new_file_name),
                                                                      expected_correct_diagrams(new_expected_correct_diagrams) {}
    QString file_name;
    int expected_correct_diagrams;
};

struct TestFileReader
{
    static std::ifstream read(const QString& file_name, const QString& test_files_path = QDir(QCoreApplication::applicationDirPath()).filePath("test_files"))
    {
        std::string test_file_path = QDir(test_files_path).filePath(file_name).toStdString();
        std::ifstream test_file_stream(test_file_path);

        if(!test_file_stream.is_open())
        {
            ADD_FAILURE() << "The file could not be opened! Path: " << test_file_path;
        }

        return test_file_stream;
    }
};


#endif // TEST_PROTOCOL_COMMON_H
