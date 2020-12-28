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



#include <QCoreApplication>
#include <QString>
#include <QDir>

#include "../application/sources/global.hpp"
#include "../application/sources/measurement_data_protocol.hpp"
#include "test_protocol_common.h"



class TestMeasurementDataProtocol : public ::testing::Test,
                                    public testing::WithParamInterface<TestProtocolParameter>
{
protected:
    std::ifstream ReadTestFileContent(const QString& file_name)
    {
        std::string test_file_path = QDir(test_files_path).filePath(file_name).toStdString();
        std::ifstream test_file_stream(test_file_path);

        if(!test_file_stream.is_open())
        {
            ADD_FAILURE() << "The file could not be opened! Path: " << test_file_path;
        }

        return test_file_stream;
    }

    MeasurementDataProtocol test_mdp_processor;
    std::string expected_protocol_name = "Measurement Data Protocol MDP";
    std::string expected_file_type = "mdp";
    std::vector<DiagramSpecialized> processed_diagrams;

    // The path to the test files
    QString test_files_path = QDir(QCoreApplication::applicationDirPath()).filePath("test_files");
};

TEST_F(TestMeasurementDataProtocol, GetProtocolName)
{
    EXPECT_EQ(test_mdp_processor.GetProtocolName(), expected_protocol_name);
}

TEST_F(TestMeasurementDataProtocol, CanThisFileBeProcessed)
{
    std::string filename_to_test = std::string("myfile.") + expected_file_type;
    EXPECT_TRUE(test_mdp_processor.CanThisFileBeProcessed(filename_to_test));
    filename_to_test = std::string("mymdpfile.") + std::string("txt");
    EXPECT_FALSE(test_mdp_processor.CanThisFileBeProcessed(filename_to_test));
}

TEST_F(TestMeasurementDataProtocol, GetSupportedFileType)
{
    EXPECT_EQ(test_mdp_processor.GetSupportedFileType(), expected_file_type);
}

TEST_F(TestMeasurementDataProtocol, ProcessData_ExportData_EmptyStream)
{
    std::ifstream empty_stream;
    processed_diagrams = test_mdp_processor.ProcessData(empty_stream);
    EXPECT_EQ(processed_diagrams.size(), std::size_t(0));

    std::stringstream exported_data = test_mdp_processor.ExportData(processed_diagrams);
    processed_diagrams = test_mdp_processor.ProcessData(exported_data);
    EXPECT_EQ(processed_diagrams.size(), std::size_t(0));
}

TEST_P(TestMeasurementDataProtocol, ProcessData_ExportData)
{
    auto test_parameter = GetParam();
    std::ifstream file_stream = ReadTestFileContent(test_parameter.file_name);
    processed_diagrams = test_mdp_processor.ProcessData(file_stream);
    EXPECT_EQ(processed_diagrams.size(), std::size_t(test_parameter.expected_correct_diagrams)) << "test_file: " << test_parameter.file_name.toStdString();

    std::stringstream exported_data = test_mdp_processor.ExportData(processed_diagrams);
    processed_diagrams = test_mdp_processor.ProcessData(exported_data);
    EXPECT_EQ(processed_diagrams.size(), std::size_t(test_parameter.expected_correct_diagrams)) << "test_file: " << test_parameter.file_name.toStdString();
}

INSTANTIATE_TEST_SUITE_P(TestMeasurementDataProtocolInstantiation,
                         TestMeasurementDataProtocol,
                         testing::Values(TestProtocolParameter("TEST_1C_0E_MDP.mdp", 1),
                                         TestProtocolParameter("TEST_2C_0E_MDP.mdp", 2),
                                         TestProtocolParameter("TEST_1C_1E_MDP_HeadlineError.mdp", 1),
                                         TestProtocolParameter("TEST_1C_2E_MDP_DatalineError.mdp", 1)
                                         ));
