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
#include <memory>
#include <variant>

#include <QAbstractItemModel>
#include <QModelIndex>

#include "global.hpp"
#include "diagram.hpp"



#ifndef DIAGRAM_CONTAINER_H
#define DIAGRAM_CONTAINER_H

//#define DIAGRAM_CONTAINER_DEBUG_MODE



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

    std::size_t GetNumberOfDiagrams(void) const {return root_element->CountElementsBelowWithType<Element::DataType_Diagram>();}
    void AddDiagramFromFile(const std::string& file_path, const DiagramSpecialized& diagram);
    void AddDiagramFromNetwork(const std::string& connection_name, const DiagramSpecialized& diagram);

    // Members overridden from the QAbstractItemModel
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    // The type definition of the elements of the diagram container
    class Element
    {
    public:
        // Data type used for elements that do not contain a diagram but is only a name entry
        using DataType_Name = std::string;
        // Data type used of elements that actually contain a diagram
        using DataType_Diagram = DiagramSpecialized;
        // The two above data types combined
        using DataType = std::variant<DataType_Name, DataType_Diagram>;

        explicit Element(const DataType& new_data, Element* new_parent = nullptr) : data(new_data), parent(new_parent) {}

        Element(const Element& new_backend) = delete;
        Element(Element&& new_backend) = delete;

        Element& operator=(const Element& new_backend) = delete;
        Element& operator=(Element&& new_backend) = delete;

        ~Element() = default;

        bool IsRoot(void) const {return (nullptr == parent);}
        std::size_t GetNumberOfChildren(void) const {return children.size();}
        Element* CreateChild(const DataType& childs_data);
        Element* GetChildWithIndex(const std::size_t& index);
        Element* GetChildWithNameEntry(const DataType_Name& name_to_look_for);
        bool GetIndexOfChild(const Element* child, std::size_t& index_of_child);
        void KillTheChildren(void) {children.clear();}
        void KillChild(std::size_t child_index) {children.erase(children.begin() + child_index);}
        template <typename T>
            bool ContainsType(void) const {return std::holds_alternative<T>(data);}
        template <typename T>
            std::size_t CountElementsBelowWithType(void) const
            {
                std::size_t result = 0;

                for(const auto& i : children)
                {
                    result += i->CountElementsBelowWithType<T>();
                }

                return result;
            }

        std::string GetDisplayableString(void) const;
#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
        void PrintAllElementsBelow(const std::string& identation = "   ") const;
#endif

        // The data contained by the element
        DataType data;
        // The parent of this element, if there is no parent, it has the value nullptr
        Element* parent;
        // The elements whose parent is this element
        std::vector<std::unique_ptr<Element> > children;
    };

    void AddDiagramToElement(Element* element, const std::string& file_or_connection_name, const DiagramSpecialized &diagram);
    QModelIndex GetModelIndexOfElement(Element* element) const;
    Element* AddChildToElement(Element* element, Element::DataType data);
    void RemoveChildFromElement(Element* element, Element* child);

    // Every element contains only one column
    static constexpr int column_count = 1;
    static constexpr char root_element_data[] = "Available diagrams";
    static constexpr char files_element_data[] = "Diagrams loaded from files";
    static constexpr char network_element_data[] = "Diagrams received on the network";
    static constexpr char empty_element_data[] = "No diagrams yet...";
    // This is root element of the tree, it only contains a string that can be used as header in the view
    std::unique_ptr<Element> root_element;
    // The following two members are helper pointers to easily access the elements directly below the root elements
    // This is a pointer to the element that is responsible for holding the diagrams that were loaded from files
    Element* files_element;
    // This is a pointer to the element that is responsible for holding the diagrams that were received trough the network connetions
    Element* network_element;
};

#endif // DIAGRAM_CONTAINER_H
