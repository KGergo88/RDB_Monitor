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


#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../application/sources/diagram.hpp"


using data_type = int;
using index_type = std::size_t;


TEST(TestDiagram, BasicConstructor_GetTitle_GetAxisXTitle)
{
    // Testing the default construction
    Diagram<data_type, index_type> default_diagram;
    EXPECT_EQ(default_diagram.GetTitle(), std::string(""));
    EXPECT_EQ(default_diagram.GetAxisXTitle(), std::string(""));

    // Testing the construction from values
    std::string diagram_title("DiagramTitle");
    std::string axis_x_title("AxisXTitle");
    Diagram<data_type, index_type> diagram_from_values(diagram_title, axis_x_title);
    EXPECT_EQ(diagram_from_values.GetTitle(), diagram_title);
    EXPECT_EQ(diagram_from_values.GetAxisXTitle(), axis_x_title);
}

TEST(TestDiagram, CopyConstructor_MoveConstructor)
{
    std::string diagram_title("DiagramTitle");
    std::string axis_x_title("AxisXTitle");
    Diagram<data_type, index_type> diagram_to_copy(diagram_title, axis_x_title);

    // Testing the copy construction
    Diagram<data_type, index_type> copied_diagram(diagram_to_copy);
    EXPECT_EQ(copied_diagram.GetTitle(), diagram_title);
    EXPECT_EQ(copied_diagram.GetAxisXTitle(), axis_x_title);

    Diagram<data_type, index_type> diagram_to_move(diagram_title, axis_x_title);

    // Testing the move construction
    Diagram<data_type, index_type> moved_diagram(std::move(diagram_to_move));
    EXPECT_EQ(copied_diagram.GetTitle(), diagram_title);
    EXPECT_EQ(copied_diagram.GetAxisXTitle(), axis_x_title);
}

TEST(TestDiagram, Operators)
{
    std::string diagram_title("DiagramTitle");
    std::string axis_x_title("AxisXTitle");
    Diagram<data_type, index_type> diagram_to_copy(diagram_title, axis_x_title);
    Diagram<data_type, index_type> diagram_to_move(diagram_title, axis_x_title);

    Diagram<data_type, index_type> diagram_to_overwrite;
    EXPECT_EQ(diagram_to_overwrite.GetTitle(), "");
    EXPECT_EQ(diagram_to_overwrite.GetAxisXTitle(), "");

    // Testing the copy operator
    diagram_to_overwrite = diagram_to_copy;
    EXPECT_EQ(diagram_to_overwrite.GetTitle(), diagram_title);
    EXPECT_EQ(diagram_to_overwrite.GetAxisXTitle(), axis_x_title);

    diagram_to_overwrite.EraseContent();
    EXPECT_EQ(diagram_to_overwrite.GetTitle(), "");
    EXPECT_EQ(diagram_to_overwrite.GetAxisXTitle(), "");

    // Testing the move operator
    diagram_to_overwrite = std::move(diagram_to_move);
    EXPECT_EQ(diagram_to_overwrite.GetTitle(), diagram_title);
    EXPECT_EQ(diagram_to_overwrite.GetAxisXTitle(), axis_x_title);
}

TEST(TestDiagram, SetTitle_SetAxisXTitle)
{
    Diagram<data_type, index_type> diagram;
    EXPECT_EQ(diagram.GetTitle(), std::string(""));
    EXPECT_EQ(diagram.GetAxisXTitle(), std::string(""));

    std::string diagram_title("DiagramTitle");
    std::string axis_x_title("AxisXTitle");

    // Testing the SetTitle
    diagram.SetTitle(diagram_title);
    EXPECT_EQ(diagram.GetTitle(), diagram_title);

    // Testing the SetAxisXTitle
    diagram.SetAxisXTitle(axis_x_title);
    EXPECT_EQ(diagram.GetAxisXTitle(), axis_x_title);
}

