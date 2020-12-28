//==============================================================================//
//                                                                              //
//    RDB Diplomaterv Monitor                                                   //
//    A monitor program for the RDB Diplomaterv project                         //
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



#include <fstream>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QString>



#ifndef TEST_PROTOCOL_COMMON_H
#define TEST_PROTOCOL_COMMON_H



struct TestProtocolParameter
{
    TestProtocolParameter(const QString& new_file_name,
                          const int& new_expected_correct_diagrams) : file_name(new_file_name),
                                                                      expected_correct_diagrams(new_expected_correct_diagrams) {}
    QString file_name;
    int expected_correct_diagrams;
};

#endif // TEST_PROTOCOL_COMMON_H
