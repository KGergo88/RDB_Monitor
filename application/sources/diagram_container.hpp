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



#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <variant>

#include <QAbstractItemModel>
#include <QModelIndex>

#include "global.hpp"
#include "diagram.hpp"



#ifndef DIAGRAM_CONTAINER_HPP
#define DIAGRAM_CONTAINER_HPP

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

    std::size_t GetNumberOfDiagrams(void) const {return root_element->CountElementsWithTypeRecursive<Element::DataType_Diagram>();}
    bool IsThisFileAlreadyStored(const std::string& file_name, const std::string& file_path);
    DiagramSpecialized* GetDiagram(const QModelIndex& model_index);
    void ShowCheckBoxes(void);
    void HideCheckBoxes(void);
    std::vector<DiagramSpecialized> GetCheckedDiagrams(void);
    QModelIndex AddDiagramFromNetwork(const std::string connection_name, const DiagramSpecialized& diagram);
    QModelIndex AddDiagramFromFile(const std::string file_name, const std::string& file_path, const DiagramSpecialized& diagram);

    // Members overridden from the QAbstractItemModel
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    // The type definition of the elements of the diagram container
    class Element
    {
    public:
        // Data type used for elements that only contain a string
        using DataType_Name = std::string;
        // Data type used for elements that represent a file that was imported
        struct DataType_File
        {
            DataType_File() = default;
            DataType_File(const std::string& new_name, const std::string& new_path) : name(new_name), path(new_path) {}
            bool operator==(const DataType_File& other) const {return ((name == other.name) && (path == other.path));}
            std::string name;
            std::string path;
        };
        // Data type used for elements that represent a network connection
        struct DataType_Connection
        {
            DataType_Connection() = default;
            DataType_Connection(const std::string& new_name) : name(new_name) {}
            bool operator==(const DataType_Connection& other) const {return (name == other.name);}
            std::string name;
        };
        // Data type used of elements that contain a diagram
        using DataType_Diagram = DiagramSpecialized;
        // The above data types combined
        using DataType = std::variant<DataType_Name, DataType_File, DataType_Connection, DataType_Diagram>;

        // Pre-set flag value
        static constexpr Qt::ItemFlags element_flags_default = Qt::ItemIsEnabled;

        explicit Element(const DataType& new_data, Element* new_parent = nullptr, const Qt::ItemFlags& new_flags = element_flags_default, const Qt::CheckState new_check_state = Qt::Unchecked)
            : data(new_data), parent(new_parent), flags(new_flags), check_state(new_check_state) {}

        Element(const Element& new_backend) = delete;
        Element(Element&& new_backend) = delete;

        Element& operator=(const Element& new_backend) = delete;
        Element& operator=(Element&& new_backend) = delete;

        ~Element() = default;

        bool IsRoot(void) const {return (nullptr == parent);}
        std::size_t GetNumberOfChildren(void) const {return children.size();}
        Element* CreateChild(const DataType& childs_data, const Qt::ItemFlags& childs_flags = element_flags_default, const Qt::CheckState& childs_check_state = Qt::CheckState::Unchecked);
        Element* GetChildWithIndex(const std::size_t& index);
        bool GetIndexWithChild(const Element* child, std::size_t& index_of_child);
        void KillTheChildren(void) {children.clear();}
        void KillChild(std::size_t child_index) {children.erase(children.begin() + child_index);}
        template <typename T> bool ContainsType(void) const {return std::holds_alternative<T>(data);}
        template <typename T> std::size_t CountElementsWithTypeRecursive(void) const
            {
                std::size_t result = 0;

                // Checking whether this element has the type
                if(std::holds_alternative<T>(data))
                {
                    result++;
                }

                // Going trough all the children and calling the same function on them
                for(const auto& i : children)
                {
                    result += i->CountElementsWithTypeRecursive<T>();
                }

                return result;
            }
        template <typename T> Element* GetChildWithData(const T& data_to_look_for)
        {
            Element* result = nullptr;

            for(const auto& i: children)
            {
                if(std::holds_alternative<T>(i->data))
                {
                    if(data_to_look_for == std::get<T>(i->data))
                    {
                        result = i.get();
                        break;
                    }
                }
            }

            return result;
        }
        std::string GetDisplayableString(void) const;
        void ChildsCheckStateHasChanged(void);
        void CallFunctionOnElementsRecursive(std::function<void(Element*)> function);
#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
        void PrintAllElementsRecursive(const std::string& identation = "   ") const;
#endif

        // The data contained by the element
        DataType data;
        // The parent of this element, if there is no parent, it has the value nullptr
        Element* parent;
        // The elements whose parent is this element
        std::vector<std::unique_ptr<Element> > children;
        // Flags in OR combination that control the visualisation of the element
        Qt::ItemFlags flags;
        // Flag that tells whether the element was checked by the user
        Qt::CheckState check_state;
    };

    QModelIndex AddDiagram(Element* type_parent, const DiagramSpecialized& diagram, const std::function<Element*(void)> storage_logic);
    QModelIndex GetModelIndexOfElement(Element* element) const;
    Element* AddChildToElement(Element* element, const Element::DataType& data);
    void RemoveChildFromElement(Element* element, Element* child);
    void SetCheckStateOfElement(Element* element, const Qt::CheckState& new_check_state);
    void ChangeDiagramTitleOfElement(Element* element, const std::string& new_diagram_title);

    // Every element contains only one column
    static constexpr int column_count = 1;
    // Pre-defined element data
    static const Element::DataType_Name root_element_data;
    static const Element::DataType_Name files_element_data;
    static const Element::DataType_Name network_element_data;
    static const Element::DataType_Name empty_element_data;
    // This is root element of the tree, it only contains a string that can be used as header in the view
    std::unique_ptr<Element> root_element;
    // The following two members are helper pointers to easily access the elements directly below the root elements
    // This is a pointer to the element that is responsible for holding the diagrams that were loaded from files
    Element* files_element;
    // This is a pointer to the element that is responsible for holding the diagrams that were received trough the network connetions
    Element* network_element;
};

#endif // DIAGRAM_CONTAINER_HPP
