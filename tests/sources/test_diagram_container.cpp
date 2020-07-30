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



#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QSignalSpy>
#include "../application/sources/diagram_container.hpp"


// Constructed model structure should be...
//
// root ("Available digrams")
//     |
//     --- first_child ("Diagrams loaded from files")
//     |       |
//     |       --- first_child_of_first_child ("No diagrams yet...")
//     |
//     --- second_child ("Diagrams received on the network")
//             |
//             --- first_child_of_second_child ("No diagrams yet...")

class TestDiagramContainer : public ::testing::Test
{
protected:
    DiagramContainer container;
    QModelIndex root;
    QModelIndex first_child;
    QModelIndex first_child_of_first_child;
    QModelIndex second_child;
    QModelIndex first_child_of_second_child;

    TestDiagramContainer() {}
    ~TestDiagramContainer() override {}
    void SetUp() override
    {
        root = QModelIndex{};
        first_child = container.index(0,0,root);
        first_child_of_first_child = container.index(0,0,first_child);
        second_child = container.index(1,0,root);
        first_child_of_second_child = container.index(0,0,second_child);
    }
    void TearDown() override {}
};

TEST_F(TestDiagramContainer, ModelStructureAndIndexingAfterConstruction)
{
    EXPECT_EQ(first_child.row(), 0);
    EXPECT_EQ(first_child.column(), 0);
    EXPECT_TRUE(first_child.isValid());
    EXPECT_EQ(first_child_of_first_child.row(), 0);
    EXPECT_EQ(first_child_of_first_child.column(), 0);
    EXPECT_TRUE(first_child_of_first_child.isValid());

    EXPECT_EQ(second_child.row(), 1);
    EXPECT_EQ(second_child.column(), 0);
    EXPECT_TRUE(second_child.isValid());
    EXPECT_EQ(first_child_of_second_child.row(), 0);
    EXPECT_EQ(first_child_of_second_child.column(), 0);
    EXPECT_TRUE(first_child_of_second_child.isValid());

    QModelIndex non_existent_second_child_of_first_child = container.index(1,0,first_child);
    QModelIndex non_existent_second_child_of_second_child = container.index(1,0,second_child);
    QModelIndex non_existent_third_child = container.index(2,0,root);

    EXPECT_EQ(non_existent_second_child_of_first_child.row(), -1);
    EXPECT_EQ(non_existent_second_child_of_first_child.column(), -1);
    EXPECT_FALSE(non_existent_second_child_of_first_child.isValid());
    EXPECT_EQ(non_existent_second_child_of_second_child.row(), -1);
    EXPECT_EQ(non_existent_second_child_of_second_child.column(), -1);
    EXPECT_FALSE(non_existent_second_child_of_second_child.isValid());
    EXPECT_EQ(non_existent_third_child.row(), -1);
    EXPECT_EQ(non_existent_third_child.column(), -1);
    EXPECT_FALSE(non_existent_third_child.isValid());
}

TEST_F(TestDiagramContainer, ParentReferencing)
{
    EXPECT_EQ(container.parent(root), QModelIndex{});
    EXPECT_EQ(container.parent(first_child), root);
    EXPECT_EQ(container.parent(second_child), root);
    EXPECT_EQ(container.parent(first_child_of_first_child), first_child);
    EXPECT_EQ(container.parent(first_child_of_second_child), second_child);
}

TEST_F(TestDiagramContainer, RowAndColumnCountAfterConstruction)
{
    EXPECT_EQ(container.rowCount(root), 2);
    EXPECT_EQ(container.columnCount(root), 1);
    EXPECT_EQ(container.rowCount(first_child), 1);
    EXPECT_EQ(container.columnCount(first_child), 1);
    EXPECT_EQ(container.rowCount(second_child), 1);
    EXPECT_EQ(container.columnCount(second_child), 1);
    EXPECT_EQ(container.rowCount(first_child_of_first_child), 0);
    EXPECT_EQ(container.columnCount(first_child_of_first_child), 1);
    EXPECT_EQ(container.rowCount(first_child_of_second_child), 0);
    EXPECT_EQ(container.columnCount(first_child_of_second_child), 1);
}

