#include "diagram_container.h"



DiagramContainer::DiagramContainer(QObject* parent) : QAbstractItemModel(parent)
{
    root_item = std::make_unique<Element>("Available diagrams");
    root_item->CreateChild("Diagrams loaded from files").CreateChild("No diagram was loaded yet...");
    root_item->CreateChild("Diagrams received on the network").CreateChild("No diagram was received yet...");

#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
    std::cout << "Elements contained by the DiagramContainer:" << std::endl;
    root_item->PrintAllElementsBelow();
    std::cout << std::endl;
#endif
}

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
        parent_element = root_item.get();
    }

    std::size_t number_of_children = parent_element->GetNumberOfChildren();
    if(number_of_children > row)
    {
        if(column_count > column)
        {
            Element* child_element = parent_element->GetChildAddress(row);
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
        Element* parent_element = indexed_element->GetParent();
        if(root_item.get() != parent_element)
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
        result = root_item->GetNumberOfChildren();
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
        result = QVariant(QString::fromStdString(root_item->GetDisplayableString()));
    }

    return result;
}

DiagramContainer::Element& DiagramContainer::Element::CreateChild(const std::string& childs_name)
{
    children.push_back(std::make_unique<Element>(childs_name, this));
    return *children.at(children.size() - 1);
}

DiagramContainer::Element* DiagramContainer::Element::GetChildAddress(const std::size_t& index)
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

template <typename T> std::size_t DiagramContainer::Element::CountElementsBelowWithType(void) const
{
    std::size_t result = 0;

    for(const auto& i : children)
    {
        result += i->CountElementsBelowWithType<T>();
    }

    return result;
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
