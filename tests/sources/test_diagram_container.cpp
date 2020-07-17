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



TEST(TestDiagramContainer, ConstructorAndIndexing)
{
    // Model structure should be
    // root_element ("Available digrams")
    //     |
    //     --- files_element_data ("Diagrams loaded from files")
    //     |       |
    //     |       --- empty_element_data ("No diagrams yet...")
    //     |
    //     --- network_element_data ("Diagrams received on the network")
    //     |       |
    //     |       --- empty_element_data ("No diagrams yet...")

    DiagramContainer container{};
    QModelIndex root = QModelIndex{};
    QModelIndex first_child = container.index(0,0,root);
    QModelIndex first_child_of_first_child = container.index(0,0,first_child);
    QModelIndex non_existent_second_child_of_first_child = container.index(1,0,first_child);
    QModelIndex second_child = container.index(1,0,root);
    QModelIndex first_child_of_second_child = container.index(0,0,second_child);
    QModelIndex non_existent_second_child_of_second_child = container.index(1,0,second_child);
    QModelIndex non_existent_third_child = container.index(2,0,root);

    EXPECT_EQ(first_child.row(), 0);
    EXPECT_EQ(first_child.column(), 0);
    EXPECT_EQ(first_child_of_first_child.row(), 0);
    EXPECT_EQ(first_child_of_first_child.column(), 0);
    EXPECT_EQ(non_existent_second_child_of_first_child.row(), -1);
    EXPECT_EQ(non_existent_second_child_of_first_child.column(), -1);
    EXPECT_TRUE(first_child.isValid());
    EXPECT_TRUE(first_child_of_first_child.isValid());
    EXPECT_FALSE(non_existent_second_child_of_first_child.isValid());

    EXPECT_EQ(second_child.row(), 1);
    EXPECT_EQ(second_child.column(), 0);
    EXPECT_EQ(first_child_of_second_child.row(), 0);
    EXPECT_EQ(first_child_of_second_child.column(), 0);
    EXPECT_EQ(non_existent_second_child_of_second_child.row(), -1);
    EXPECT_EQ(non_existent_second_child_of_second_child.column(), -1);
    EXPECT_TRUE(second_child.isValid());
    EXPECT_TRUE(first_child_of_second_child.isValid());
    EXPECT_FALSE(non_existent_second_child_of_second_child.isValid());

    EXPECT_EQ(non_existent_second_child_of_second_child.row(), -1);
    EXPECT_EQ(non_existent_second_child_of_second_child.column(), -1);
    EXPECT_FALSE(non_existent_third_child.isValid());

    EXPECT_EQ(first_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(second_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_child_of_first_child.flags(), Qt::ItemIsEnabled);
    EXPECT_EQ(first_child_of_second_child.flags(), Qt::ItemIsEnabled);
}