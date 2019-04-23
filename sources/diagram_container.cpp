#include "diagram_container.h"



DiagramContainer::DiagramContainer(QObject* parent) : QAbstractItemModel(parent)
{
    root_item = std::make_unique<Element>("Available diagrams");
    root_item->CreateChild("Diagrams loaded from files").CreateChild("No diagram was loaded yet...");
    root_item->CreateChild("Diagrams received on the network").CreateChild("No diagram was received yet...");

    std::cout << "Elements contained by the DiagramContainer:" << std::endl;
    root_item->PrintAllElementsBelow();
    std::cout << std::endl;
}

QModelIndex DiagramContainer::index(int row, int column, const QModelIndex &parent) const
{
    std::cout << "DiagramContainer::index was called!"
              << " row: " << row
              << " column: " << column
              << " parent.internalPointer: " << parent.internalPointer()
              << " parent.isValid: " << parent.isValid()
              << " parent.row: " << parent.row()
              << " parent.column: " << parent.column()
              << std::endl;

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
    std::cout << "DiagramContainer::parent was called!"
              << " index.internalPointer: " << index.internalPointer()
              << " index.isValid: " << index.isValid()
              << " index.row: " << index.row()
              << " index.column: " << index.column()
              << std::endl;

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
    std::cout << "DiagramContainer::rowCount was called!"
              << " parent.internalPointer: " << parent.internalPointer()
              << " parent.isValid: " << parent.isValid()
              << " parent.row: " << parent.row()
              << " parent.column: " << parent.column()
              << std::endl;

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
    std::cout << "DiagramContainer::columnCount was called!"
              << " parent.internalPointer: " << parent.internalPointer()
              << " parent.isValid: " << parent.isValid()
              << " parent.row: " << parent.row()
              << " parent.column: " << parent.column()
              << std::endl;

    return column_count;
}

QVariant DiagramContainer::data(const QModelIndex &index, int role) const
{
    std::cout << "DiagramContainer::columnCount was called!"
              << " index.internalPointer: " << index.internalPointer()
              << " index.isValid: " << index.isValid()
              << " index.row: " << index.row()
              << " index.column: " << index.column()
              << " role: " << role
              << std::endl;

    QVariant result;

    if(index.isValid())
    {
        Element* element = static_cast<Element*>(index.internalPointer());
        switch(role)
        {
        case Qt::DisplayRole:
            result = QVariant(QString::fromStdString(element->name));
            break;
        default:
            break;
        }
    }

    return result;
}

QVariant DiagramContainer::headerData(int section, Qt::Orientation orientation, int role) const
{
    std::cout << "DiagramContainer::headerData was called!"
              << " section: " << section
              << " orientation: " << orientation
              << " role: " << role
              << std::endl;
    (void) section;
    QVariant result;

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        result = QVariant(QString::fromStdString(root_item->name));
    }

    return result;
}
