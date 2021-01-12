//==============================================================================//
//                                                                              //
//    RDB Monitor                                                               //
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
      if(QFileInfo(QString::fromStdString(test_configuration_path)).exists())
      {
          QFile::remove(QString::fromStdString(test_configuration_path));
      }
  }

  void ReadConfigFileContent(QJsonObject& config_file_content, std::string path = "")
  {
      // Let's drop the current content
      config_file_content = QJsonObject();

      QFile file;
      if("" == path)
      {
          file.setFileName(QString::fromStdString(test_configuration_path));
      }
      else
      {
          file.setFileName(QString::fromStdString(path));
      }

      // Open the stored config file
      if(file.open(QIODevice::ReadWrite | QIODevice::Text))
      {
          // And read out its content
          auto json_document = QJsonDocument::fromJson(file.readAll());
          ASSERT_FALSE(json_document.isNull());
          ASSERT_TRUE(json_document.isObject());
          config_file_content = json_document.object();
      }
      file.close();

      // Check whether we managed to read the config file content
      ASSERT_FALSE(config_file_content.empty());
  }

  void WriteConfigFileContent(QJsonObject& config_file_content, std::string path = "")
  {
      QFile file;
      if("" == path)
      {
          file.setFileName(QString::fromStdString(test_configuration_path));
      }
      else
      {
          file.setFileName(QString::fromStdString(path));
      }

      if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
      {
          file.write(QJsonDocument(config_file_content).toJson());
      }
      file.close();
  }

  // The path to the test config file
  std::string test_configuration_path = QDir(QDir::currentPath()).filePath("test_config.json").toStdString();
};

TEST_F(TestConfiguration, Constructor)
{
    // Constructing a Configuration object
    std::unique_ptr<Configuration> test_configuration = std::make_unique<Configuration>(test_configuration_path);
    // We expect that the config file will not be written to the disk during construction
    ASSERT_FALSE(QFileInfo::exists(QString::fromStdString(test_configuration_path)));
    // Destructing the Configuration object
    test_configuration.reset();
    // We expect that the config fill will be written to the disk during destruction
    ASSERT_TRUE(QFileInfo::exists(QString::fromStdString(test_configuration_path)));

    // Let's examine the created configuration file and change the values on the disk
    QString new_import_folder_value = "This/is/the/new/value/for/import/folder";
    QString new_export_folder_value = "This/is/the/new/value/for/export/folder";
    QJsonObject config_file_content;
    ReadConfigFileContent(config_file_content);

    // Check the read out content of the file
    ASSERT_EQ(config_file_content["import_folder"], QDir::homePath());
    ASSERT_EQ(config_file_content["export_folder"], QDir::homePath());

    // Modify the read out content
    config_file_content["import_folder"] = new_import_folder_value;
    config_file_content["export_folder"] = new_export_folder_value;

    // And write it back to the disk
    WriteConfigFileContent(config_file_content);

    // Constructing a Configuration object (now we expect that it will read the changed config file)
    test_configuration = std::make_unique<Configuration>(test_configuration_path);
    // Check whether the updated content was loaded during construction
    ASSERT_EQ(test_configuration->ImportFolder(), config_file_content["import_folder"].toString().toStdString());
    ASSERT_EQ(test_configuration->ExportFolder(), config_file_content["export_folder"].toString().toStdString());
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
    ASSERT_EQ(read_back_value, import_folder_value);
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
    ASSERT_EQ(read_back_value, export_folder_value);
}

TEST_F(TestConfiguration, OutdatedConfiguration)
{
    // This test checks whether the functionality of updating an older config file works as expected

    // Constructing a Configuration object
    std::unique_ptr<Configuration> test_configuration = std::make_unique<Configuration>(test_configuration_path);
    // Destructing the object to create the configuration file
    test_configuration.reset();

    // Open the stored config file
    QFile file(QString::fromStdString(test_configuration_path));
    QJsonObject config_file_content;
    ReadConfigFileContent(config_file_content);

    // Remove one of the settings and add a new one to simulate an outdated config file
    auto key_to_remove = "import_folder";
    auto key_to_add = "this_is_an_old_key_that_is_not_used_anymore";
    auto value_to_add = "and_this_is_its_old_value";
    ASSERT_TRUE(config_file_content.contains(key_to_remove)) << "Test error! This setting does not seem to exist anymore, pick another one!";
    config_file_content.remove(key_to_remove);
    config_file_content.insert(key_to_add, value_to_add);

    // And write it back to the disk
    WriteConfigFileContent(config_file_content);

    // Constructing a Configuration object
    test_configuration = std::make_unique<Configuration>(test_configuration_path);
    // Destructing the object to create the configuration file
    test_configuration.reset();

    // Check whether the removed key was added and the added key-value pair is still there
    ReadConfigFileContent(config_file_content);
    ASSERT_TRUE(config_file_content.contains(key_to_remove));
    ASSERT_TRUE(config_file_content.contains(key_to_add));
    ASSERT_EQ(config_file_content[key_to_add], value_to_add);
}
