#include "diagram_container.hpp"



// --- Static variable definitions of the DiagramContainer class ----------------------------------------------------------------------------------------------------------------------------------------------------------

const DiagramContainer::Element::DataType_Name DiagramContainer::root_element_data("Available diagrams");
const DiagramContainer::Element::DataType_Name DiagramContainer::files_element_data("Diagrams loaded from files");
const DiagramContainer::Element::DataType_Name DiagramContainer::network_element_data("Diagrams received on the network");
const DiagramContainer::Element::DataType_Name DiagramContainer::empty_element_data("No diagrams yet...");

// --- Methods of the DiagramContainer class ----------------------------------------------------------------------------------------------------------------------------------------------------------

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

QModelIndex DiagramContainer::AddDiagramFromFile(const std::string file_name, const std::string& file_path, const DiagramSpecialized& diagram)
{
    // If the empty element is still here then we remove it
    Element* empty_element = files_element->GetChildWithData(empty_element_data);
    if(nullptr != empty_element)
    {
        RemoveChildFromElement(files_element, empty_element);
    }

    // Looking for the file name element that contains the diagrams of this file and creating it if it does not exists
    Element::DataType_File file_name_element_data(file_name, file_path);
    Element* file_name_element = files_element->GetChildWithData(file_name_element_data);
    if(nullptr == file_name_element)
    {
        file_name_element = AddChildToElement(files_element, file_name_element_data);
    }

    // Adding the diagram to the element that represents this file
    Element* new_diagram_element = AddChildToElement(file_name_element, diagram);

    return GetModelIndexOfElement(new_diagram_element);
}

bool DiagramContainer::IsThisFileAlreadyStored(const std::string& file_name, const std::string& file_path)
{
    bool bResult = false;

    if(nullptr != files_element->GetChildWithData(Element::DataType_File(file_name, file_path)))
    {
        bResult = true;
    }

    return bResult;
}

DiagramSpecialized* DiagramContainer::GetDiagram(const QModelIndex& model_index)
{
    DiagramSpecialized* result = nullptr;

    if(model_index.isValid())
    {
        Element* requested_element = static_cast<Element*>(model_index.internalPointer());
        if(requested_element->ContainsType<Element::DataType_Diagram>())
        {
            result = &std::get<DiagramSpecialized>(requested_element->data);
        }
    }

    return result;
}

void DiagramContainer::ShowCheckBoxes(void)
{
    root_element->SetCheckStateRecursive(Qt::CheckState::Unchecked);
    root_element->SetFlagsRecursive(Element::element_flags_checkable);
}

void DiagramContainer::HideCheckBoxes(void)
{
    root_element->SetFlagsRecursive(Element::element_flags_default);
}

std::vector<DiagramSpecialized> DiagramContainer::GetCheckedDiagrams(void)
{
    std::vector<DiagramSpecialized> checked_diagrams;

    root_element->CallFunctionOnElementsRecursive(
    [&](Element* element) -> void
    {
       if(element->check_state)
       {
           if(element->ContainsType<Element::DataType_Diagram>())
           {
               checked_diagrams.push_back(std::get<Element::DataType_Diagram>(element->data));
           }
       }
    });

    return checked_diagrams;
}

QModelIndex DiagramContainer::GetModelIndexOfElement(Element *element) const
{
    QModelIndex result;
    std::size_t index_of_element;

    if(element->parent->GetIndexWithChild(element, index_of_element))
    {
        result = createIndex(index_of_element, (column_count - 1), element);
    }

    return result;
}

DiagramContainer::Element* DiagramContainer::AddChildToElement(Element* element, const Element::DataType& data)
{
    Element* result = nullptr;

    // Notifying the views that an insertion will happen
    beginInsertRows(GetModelIndexOfElement(element), element->GetNumberOfChildren(), (element->GetNumberOfChildren()));


    // The childs check_state will be inherited from the parent in a way to respect the tri-state checkedness
    Qt::CheckState childs_check_state;
    if(Qt::CheckState::Checked == element->check_state)
    {
        childs_check_state = Qt::CheckState::Checked;
    }
    else
    {
        childs_check_state = Qt::CheckState::Unchecked;
    }

    // Creating the child (The childs flags are simply inherited from the parent)
    result = element->CreateChild(data, element->flags, childs_check_state);

    // Notifying the views that an insertion just happened
    endInsertRows();

    return result;
}

void DiagramContainer::RemoveChildFromElement(Element* element, Element* child)
{
    std::size_t index_of_child;

    // Checking whether the child exists
    if(element->GetIndexWithChild(child, index_of_child))
    {
        // Notifying the views that a removal will happen
        beginRemoveRows(GetModelIndexOfElement(element), index_of_child, index_of_child);

        // Removing the selected child
        element->KillChild(index_of_child);

        // Notifying the views that a removal just happened
        endRemoveRows();
    }
}