TEST(TestDiagram, AddNewDataLine_GetTheNumberOfDataLines)
{
    Diagram<data_type, index_type> diagram;

    EXPECT_EQ(diagram.GetTheNumberOfDataLines(), index_type(0));
    diagram.AddNewDataLine("First Data Line");
    EXPECT_EQ(diagram.GetTheNumberOfDataLines(), index_type(1));
    diagram.AddNewDataLine("Second Data Line");
    EXPECT_EQ(diagram.GetTheNumberOfDataLines(), index_type(2));
}

TEST(TestDiagram, GetDataLineTitle_SetDataLineTitle)
{
    Diagram<data_type, index_type> diagram;
    std::string old_title("OldTitle");
    std::string new_title("NewTitle");

    // Testing the getting and setting of unexisting data_line objects
    EXPECT_THROW(diagram.GetDataLineTitle(0), std::string);
    EXPECT_THROW(diagram.SetDataLineTitle(0, new_title), std::string);

    // Testing the getting and setting of existing data_line objects
    diagram.AddNewDataLine("", old_title);
    EXPECT_EQ(diagram.GetDataLineTitle(0), old_title);
    diagram.SetDataLineTitle(0, new_title);
    EXPECT_EQ(diagram.GetDataLineTitle(0), new_title);
}

TEST(TestDiagram, AddNewDataPoint_GetTheNumberOfDataPoints)
{
    DataPoint<data_type> first_data_point(1, 2);
    DataPoint<data_type> second_data_point(3, 4);
    DataPoint<data_type> third_data_point(5, 6);
    Diagram<data_type, index_type> diagram;

    // Testing the getting of data_point number and data_point adding to unexisting data_line objects
    EXPECT_THROW(diagram.GetTheNumberOfDataPoints(0), std::string);
    EXPECT_THROW(diagram.AddNewDataPoint(0, first_data_point), std::string);

    // Testing the getting of data_point number and data_point adding to existing data_line objects
    diagram.AddNewDataLine("First Data Line");
    EXPECT_EQ(diagram.GetTheNumberOfDataPoints(0), index_type(0));
    diagram.AddNewDataPoint(0, first_data_point);
    diagram.AddNewDataLine("Second Data Line");
    EXPECT_EQ(diagram.GetTheNumberOfDataPoints(1), index_type(0));
    diagram.AddNewDataPoint(1, first_data_point);
    diagram.AddNewDataPoint(1, second_data_point);
    diagram.AddNewDataLine("Third Data Line");
    EXPECT_EQ(diagram.GetTheNumberOfDataPoints(2), index_type(0));
    diagram.AddNewDataPoint(2, first_data_point);
    diagram.AddNewDataPoint(2, second_data_point);
    diagram.AddNewDataPoint(2, third_data_point);
    EXPECT_EQ(diagram.GetTheNumberOfDataPoints(0), index_type(1));
    EXPECT_EQ(diagram.GetTheNumberOfDataPoints(1), index_type(2));
    EXPECT_EQ(diagram.GetTheNumberOfDataPoints(2), index_type(3));
}

TEST(TestDiagram, GetDataPoint_SetDataPoint)
{
    DataPoint<data_type> first_data_point(1, 2);
    DataPoint<data_type> second_data_point(3, 4);
    Diagram<data_type, index_type> diagram;

    // Testing the getting and setting of unexisting data_point objects
    EXPECT_THROW(diagram.GetDataPoint(0, 0), std::string);
    EXPECT_THROW(diagram.SetDataPoint(0, 0, first_data_point), std::string);

    // Testing the getting and setting of existing data_point objects
    diagram.AddNewDataLine("First Data Line");
    diagram.AddNewDataPoint(0, first_data_point);
    diagram.AddNewDataLine("Second Data Line");
    diagram.AddNewDataPoint(1, first_data_point);
    diagram.AddNewDataPoint(1, second_data_point);

    EXPECT_EQ(diagram.GetDataPoint(0, 0), first_data_point);
    EXPECT_EQ(diagram.GetDataPoint(1, 0), first_data_point);
    EXPECT_EQ(diagram.GetDataPoint(1, 1), second_data_point);

    diagram.SetDataPoint(1, 0, diagram.GetDataPoint(1, 1));
    EXPECT_EQ(diagram.GetDataPoint(1, 0), diagram.GetDataPoint(1, 1));
    diagram.SetDataPoint(0, 0, second_data_point);
    EXPECT_EQ(diagram.GetDataPoint(0,0), second_data_point);
}

