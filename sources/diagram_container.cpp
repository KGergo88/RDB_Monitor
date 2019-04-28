#include "diagram_container.h"



DiagramContainer::DiagramContainer(QObject* parent) : QAbstractItemModel(parent)
{
    // Creating the root element
    root_element = std::make_unique<Element>(root_element_data);

    // Creating the element that holds the diagrams from the files with an empty element
    files_element = root_element->CreateChild(files_element_data);
    files_element->CreateChild(empty_element_data);

    // Creating the element that holds the diagrams from the network with an empty element
    network_element = root_element->CreateChild(network_element_data);
    network_element->CreateChild(empty_element_data);

#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
    std::cout << "Elements contained by the DiagramContainer:" << std::endl;
    root_item->PrintAllElementsBelow();
    std::cout << std::endl;
#endif
}

void DiagramContainer::AddDiagramFromFile(const std::string &file_path, const DiagramSpecialized &diagram)
{
    AddDiagramToElement(files_element, file_path, diagram);
}

void DiagramContainer::AddDiagramFromNetwork(const std::string& connection_name, const DiagramSpecialized& diagram)
{
    AddDiagramToElement(network_element, connection_name, diagram);
}

void DiagramContainer::AddDiagramToElement(Element* element, const std::string& file_or_connection_name, const DiagramSpecialized &diagram)
{
    // If the empty element is still here then we remove it
    Element* empty_element = element->GetChildWithNameEntry(empty_element_data);
    if(nullptr != empty_element)
    {
        RemoveChildFromElement(element, empty_element);
    }

    // Looking for the sub-element that belongs to this file or connection and creating it if it does not exists
    Element* sub_element = element->GetChildWithNameEntry(file_or_connection_name);
    if(nullptr == sub_element)
    {
        sub_element = AddChildToElement(element, file_or_connection_name);
    }

    // Adding the diagram to the element that belongs to this file
    AddChildToElement(sub_element, diagram);
}

QModelIndex DiagramContainer::GetModelIndexOfElement(Element *element) const
{
    QModelIndex result;
    std::size_t index_of_element;

    if(element->parent->GetIndexOfChild(element, index_of_element))
    {
        result = createIndex(index_of_element, (column_count - 1), element);
    }

    return result;
}

DiagramContainer::Element* DiagramContainer::AddChildToElement(Element* element, Element::DataType data)
{
    Element* result = nullptr;

    beginInsertRows(GetModelIndexOfElement(element), element->GetNumberOfChildren(), (element->GetNumberOfChildren()));
    result = element->CreateChild(data);
    endInsertRows();

    return result;
}

void DiagramContainer::RemoveChildFromElement(Element* element, Element* child)
{
    std::size_t index_of_child;
    if(element->GetIndexOfChild(child, index_of_child))
    {
        beginRemoveRows(GetModelIndexOfElement(element), index_of_child, index_of_child);
        element->KillChild(index_of_child);
        endRemoveRows();
    }
}



//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------



QModelIndex DiagramContainer::index(int row, int column, const QModelIndex &parent) const
{
#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
    std::cout << "DiagramContainer::index was called!"
              << " row: " << row
              << " column: " << column
              << " parent.internalPointer: " << parent.internalPointer()
              << " parent.isValid: " << parent.isValid()
              << " parent.row: " << parent.row()
              << " parent.column: " << parent.column()
              << std::endl;
#endif

    QModelIndex result;
    Element* parent_element;

    if(parent.isValid())
    {
        parent_element = static_cast<Element*>(parent.internalPointer());
    }
    else
    {
        parent_element = root_element.get();
    }

    std::size_t number_of_children = parent_element->GetNumberOfChildren();
    if(number_of_children > row)
    {
        if(column_count > column)
        {
            Element* child_element = parent_element->GetChildWithIndex(row);
            result = createIndex(row, column, child_element);
        }
    }

    return result;
}

QModelIndex DiagramContainer::parent(const QModelIndex &index) const
{
#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
    std::cout << "DiagramContainer::parent was called!"
              << " index.internalPointer: " << index.internalPointer()
              << " index.isValid: " << index.isValid()
              << " index.row: " << index.row()
              << " index.column: " << index.column()
              << std::endl;
#endif

    QModelIndex result;

    if(index.isValid())
    {
        Element* indexed_element = static_cast<Element*>(index.internalPointer());
        Element* parent_element = indexed_element->parent;
        if(root_element.get() != parent_element)
        {
            result = createIndex(0, (column_count - 1), parent_element);
        }
    }

    return result;
}