void DiagramContainer::SetCheckStateOfElement(Element* element, const Qt::CheckState& new_check_state)
{
    // Setting the new check state for this element and all the elements below this
    element->SetCheckStateRecursive(new_check_state);

    // Root elements do not have a parent and if there is no parent to notify then there is nothing to do here
    if(!element->IsRoot())
    {
        element->parent->ChildsCheckStateHasChanged();
    }

    // Notifying the views about the check_state changes
    // For the sake of simplicity, we are signaling a change for all the elements with the invalid QModelIndex
    // If this will cause a performance issue, then a more complex solution can be introduced that sends notifications only for the changed elements,
    // but at the moment this case seams to be highly unlikely because the number of Elements stored in the DiagramContainer are relatively low
    QVector<int> role;
    role.append(Qt::CheckStateRole);
    emit dataChanged(QModelIndex(), QModelIndex(), role);
}

// --- Methods of the DiagramContainer class that override the methods of the QAbstractItemModel ------------------------------------------------------------------------------------------------------

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
        Element* indexed_element = static_cast<Element*>(parent.internalPointer());
        result = indexed_element->GetNumberOfChildren();
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

    // The columnCount has a fixed value in the DiagramContainer, which is defined by the column_count
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
        case Qt::CheckStateRole:
            // We will only return a valid QVariant if the element is checkable
            // This is needed, because otherwise a checkbox would be present for non-checkable elements as well
            if(element->flags & Qt::ItemIsUserCheckable)
            {
                result = QVariant(element->check_state);
            }
            break;
        default:
            // Nothing to do here, an invalid QVariant will be returned...
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

Qt::ItemFlags DiagramContainer::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result;

    if(index.isValid())
    {
        Element* indexed_element = static_cast<Element*>(index.internalPointer());
        result = indexed_element->flags;
    }
    else
    {
        result = root_element->flags;
    }

    return result;
}

bool DiagramContainer::setData(const QModelIndex &index, const QVariant &value, int role)
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

    bool bResult = false;

    if(index.isValid())
    {
        Element* element = static_cast<Element*>(index.internalPointer());

        switch(role)
        {
        case Qt::CheckStateRole:
            // We only enable setting for elements that are checkable
            if(element->flags & Qt::ItemIsUserCheckable)
            {
                if(value.isValid())
                {
                    SetCheckStateOfElement(element, *static_cast<const Qt::CheckState*>(value.data()));
                    bResult = true;
                }
            }
            break;
        default:
            // Nothing to do here, false will be returned...
            break;
        }
    }

    return bResult;
}

// --- Methods functions of the DiagramContainer::Element class ---------------------------------------------------------------------------------------------------------------------------------------

DiagramContainer::Element* DiagramContainer::Element::CreateChild(const DataType& childs_data, const Qt::ItemFlags& childs_flags, const Qt::CheckState& childs_check_state)
{
    children.push_back(std::make_unique<Element>(childs_data, this, childs_flags, childs_check_state));
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

bool DiagramContainer::Element::GetIndexWithChild(const Element* child, std::size_t& index_of_child)
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
    else if(std::holds_alternative<Element::DataType_File>(data))
    {
        result = std::get<Element::DataType_File>(data).name;
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

void DiagramContainer::Element::SetFlagsRecursive(const Qt::ItemFlags& new_flags)
{
    // Setting the flags for this element
    flags = new_flags;

    // Going trough all the children and calling the same function on them as well
    for(const auto& i : children)
    {
        i->SetFlagsRecursive(new_flags);
    }
}

void DiagramContainer::Element::SetCheckStateRecursive(const Qt::CheckState& new_check_state)
{
    // Setting the check_state for this element
    check_state = new_check_state;

    // Going trough all the children and calling the same function on them as well
    for(const auto& i : children)
    {
        i->SetCheckStateRecursive(new_check_state);
    }
}

void DiagramContainer::Element::ChildsCheckStateHasChanged(void)
{
    // It does not make sense to call this function on an element that does not have a child,
    // but just to be on the safe side, we will check it to avoid invalid access to the children container
    if(0 < GetNumberOfChildren())
    {
        // We say that the accumulated check state is the check state of the first child
        // If the other children has a different check_state then we will change this to partially checked
        // This makes sense, since the check_state can have only the following values: Unchecked, PartiallyChecked, Checked
        Qt::CheckState accumulated_check_state_of_the_children = children[0]->check_state;
        for(const auto& i : children)
        {
            if(accumulated_check_state_of_the_children != i->check_state)
            {
                accumulated_check_state_of_the_children = Qt::CheckState::PartiallyChecked;
            }
        }

        // If the accumulated check state of the children is the same that this element has, then there is nothing to do
        if(check_state != accumulated_check_state_of_the_children)
        {
            // We need to make sure that the new chec_state is set before notifying the parent about the changes
            // This is needed because the parent will also take into account of this element's check_state
            check_state = accumulated_check_state_of_the_children;
            if(!IsRoot())
            {
                parent->ChildsCheckStateHasChanged();
            }
        }
    }
}

void DiagramContainer::Element::CallFunctionOnElementsRecursive(std::function<void(Element*)> function)
{
    // Calling the function on this element
    function(this);

    // Calling this function on all the children
    for(const auto& i : children)
    {
        i->CallFunctionOnElementsRecursive(function);
    }
}

#ifdef DIAGRAM_CONTAINER_DEBUG_MODE
void DiagramContainer::Element::PrintAllElementsRecursive(const std::string& identation) const
{
    std::cout << identation << "Name: \"" << name << "\", Address: " << this << std::endl;

    for(const auto& i : children)
    {
        i->PrintAllElementsRecursive(identation + identation);
    }
}
#endif
