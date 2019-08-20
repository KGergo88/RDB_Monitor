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



#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QJsonDocument>

#include "../application/sources/configuration.hpp"

class TestConfiguration : public ::testing::Test
{
 protected:
  void SetUp() override
  {
      // Checking whether thet test config file exists and if so, delete it
      if(std::filesystem::exists(test_configuration_path))
      {
          std::filesystem::remove(test_configuration_path);
      }
  }

  // The path to the test config file
  std::filesystem::path test_configuration_path = std::filesystem::current_path() / "test_config.json";
};

TEST_F(TestConfiguration, Constructor)
{
    // Constructing a Configuration object
    std::unique_ptr<Configuration> test_configuration = std::make_unique<Configuration>(test_configuration_path);
    // We expect that the config file will not be written to the disk during construction
    EXPECT_FALSE(std::filesystem::exists(test_configuration_path));
    // Destructing the Configuration object
    test_configuration.reset();
    // We expect that the config fill will be written to the disk during destruction
    EXPECT_TRUE(std::filesystem::exists(test_configuration_path));

    // Let's examine the created configuration file and change the values on the disk
    QString new_import_folder_value = "This/is/the/new/value/for/import/folder";
    QString new_export_folder_value = "This/is/the/new/value/for/export/folder";
    QFile file(QString::fromStdString(test_configuration_path.string()));
    QJsonObject config_file_content;

    // Open the stored config file
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        // And read out its content
        auto json_document = QJsonDocument::fromJson(file.readAll());
        EXPECT_FALSE(json_document.isNull());
        EXPECT_TRUE(json_document.isObject());
        config_file_content = json_document.object();
    }
    file.close();

    // Check the read out content of the file
    EXPECT_EQ(config_file_content["import_folder"], QDir::homePath());
    EXPECT_EQ(config_file_content["export_folder"], QDir::homePath());

    // Modify the read out content
    config_file_content["import_folder"] = new_import_folder_value;
    config_file_content["export_folder"] = new_export_folder_value;

    // And write it back to the disk
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        file.write(QJsonDocument(config_file_content).toJson());
    }
    file.close();

    // Constructing a Configuration object (now we expect that it will read the changed config file)
    test_configuration = std::make_unique<Configuration>(test_configuration_path);
    // Check whether the updated content was loaded during construction
    EXPECT_EQ(test_configuration->ImportFolder(), config_file_content["import_folder"].toString().toStdString());
    EXPECT_EQ(test_configuration->ExportFolder(), config_file_content["export_folder"].toString().toStdString());
}

TEST_F(TestConfiguration, ImportFolder)
{
    // Constructing a Configuration object
    std::unique_ptr<Configuration> test_configuration = std::make_unique<Configuration>(test_configuration_path);

    // Get the import folder value, then change it
    auto import_folder_value = test_configuration->ImportFolder();
    import_folder_value.append("modification");

    // Write back the updated value and check whether it was changed correctly
    test_configuration->ImportFolder(import_folder_value);
    auto read_back_value = test_configuration->ImportFolder();
    EXPECT_EQ(read_back_value, import_folder_value);
}

TEST_F(TestConfiguration, ExportFolder)
{
    // Constructing a Configuration object
    std::unique_ptr<Configuration> test_configuration = std::make_unique<Configuration>(test_configuration_path);

    // Get the export folder value, then change it
    auto export_folder_value = test_configuration->ExportFolder();
    export_folder_value.append("modification");

    // Write back the updated value and check whether it was changed correctly
    test_configuration->ExportFolder(export_folder_value);
    auto read_back_value = test_configuration->ExportFolder();
    EXPECT_EQ(read_back_value, export_folder_value);
}
