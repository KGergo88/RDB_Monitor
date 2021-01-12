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



#include <QString>
#include <QDir>

#include "../application/sources/global.hpp"
#include "../application/sources/continous_measurement_data_protocol.hpp"
#include "test_protocol_common.h"



class TestContinousMeasurementDataProtocol : public ::testing::Test,
                                             public testing::WithParamInterface<TestProtocolParameter>
{
protected:
    ContinousMeasurementDataProtocol test_cmdp_processor;
    std::string expected_protocol_name = continous_measurement_data_protocol_name;
    std::vector<DiagramSpecialized> processed_diagrams;
    // Empty string as the CMDP does not support file handling
    std::string expected_file_type;
};

TEST_F(TestContinousMeasurementDataProtocol, GetProtocolName)
{
    EXPECT_EQ(test_cmdp_processor.GetProtocolName(), expected_protocol_name);
}

TEST_F(TestContinousMeasurementDataProtocol, CanThisFileBeProcessed)
{
    std::string filename_to_test = std::string("myfile.") + expected_file_type;
    EXPECT_FALSE(test_cmdp_processor.CanThisFileBeProcessed(filename_to_test));
    filename_to_test = std::string("mymdpfile.") + std::string("txt");
    EXPECT_FALSE(test_cmdp_processor.CanThisFileBeProcessed(filename_to_test));
}

TEST_F(TestContinousMeasurementDataProtocol, GetSupportedFileType)
{
    EXPECT_EQ(test_cmdp_processor.GetSupportedFileType(), expected_file_type);
}

TEST_F(TestContinousMeasurementDataProtocol, ProcessData_EmptyStream)
{
    std::ifstream empty_stream;
    processed_diagrams = test_cmdp_processor.ProcessData(empty_stream);
    EXPECT_EQ(processed_diagrams.size(), std::size_t(0));
}

TEST_F(TestContinousMeasurementDataProtocol, ExportData)
{
    std::stringstream exported_data;
    EXPECT_ANY_THROW(exported_data = test_cmdp_processor.ExportData(std::vector<DiagramSpecialized>()));
    EXPECT_EQ(exported_data.str(), std::string());
}

TEST_P(TestContinousMeasurementDataProtocol, ProcessData)
{
    auto test_parameter = GetParam();
    std::ifstream file_stream = TestFileReader::read(test_parameter.file_name);
    processed_diagrams = test_cmdp_processor.ProcessData(file_stream);
    EXPECT_EQ(processed_diagrams.size(), std::size_t(test_parameter.expected_correct_diagrams)) << "test_file: " << test_parameter.file_name.toStdString();
}

INSTANTIATE_TEST_SUITE_P(TestContinousMeasurementDataProtocolInstantiation,
                         TestContinousMeasurementDataProtocol,
                         testing::Values(TestProtocolParameter("TEST_1C_0E_CMDP.cmdp", 1),
                                         TestProtocolParameter("TEST_1C_0E_CMDP_NoTitles.cmdp", 1),
                                         TestProtocolParameter("TEST_1C_0E_CMDP_Simple.cmdp", 1)));
