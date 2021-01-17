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


#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP


#include <set>

#include <QString>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


class Configuration
{
public:
    Configuration(std::string new_configuration_file_path = QDir(QDir::currentPath()).filePath(configuration_file_name).toStdString())
        : configuration_file_path(new_configuration_file_path)
    {
        valid_settings.emplace(setting_import_folder, QDir::homePath());
        valid_settings.emplace(setting_export_folder, QDir::homePath());

        if(!LoadExistingConfiguration())
        {
            data = CreateEmptyConfiguration();
        }
    }

    Configuration(const Configuration&) = delete;
    Configuration(Configuration&&) = delete;

    ~Configuration()
    {
        SaveConfiguration();
    }

    Configuration& operator=(const Configuration&) = delete;
    Configuration& operator=(Configuration&&) = delete;

    std::string ImportFolder(void) {return data[setting_import_folder].toString().toStdString();}
    void ImportFolder(const std::string& new_value) {data[setting_import_folder] = QJsonValue(QString::fromStdString(new_value));}
    std::string ExportFolder(void) {return data[setting_export_folder].toString().toStdString();}
    void ExportFolder(const std::string& new_value) {data[setting_export_folder] = QString::fromStdString(new_value);}

private:
    bool LoadExistingConfiguration(void);
    bool CheckSetting(const QString& name, QJsonValue::Type type) const;
    QJsonObject CreateEmptyConfiguration(void);
    void SaveConfiguration(void);

    struct Setting
    {
        Setting(const std::string& new_name, QJsonValue new_default_value) : name(QString::fromStdString(new_name)), default_value(new_default_value) {}

        bool operator<(const Setting& other) const {return (name < other.name);}

        QString name;
        QJsonValue default_value;
    };

    static constexpr char configuration_file_name[] = "configuration.json";
    static constexpr char setting_import_folder[] = "import_folder";
    static constexpr char setting_export_folder[] = "export_folder";

    std::set<Setting> valid_settings;
    const std::string configuration_file_path;
    QJsonObject data;
};


#endif /* CONFIGURATION_HPP */
