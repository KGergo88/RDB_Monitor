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



#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <memory>

#include <QAbstractItemModel>
#include <QModelIndex>

#include "global.hpp"
#include "diagram.hpp"



#ifndef DIAGRAM_CONTAINER_H
#define DIAGRAM_CONTAINER_H



class DiagramContainer : public QAbstractItemModel
{
    Q_OBJECT

public:
    DiagramContainer(QObject* parent = nullptr);

    DiagramContainer(const DiagramContainer& new_backend) = delete;
    DiagramContainer(DiagramContainer&& new_backend) = delete;

    DiagramContainer& operator=(const DiagramContainer& new_backend) = delete;
    DiagramContainer& operator=(DiagramContainer&& new_backend) = delete;

    virtual ~DiagramContainer() override = default;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    class Element
    {
    public:
        explicit Element(const std::string& new_name, Element* new_parent = nullptr) : name(new_name), parent(new_parent) {}

        Element(const Element& new_backend) = delete;
        Element(Element&& new_backend) = delete;

        Element& operator=(const Element& new_backend) = delete;
        Element& operator=(Element&& new_backend) = delete;

        bool operator<(const Element& other) const {return (name < other.name);}

        ~Element() = default;

        bool AmIRootElement(void) const {return (nullptr == parent);}
        Element* GetParent(void) {return parent;}
        std::size_t GetNumberOfChildren(void) const {return children.size();}
        Element& CreateChild(const std::string& childs_name)
        {
            children.push_back(std::make_unique<Element>(childs_name, this));
            return *children.at(children.size() - 1);
        }

        Element* GetChildAddress(const std::size_t& index)
        {
            if(index < children.size())
            {
                return children.at(index).get();
            }
            else
            {
                std::string errorMessage = "The indexed DiagramContainer::Element::children does not exist. /n Requested index: ";
                errorMessage += std::to_string(index);
                errorMessage += "/nMax index: ";
                errorMessage += std::to_string(children.size());
                throw errorMessage;
            }
        }

        void PrintAllElementsBelow(const std::string& identation = "   ") const
        {
            std::cout << identation << "Name: \"" << name << "\", Address: " << this << std::endl;

            for(const auto& i : children)
            {
                i->PrintAllElementsBelow(identation + identation);
            }
        }

        std::string name;
        Element* parent;
        std::vector<std::unique_ptr<Element> > children;
    };

    static constexpr int column_count = 1;
    std::unique_ptr<Element> root_item;
};

#endif // DIAGRAM_CONTAINER_H
