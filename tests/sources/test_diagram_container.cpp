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

#include "../application/sources/diagram_container.hpp"


// Model structure should be
// root ("Available digrams")
//     |
//     --- first_child ("Diagrams loaded from files")
//     |       |
//     |       --- first_child_of_first_child ("No diagrams yet...")
//     |
//     --- second_child ("Diagrams received on the network")
//             |
//             --- first_child_of_second_child ("No diagrams yet...")

class TestDiagramContainerClass : public ::testing::Test
{
protected:
    DiagramContainer container{};
    QModelIndex root = QModelIndex{};
    QModelIndex first_child = container.index(0,0,root);
    QModelIndex first_child_of_first_child = container.index(0,0,first_child);
    QModelIndex second_child = container.index(1,0,root);
    QModelIndex first_child_of_second_child = container.index(0,0,second_child);

    TestDiagramContainerClass() {  }
    ~TestDiagramContainerClass() override {  }
    void SetUp() override {  }
    void TearDown() override {  }
};


TEST_F(TestDiagramContainerClass, ModelStructureAndIndexingCorrectAfterConstruction)
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

TEST_F(TestDiagramContainerClass, ParentReferencingWorks)
{
    EXPECT_EQ(container.parent(root), QModelIndex{});
    EXPECT_EQ(container.parent(first_child), root);
    EXPECT_EQ(container.parent(second_child), root);
    EXPECT_EQ(container.parent(first_child_of_first_child), first_child);
    EXPECT_EQ(container.parent(first_child_of_second_child), second_child)
    << container.parent(first_child_of_second_child).internalId() << " | " << second_child.internalId() << "\n"
    << container.parent(first_child_of_second_child).row() << " | " << second_child.row() << "\n"
    << container.parent(first_child_of_second_child).column() << " | " << second_child.column() << "\n";
}

TEST_F(TestDiagramContainerClass, RowAndColumnCountCorrectAfterConstruction)
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

TEST_F(TestDiagramContainerClass, FlagsAreSetCorrectlyAtConstruction)
{
    EXPECT_EQ(root.flags(), Qt::NoItemFlags);
    EXPECT_EQ(first_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(second_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_child_of_first_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_child_of_second_child.flags(), Qt::ItemIsEnabled);
}