TEST_F(TestDiagramContainer, FlagsAfterConstruction)
{
    EXPECT_EQ(root.flags(), Qt::NoItemFlags);
    EXPECT_EQ(first_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(second_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_child_of_first_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_child_of_second_child.flags(), Qt::ItemIsEnabled);
}

TEST_F(TestDiagramContainer, DataAfterConstruction)
{
    EXPECT_EQ(container.headerData(0, Qt::Horizontal, Qt::DisplayRole).value<QString>(), QString("Available diagrams"));
    EXPECT_FALSE(container.headerData(0, Qt::Vertical, Qt::DisplayRole).isValid());
    EXPECT_FALSE(container.headerData(0, Qt::Horizontal, Qt::CheckStateRole).isValid());

    EXPECT_EQ(container.data(first_child, Qt::DisplayRole).value<QString>(), QString("Diagrams loaded from files"));
    EXPECT_EQ(container.data(first_child, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);

    EXPECT_EQ(container.data(second_child, Qt::DisplayRole).value<QString>(), QString("Diagrams received on the network"));
    EXPECT_EQ(container.data(second_child, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);

    EXPECT_EQ(container.data(first_child_of_first_child, Qt::DisplayRole).value<QString>(), QString("No diagrams yet..."));
    EXPECT_EQ(container.data(first_child_of_first_child, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);

    EXPECT_EQ(container.data(first_child_of_second_child, Qt::DisplayRole).value<QString>(), QString("No diagrams yet..."));
    EXPECT_EQ(container.data(first_child_of_second_child, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
}

TEST_F(TestDiagramContainer, ShowHideCheckBoxes)
{
    container.ShowCheckBoxes();
    EXPECT_EQ(root.flags(), Qt::NoItemFlags);
    EXPECT_EQ(first_child.flags(), Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    EXPECT_EQ(second_child.flags(), Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    EXPECT_EQ(first_child_of_first_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_child_of_second_child.flags(), Qt::ItemIsEnabled);

    container.HideCheckBoxes();
    EXPECT_EQ(root.flags(), Qt::NoItemFlags);
    EXPECT_EQ(first_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(second_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_child_of_first_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_child_of_second_child.flags(), Qt::ItemIsEnabled);
}

TEST_F(TestDiagramContainer, ChangingCheckStateOfItem)
{
    container.ShowCheckBoxes();
    QSignalSpy SpyDataChangedSignal(&container, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)));

    EXPECT_TRUE(container.setData(first_child, QVariant{Qt::Checked}, Qt::CheckStateRole));
    EXPECT_EQ(container.data(first_child, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked);
    EXPECT_EQ(SpyDataChangedSignal.count(), 1);

    EXPECT_TRUE(container.setData(second_child, QVariant{Qt::Checked}, Qt::CheckStateRole));
    EXPECT_EQ(container.data(second_child, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked);
    EXPECT_EQ(SpyDataChangedSignal.count(), 2);

    EXPECT_FALSE(container.setData(first_child_of_first_child, QVariant{Qt::Checked}, Qt::CheckStateRole));
    EXPECT_FALSE(container.setData(first_child_of_second_child, QVariant{Qt::Checked}, Qt::CheckStateRole));
    EXPECT_EQ(SpyDataChangedSignal.count(), 2);
}

class TestDiagramContainerWithDiagrams : public ::testing::Test
{
protected:
    DiagramContainer container;

    QModelIndex diagrams_from_file;
    QModelIndex diagrams_from_first_file;
    QModelIndex first_diagram_from_first_file;
    QModelIndex second_diagram_from_first_file;
    QModelIndex diagrams_from_second_file;
    QModelIndex first_diagram_from_second_file;

    QModelIndex diagrams_from_network;
    QModelIndex diagrams_from_first_network;
    QModelIndex first_diagram_from_first_network;
    QModelIndex second_diagram_from_first_network;
    QModelIndex diagrams_from_second_network;
    QModelIndex first_diagram_from_second_network;

    const std::string file_path{"file_path"};
    const std::string first_file_name{"first_file_name"};
    const std::string second_file_name{"second_file_name"};
    const std::string first_connection_name{"first_connection_name"};
    const std::string second_connection_name{"second_connection_name"};
    const std::string title{"Title"};
    const std::string axis_x{"Axis X"};

    DiagramSpecialized default_diagram;
    DiagramSpecialized diagram{title, axis_x};

    TestDiagramContainerWithDiagrams() {}
    ~TestDiagramContainerWithDiagrams() override {}
    void SetUp() override
    {
        diagrams_from_file = container.index(0,0,QModelIndex{});
        first_diagram_from_first_file = container.AddDiagramFromFile(first_file_name, file_path, default_diagram);
        second_diagram_from_first_file = container.AddDiagramFromFile(first_file_name, file_path, diagram);
        first_diagram_from_second_file = container.AddDiagramFromFile(second_file_name, file_path, diagram);
        diagrams_from_first_file = container.index(0,0,diagrams_from_file);
        diagrams_from_second_file = container.index(1,0,diagrams_from_file);
        
        diagrams_from_network = container.index(1,0, QModelIndex{});
        first_diagram_from_first_network = container.AddDiagramFromNetwork(first_connection_name, default_diagram);
        second_diagram_from_first_network = container.AddDiagramFromNetwork(first_connection_name, diagram);
        first_diagram_from_second_network = container.AddDiagramFromNetwork(second_connection_name, diagram);
        diagrams_from_first_network = container.index(0,0,diagrams_from_network);
        diagrams_from_second_network = container.index(1,0,diagrams_from_network);
    }
    void TearDown() override {}
    void UncheckAllFileElements()
    {
        container.setData(diagrams_from_file, QVariant{Qt::Unchecked}, Qt::CheckStateRole);
        container.setData(diagrams_from_first_file, QVariant{Qt::Unchecked}, Qt::CheckStateRole);
        container.setData(first_diagram_from_first_file, QVariant{Qt::Unchecked}, Qt::CheckStateRole);
        container.setData(second_diagram_from_first_file, QVariant{Qt::Unchecked}, Qt::CheckStateRole);
        container.setData(diagrams_from_second_file, QVariant{Qt::Unchecked}, Qt::CheckStateRole);
        container.setData(first_diagram_from_second_file, QVariant{Qt::Unchecked}, Qt::CheckStateRole);
    }
};

TEST_F(TestDiagramContainerWithDiagrams, HierarchyAfterAddingDiagrams)
{
    EXPECT_TRUE(diagrams_from_file.isValid());
    EXPECT_TRUE(diagrams_from_first_file.isValid());
    EXPECT_TRUE(first_diagram_from_first_file.isValid());
    EXPECT_TRUE(second_diagram_from_first_file.isValid());
    EXPECT_TRUE(diagrams_from_second_file.isValid());
    EXPECT_TRUE(first_diagram_from_second_file.isValid());

    EXPECT_TRUE(diagrams_from_network.isValid());
    EXPECT_TRUE(diagrams_from_first_network.isValid());
    EXPECT_TRUE(first_diagram_from_first_network.isValid());
    EXPECT_TRUE(second_diagram_from_first_network.isValid());
    EXPECT_TRUE(diagrams_from_second_network.isValid());
    EXPECT_TRUE(first_diagram_from_second_network.isValid());

    EXPECT_EQ(diagrams_from_first_file, container.index(0,0,diagrams_from_file));
    EXPECT_EQ(first_diagram_from_first_file, container.index(0,0,diagrams_from_first_file));
    EXPECT_EQ(second_diagram_from_first_file, container.index(1,0,diagrams_from_first_file));
    EXPECT_EQ(diagrams_from_second_file, container.index(1,0,diagrams_from_file));
    EXPECT_EQ(first_diagram_from_second_file, container.index(0,0,diagrams_from_second_file));

    EXPECT_EQ(diagrams_from_first_network, container.index(0,0,diagrams_from_network));
    EXPECT_EQ(first_diagram_from_first_network, container.index(0,0,diagrams_from_first_network));
    EXPECT_EQ(second_diagram_from_first_network, container.index(1,0,diagrams_from_first_network));
    EXPECT_EQ(diagrams_from_second_network, container.index(1,0,diagrams_from_network));
    EXPECT_EQ(first_diagram_from_second_network, container.index(0,0,diagrams_from_second_network));
}

TEST_F(TestDiagramContainerWithDiagrams, FlagsAndDataAfterAddingDiagrams)
{
    EXPECT_EQ(container.data(diagrams_from_first_file, Qt::DisplayRole).value<QString>(), QString::fromStdString(first_file_name));
    EXPECT_EQ(container.data(first_diagram_from_first_file, Qt::DisplayRole).value<QString>(), QString{});
    EXPECT_EQ(container.data(second_diagram_from_first_file, Qt::DisplayRole).value<QString>(), QString::fromStdString(title));
    EXPECT_EQ(container.data(diagrams_from_second_file, Qt::DisplayRole).value<QString>(),QString::fromStdString(second_file_name));
    EXPECT_EQ(container.data(first_diagram_from_second_file, Qt::DisplayRole).value<QString>(), QString::fromStdString(title));

    EXPECT_EQ(container.data(diagrams_from_first_network, Qt::DisplayRole).value<QString>(), QString::fromStdString(first_connection_name));
    EXPECT_EQ(container.data(first_diagram_from_first_network, Qt::DisplayRole).value<QString>(), QString{});
    EXPECT_EQ(container.data(second_diagram_from_first_network, Qt::DisplayRole).value<QString>(), QString::fromStdString(title));
    EXPECT_EQ(container.data(diagrams_from_second_network, Qt::DisplayRole).value<QString>(), QString::fromStdString(second_connection_name));
    EXPECT_EQ(container.data(first_diagram_from_second_network, Qt::DisplayRole).value<QString>(), QString::fromStdString(title));

    EXPECT_EQ(container.data(diagrams_from_first_file, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    EXPECT_EQ(container.data(first_diagram_from_first_file, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    EXPECT_EQ(container.data(second_diagram_from_first_file, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    EXPECT_EQ(container.data(diagrams_from_second_file, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    EXPECT_EQ(container.data(first_diagram_from_second_file, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    
    EXPECT_EQ(container.data(diagrams_from_first_network, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    EXPECT_EQ(container.data(first_diagram_from_first_network, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    EXPECT_EQ(container.data(second_diagram_from_first_network, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    EXPECT_EQ(container.data(diagrams_from_second_network, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    EXPECT_EQ(container.data(first_diagram_from_second_network, Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);

    EXPECT_EQ(diagrams_from_first_file.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_diagram_from_first_file.flags(), Qt::ItemIsEnabled | Qt::ItemIsEditable);
    EXPECT_EQ(second_diagram_from_first_file.flags(), Qt::ItemIsEnabled | Qt::ItemIsEditable);
    EXPECT_EQ(diagrams_from_second_file.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_diagram_from_second_file.flags(), Qt::ItemIsEnabled | Qt::ItemIsEditable);

    EXPECT_EQ(diagrams_from_first_network.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_diagram_from_first_network.flags(), Qt::ItemIsEnabled | Qt::ItemIsEditable);
    EXPECT_EQ(second_diagram_from_first_network.flags(), Qt::ItemIsEnabled | Qt::ItemIsEditable);
    EXPECT_EQ(diagrams_from_second_network.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_diagram_from_second_network.flags(), Qt::ItemIsEnabled | Qt::ItemIsEditable);
}

TEST_F(TestDiagramContainerWithDiagrams, DiagramNumbersUpdated)
{
    EXPECT_EQ(container.GetNumberOfDiagrams(), std::size_t{6});
    
    container.AddDiagramFromFile(first_file_name, file_path, default_diagram);
    EXPECT_EQ(container.GetNumberOfDiagrams(), std::size_t{7});

    container.AddDiagramFromFile("new_file_name", "new_file_path", default_diagram);
    EXPECT_EQ(container.GetNumberOfDiagrams(), std::size_t{8});

    container.AddDiagramFromNetwork(first_connection_name, default_diagram);
    EXPECT_EQ(container.GetNumberOfDiagrams(), std::size_t{9});

    container.AddDiagramFromNetwork("new_connection_name", default_diagram);
    EXPECT_EQ(container.GetNumberOfDiagrams(), std::size_t{10});
}

TEST_F(TestDiagramContainerWithDiagrams, FileDuplication)
{
    EXPECT_TRUE(container.IsThisFileAlreadyStored(first_file_name, file_path));
    EXPECT_FALSE(container.IsThisFileAlreadyStored("new_file_name", "new_file_path"));
}

TEST_F(TestDiagramContainerWithDiagrams, GetDiagram)
{
    EXPECT_EQ(container.GetDiagram(first_diagram_from_first_file)->GetTitle(), std::string{});
    EXPECT_EQ(container.GetDiagram(first_diagram_from_first_file)->GetAxisXTitle(), std::string{});
    EXPECT_EQ(container.GetDiagram(second_diagram_from_first_file)->GetTitle(), title);
    EXPECT_EQ(container.GetDiagram(second_diagram_from_first_file)->GetAxisXTitle(), axis_x);
}

TEST_F(TestDiagramContainerWithDiagrams, GetCheckedDiagrams)
{
    container.ShowCheckBoxes();
    auto no_checked_diagrams = container.GetCheckedDiagrams();
    EXPECT_TRUE(no_checked_diagrams.empty());

    UncheckAllFileElements();
    EXPECT_TRUE(container.setData(second_diagram_from_first_file, QVariant{Qt::Checked}, Qt::CheckStateRole));
    auto one_daigram_checked = container.GetCheckedDiagrams();
    ASSERT_EQ(one_daigram_checked.size(), std::size_t{1});
    EXPECT_EQ(one_daigram_checked.at(0).GetTitle(), title);
    EXPECT_EQ(one_daigram_checked.at(0).GetAxisXTitle(), axis_x);

    UncheckAllFileElements();
    EXPECT_TRUE(container.setData(diagrams_from_first_file, QVariant{Qt::Checked}, Qt::CheckStateRole));
    auto first_file_checked = container.GetCheckedDiagrams();
    ASSERT_EQ(first_file_checked.size(), std::size_t{2});
    EXPECT_EQ(first_file_checked.at(0).GetTitle(), std::string{});
    EXPECT_EQ(first_file_checked.at(0).GetAxisXTitle(), std::string{});
    EXPECT_EQ(first_file_checked.at(1).GetTitle(), title);
    EXPECT_EQ(first_file_checked.at(1).GetAxisXTitle(), axis_x);

    UncheckAllFileElements();
    EXPECT_TRUE(container.setData(diagrams_from_file, QVariant{Qt::Checked}, Qt::CheckStateRole));
    auto diagrams_from_file_checked = container.GetCheckedDiagrams();
    ASSERT_EQ(diagrams_from_file_checked.size(), std::size_t{3});
    EXPECT_EQ(diagrams_from_file_checked.at(0).GetTitle(), std::string{});
    EXPECT_EQ(diagrams_from_file_checked.at(0).GetAxisXTitle(), std::string{});
    EXPECT_EQ(diagrams_from_file_checked.at(1).GetTitle(), title);
    EXPECT_EQ(diagrams_from_file_checked.at(1).GetAxisXTitle(), axis_x);
    EXPECT_EQ(diagrams_from_file_checked.at(2).GetTitle(), title);
    EXPECT_EQ(diagrams_from_file_checked.at(2).GetAxisXTitle(), axis_x);
}