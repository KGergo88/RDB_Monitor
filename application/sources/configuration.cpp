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



#include "configuration.hpp"



bool Configuration::LoadExistingConfiguration()
{
    bool bResult = false;

    if(QFileInfo::exists(QString::fromStdString(configuration_file_path)))
    {
        QFile file(QString::fromStdString(configuration_file_path));
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            auto json_document = QJsonDocument::fromJson(file.readAll());

            if(!json_document.isNull())
            {
                if(json_document.isObject())
                {
                    // Taking the JSON data out of the document and storing it
                    data = json_document.object();

                    // Checking whether all the valid settings are contained by the configuration
                    auto iterator = valid_settings.begin();
                    while(iterator != valid_settings.end())
                    {
                        // Does the configuration contain this valid setting?
                        if(!CheckSetting(iterator->name, iterator->default_value.type()))
                        {
                            // If not, then we will add it with the default value so in case of adding a new setting, the old config file will be updated.
                            // The unused settins will remain the config file, but will not be used anymore.
                            data.insert(iterator->name, iterator->default_value);
                        }
                        iterator++;
                    }
                    // If we have reached the end, all the elements passed the check or they were added to the config file
                    if(valid_settings.end() == iterator)
                    {
                        bResult = true;
                    }
                }
            }
        }
        file.close();
    }

    return bResult;
}

bool Configuration::CheckSetting(const QString& name, QJsonValue::Type type) const
{
    bool bResult = false;

    if(data.contains(QString(name)))
    {
        if(type == data[name].type())
        {
            bResult = true;
        }
    }

    return bResult;
}

QJsonObject Configuration::CreateEmptyConfiguration(void)
{
    QJsonObject result;

    for(const auto& i: valid_settings)
    {
        result.insert(i.name, i.default_value);
    }

    return result;
}

void Configuration::SaveConfiguration(void)
{
    QFile file(QString::fromStdString(configuration_file_path));
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {        
        file.write(QJsonDocument(data).toJson());
    }
    file.close();
}