TEST(TestDiagram, GetExtremeValues)
{
    DataPoint<data_type>();
    Diagram<data_type, index_type> diagram;

    EXPECT_THROW(diagram.GetExtremeValues(0), std::string);
    EXPECT_THROW(diagram.GetExtremeValues(), std::string);

    diagram.AddNewDataLine("First Data Line");
    diagram.AddNewDataPoint(0, DataPoint<data_type>(-2, -2));
    diagram.AddNewDataPoint(0, DataPoint<data_type>(-1, -1));
    diagram.AddNewDataPoint(0, DataPoint<data_type>( 0,  0));
    diagram.AddNewDataPoint(0, DataPoint<data_type>( 1,  1));
    diagram.AddNewDataPoint(0, DataPoint<data_type>( 2,  2));
    diagram.AddNewDataLine("Second Data Line");
    diagram.AddNewDataPoint(1, DataPoint<data_type>(-2,  1));
    diagram.AddNewDataPoint(1, DataPoint<data_type>(-1,  1));
    diagram.AddNewDataPoint(1, DataPoint<data_type>( 0,  1));
    diagram.AddNewDataPoint(1, DataPoint<data_type>( 1,  1));
    diagram.AddNewDataPoint(1, DataPoint<data_type>( 2,  1));
    diagram.AddNewDataLine("Third Data Line");
    diagram.AddNewDataPoint(2, DataPoint<data_type>(-2,  2));
    diagram.AddNewDataPoint(2, DataPoint<data_type>(-1,  1));
    diagram.AddNewDataPoint(2, DataPoint<data_type>( 0,  0));
    diagram.AddNewDataPoint(2, DataPoint<data_type>( 1, -1));
    diagram.AddNewDataPoint(2, DataPoint<data_type>( 2,  0));

    EXPECT_EQ(diagram.GetExtremeValues(0), std::pair(DataPoint<data_type>(-2, -2), DataPoint<data_type>(2, 2)));
    EXPECT_EQ(diagram.GetExtremeValues(1), std::pair(DataPoint<data_type>(-2,  1), DataPoint<data_type>(2, 1)));
    EXPECT_EQ(diagram.GetExtremeValues(2), std::pair(DataPoint<data_type>(-2, -1), DataPoint<data_type>(2, 2)));
    EXPECT_EQ(diagram.GetExtremeValues(),  std::pair(DataPoint<data_type>(-2, -2), DataPoint<data_type>(2, 2)));
}

TEST(TestDiagram, EraseContent)
{
    std::string empty_title("");
    std::string diagram_title("DiagramTitle");
    std::string axis_x_title("AxisXTitle");
    DataPoint<data_type> first_data_point(1, 2);
    DataPoint<data_type> second_data_point(3, 4);
    Diagram<data_type, index_type> diagram(diagram_title, axis_x_title);

    diagram.AddNewDataLine("First Data Line");
    diagram.AddNewDataPoint(0, first_data_point);
    diagram.AddNewDataLine("Second Data Line");
    diagram.AddNewDataPoint(1, first_data_point);
    diagram.AddNewDataPoint(1, second_data_point);

    EXPECT_EQ(diagram.GetTitle(), diagram_title);
    EXPECT_EQ(diagram.GetAxisXTitle(), axis_x_title);
    EXPECT_EQ(diagram.GetTheNumberOfDataLines(), index_type(2));
    EXPECT_EQ(diagram.GetTheNumberOfDataPoints(0), index_type(1));
    EXPECT_EQ(diagram.GetTheNumberOfDataPoints(1), index_type(2));

    diagram.EraseContent();

    EXPECT_EQ(diagram.GetTitle(), empty_title);
    EXPECT_EQ(diagram.GetAxisXTitle(), empty_title);
    EXPECT_EQ(diagram.GetTheNumberOfDataLines(), index_type(0));
    EXPECT_THROW(diagram.GetTheNumberOfDataPoints(0), std::string);
    EXPECT_THROW(diagram.GetTheNumberOfDataPoints(1), std::string);
}