int DiagramContainer::rowCount(const QModelIndex &parent) const
{
#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
    std::cout << "DiagramContainer::rowCount was called!"
              << " parent.internalPointer: " << parent.internalPointer()
              << " parent.isValid: " << parent.isValid()
              << " parent.row: " << parent.row()
              << " parent.column: " << parent.column()
              << std::endl;
#endif

    int result = 0;

    if(parent.isValid())
    {
        result = static_cast<Element*>(parent.internalPointer())->GetNumberOfChildren();
    }
    else
    {
        result = root_element->GetNumberOfChildren();
    }

    return result;
}

int DiagramContainer::columnCount(const QModelIndex &parent) const
{
#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
    std::cout << "DiagramContainer::columnCount was called!"
              << " parent.internalPointer: " << parent.internalPointer()
              << " parent.isValid: " << parent.isValid()
              << " parent.row: " << parent.row()
              << " parent.column: " << parent.column()
              << std::endl;
#endif

    (void) parent;

    return column_count;
}

QVariant DiagramContainer::data(const QModelIndex &index, int role) const
{
#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
    std::cout << "DiagramContainer::columnCount was called!"
              << " index.internalPointer: " << index.internalPointer()
              << " index.isValid: " << index.isValid()
              << " index.row: " << index.row()
              << " index.column: " << index.column()
              << " role: " << role
              << std::endl;
#endif

    QVariant result;

    if(index.isValid())
    {
        Element* element = static_cast<Element*>(index.internalPointer());
        switch(role)
        {
        case Qt::DisplayRole:
            result = QVariant(QString::fromStdString(element->GetDisplayableString()));
            break;
        default:
            break;
        }
    }

    return result;
}

QVariant DiagramContainer::headerData(int section, Qt::Orientation orientation, int role) const
{
#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
    std::cout << "DiagramContainer::headerData was called!"
              << " section: " << section
              << " orientation: " << orientation
              << " role: " << role
              << std::endl;
#endif

    (void) section;
    QVariant result;

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        result = QVariant(QString::fromStdString(root_element->GetDisplayableString()));
    }

    return result;
}



//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------



DiagramContainer::Element* DiagramContainer::Element::CreateChild(const DataType& childs_data)
{
    children.push_back(std::make_unique<Element>(childs_data, this));
    return children.at(children.size() - 1).get();
}

DiagramContainer::Element* DiagramContainer::Element::GetChildWithIndex(const std::size_t& index)
{
    Element* result = nullptr;
    if(index < children.size())
    {
        result = children.at(index).get();
    }

    return result;
}

DiagramContainer::Element* DiagramContainer::Element::GetChildWithNameEntry(const DataType_Name& name_to_look_for)
{
    Element* result = nullptr;

    for(const auto& i: children)
    {
        if(i->ContainsType<DataType_Name>())
        {
            if(std::get<DataType_Name>(i->data) == name_to_look_for)
            {
                result = i.get();
                break;
            }
        }
    }

    return result;
}

bool DiagramContainer::Element::GetIndexOfChild(const Element* child, std::size_t& index_of_child)
{
    bool bResult = false;

    for(std::size_t i = 0; i < children.size(); i++)
    {
        if(children[i].get() == child)
        {
            index_of_child = i;
            bResult = true;
            break;
        }
    }

    return bResult;
}

std::string DiagramContainer::Element::GetDisplayableString(void) const
{
    std::string result;

    if(std::holds_alternative<Element::DataType_Name>(data))
    {
        result = std::get<Element::DataType_Name>(data);
    }
    else if(std::holds_alternative<Element::DataType_Diagram>(data))
    {
        result = std::get<Element::DataType_Diagram>(data).GetTitle();
    }
    else
    {
        std::string errorMessage = "The DiagramContainer::data contains neither a \"Element::DataType_Name\" nor a \"Element::DataType_Diagram\".";
        throw errorMessage;
    }

    return result;
}

#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
void DiagramContainer::Element::PrintAllElementsBelow(const std::string& identation) const
{
    std::cout << identation << "Name: \"" << name << "\", Address: " << this << std::endl;

    for(const auto& i : children)
    {
        i->PrintAllElementsBelow(identation + identation);
    }
}
#endif
