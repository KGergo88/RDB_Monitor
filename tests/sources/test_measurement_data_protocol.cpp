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


#include <functional>

#include "../application/sources/measurement_data_protocol.hpp"
#include "test_protocol_common.h"


using testing::_;

class TestMeasurementDataProtocol : public ::testing::Test,
                                    public testing::WithParamInterface<TestProtocolParameter>
{
protected:
    TestMeasurementDataProtocol()
    {
        test_mdp_processor.RegisterCallbacks(std::bind(&ProtocolCallbackMocks::diagram_collector, &mocks, std::placeholders::_1),
                                             std::bind(&ProtocolCallbackMocks::diagram_updater, &mocks, std::placeholders::_1, std::placeholders::_2),
                                             std::bind(&ProtocolCallbackMocks::error_reporter, &mocks, std::placeholders::_1));
    }

    void ExpectNoCallbacks(void)
    {
        EXPECT_CALL(mocks, diagram_collector(_)).Times(0);
        EXPECT_CALL(mocks, diagram_updater(_, _)).Times(0);
        EXPECT_CALL(mocks, error_reporter(_)).Times(0);
    }

    MeasurementDataProtocol test_mdp_processor;
    ProtocolCallbackMocks mocks;
    std::string expected_protocol_name = measurement_data_protocol_name;
    std::string expected_file_type = "mdp";
    std::vector<DefaultDiagram> processed_diagrams;
};

TEST_F(TestMeasurementDataProtocol, GetProtocolName)
{
    ExpectNoCallbacks();

    EXPECT_EQ(test_mdp_processor.GetProtocolName(), expected_protocol_name);
}

TEST_P(TestMeasurementDataProtocol, ProcessNetworkData)
{
    auto test_parameter = GetParam();
    std::ifstream file_stream = TestFileReader::read(test_parameter.file_name);

    EXPECT_CALL(mocks, diagram_collector(testing::SizeIs(test_parameter.expected_correct_diagrams)))
        .Times(1);
    ON_CALL(mocks, diagram_collector)
        .WillByDefault(testing::Return(std::vector<QModelIndex>(test_parameter.expected_correct_diagrams)));
    EXPECT_CALL(mocks, diagram_updater(_, _)).Times(0);
    EXPECT_CALL(mocks, error_reporter(_)).Times(0);

    test_mdp_processor.ProcessNetworkData(file_stream);
}

TEST_F(TestMeasurementDataProtocol, GetSupportedFileType)
{
    ExpectNoCallbacks();

    EXPECT_EQ(test_mdp_processor.GetSupportedFileType(), expected_file_type);
}

TEST_F(TestMeasurementDataProtocol, CanThisFileBeImportedFrom)
{
    ExpectNoCallbacks();

    std::string filename_to_test = std::string("myfile.") + expected_file_type;
    EXPECT_TRUE(test_mdp_processor.CanThisFileBeImportedFrom(filename_to_test));

    filename_to_test = std::string("mymdpfile.") + std::string("txt");
    EXPECT_FALSE(test_mdp_processor.CanThisFileBeImportedFrom(filename_to_test));
}

TEST_F(TestMeasurementDataProtocol, ImportFromFile_ExportToFile_EmptyStream)
{
    ExpectNoCallbacks();

    std::ifstream empty_stream;
    processed_diagrams = test_mdp_processor.ImportFromFile(empty_stream);
    EXPECT_EQ(processed_diagrams.size(), std::size_t(0));

    std::stringstream exported_data = test_mdp_processor.ExportToFile(processed_diagrams);
    processed_diagrams = test_mdp_processor.ImportFromFile(exported_data);
    EXPECT_EQ(processed_diagrams.size(), std::size_t(0));
}

TEST_P(TestMeasurementDataProtocol, ImportFromFile_ExportToFile)
{
    ExpectNoCallbacks();

    auto test_parameter = GetParam();
    std::ifstream file_stream = TestFileReader::read(test_parameter.file_name);
    processed_diagrams = test_mdp_processor.ImportFromFile(file_stream);
    EXPECT_EQ(processed_diagrams.size(), std::size_t(test_parameter.expected_correct_diagrams)) << "test_file: " << test_parameter.file_name.toStdString();

    std::stringstream exported_data = test_mdp_processor.ExportToFile(processed_diagrams);
    processed_diagrams = test_mdp_processor.ImportFromFile(exported_data);
    EXPECT_EQ(processed_diagrams.size(), std::size_t(test_parameter.expected_correct_diagrams)) << "test_file: " << test_parameter.file_name.toStdString();
}

INSTANTIATE_TEST_SUITE_P(TestMeasurementDataProtocolInstantiation,
                         TestMeasurementDataProtocol,
                         testing::Values(TestProtocolParameter("TEST_1C_0E_MDP.mdp", 1),
                                         TestProtocolParameter("TEST_2C_0E_MDP.mdp", 2),
                                         TestProtocolParameter("TEST_1C_1E_MDP_HeadlineError.mdp", 1),
                                         TestProtocolParameter("TEST_1C_2E_MDP_DatalineError.mdp", 1)
                                         ));
