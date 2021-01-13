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

#include "../application/sources/data_line.hpp"



class TestDataLine : public ::testing::Test
{
protected:
    using Coordinate_t = int;
    using DataPoint_t = DataPoint<Coordinate_t>;
    using DataLine_t = DataLine<Coordinate_t, std::size_t>;

};

TEST_F(TestDataLine, Constructors_GetTitle)
{
    // Testing the default construction
    DataLine_t data_line_default;
    EXPECT_EQ(data_line_default.GetTitle(), std::string(""));

    // Testing the construction from values
    std::string data_line_title = "DataLineTitle";
    DataLine_t data_line_with_title(data_line_title);
    EXPECT_EQ(data_line_with_title.GetTitle(), data_line_title);

    // Testing the copy construction
    DataLine copied_data_line(data_line_with_title);
    EXPECT_EQ(copied_data_line.GetTitle(), data_line_title);

    // Testing the move construction
    DataLine data_line_to_move(data_line_with_title);
    DataLine moved_data_line(std::move(data_line_to_move));
    EXPECT_EQ(moved_data_line.GetTitle(), data_line_title);
}

TEST_F(TestDataLine, Operators)
{
    std::string data_line_title = "DataLineTitle";
    std::string overwritten_title = "Title that will be lost...";
    DataLine_t data_line_with_title(data_line_title);

    // Testing the copy operator
    DataLine_t copied_data_line;
    copied_data_line.SetTitle(overwritten_title);
    EXPECT_EQ(copied_data_line.GetTitle(), overwritten_title);
    copied_data_line = data_line_with_title;
    EXPECT_EQ(copied_data_line.GetTitle(), data_line_title);

    // Testing the move operator
    DataLine_t data_line_to_move(data_line_title);
    DataLine_t moved_data_line;
    moved_data_line.SetTitle(overwritten_title);
    EXPECT_EQ(moved_data_line.GetTitle(), overwritten_title);
    moved_data_line = std::move(data_line_to_move);
    EXPECT_EQ(moved_data_line.GetTitle(), data_line_title);
}

TEST_F(TestDataLine, SetTitle)
{
    DataLine_t data_line_without_title;
    std::string data_line_title = "DataLineTitle";

    // Testing the SetTitle
    data_line_without_title.SetTitle(data_line_title);
    EXPECT_EQ(data_line_without_title.GetTitle(), data_line_title);
}

TEST_F(TestDataLine, AddNewDataPoint_GetTheNumberOfDataPoints_GetDataPoint)
{
    int x_value = 1;
    int y_value = 2;
    DataPoint_t first_data_point(x_value, y_value);
    DataPoint_t second_data_point(x_value * 2, y_value * 2);
    DataPoint_t third_data_point(x_value * 3, y_value * 3);
    DataPoint_t fourth_data_point(x_value * 4, y_value * 4);
    DataLine_t data_line;

    // Testing the GetTheNumberOfDataPoints and AddNewDataPoint
    EXPECT_EQ(data_line.GetTheNumberOfDataPoints(), std::size_t(0));
    data_line.AddNewDataPoint(first_data_point);
    EXPECT_EQ(data_line.GetTheNumberOfDataPoints(), std::size_t(1));
    data_line << second_data_point;
    EXPECT_EQ(data_line.GetTheNumberOfDataPoints(), std::size_t(2));
    data_line << third_data_point << fourth_data_point;
    EXPECT_EQ(data_line.GetTheNumberOfDataPoints(), std::size_t(4));

    // Testing the GetDataPoint
    EXPECT_EQ(first_data_point, data_line.GetDataPoint(0));
    EXPECT_EQ(second_data_point, data_line.GetDataPoint(1));
    EXPECT_EQ(third_data_point, data_line.GetDataPoint(2));
    EXPECT_EQ(fourth_data_point, data_line.GetDataPoint(3));
}

TEST_F(TestDataLine, SetDataPoint)
{
    int x_value = 1;
    int y_value = 2;
    DataPoint_t first_data_point(x_value, y_value);
    DataPoint_t second_data_point(x_value * 2, y_value * 2);
    DataLine_t data_line;
    data_line << first_data_point << second_data_point;
    EXPECT_EQ(first_data_point, data_line.GetDataPoint(0));
    EXPECT_EQ(second_data_point, data_line.GetDataPoint(1));
    EXPECT_NE(data_line.GetDataPoint(0), data_line.GetDataPoint(1));

    // Testing the SetDataPoint
    data_line.SetDataPoint(1, data_line.GetDataPoint(0));
    EXPECT_EQ(data_line.GetDataPoint(0), data_line.GetDataPoint(1));
    ASSERT_THROW(data_line.SetDataPoint(2, first_data_point), std::string);
}

TEST_F(TestDataLine, GetDataPointWithMinValue_GetDataPointWithMaxValue)
{
    DataPoint_t first_data_point(1, 5);
    DataPoint_t second_data_point(3, 2);
    DataLine_t data_line;

    auto lower_than_x_based = [](DataPoint_t first, DataPoint_t second) -> bool {return (first.GetX() < second.GetX());};
    auto lower_than_y_based = [](DataPoint_t first, DataPoint_t second) -> bool {return (first.GetY() < second.GetY());};

    // Testing the functions on an empty data line
    ASSERT_THROW(data_line.GetDataPointWithMinValue(lower_than_x_based), std::string);
    ASSERT_THROW(data_line.GetDataPointWithMinValue(lower_than_y_based), std::string);
    ASSERT_THROW(data_line.GetDataPointWithMaxValue(lower_than_x_based), std::string);
    ASSERT_THROW(data_line.GetDataPointWithMaxValue(lower_than_y_based), std::string);

    // Testing the functions on a data line with data
    data_line << first_data_point << second_data_point;
    EXPECT_EQ(data_line.GetDataPointWithMinValue(lower_than_x_based), first_data_point);
    EXPECT_EQ(data_line.GetDataPointWithMaxValue(lower_than_x_based), second_data_point);
    EXPECT_EQ(data_line.GetDataPointWithMinValue(lower_than_y_based), second_data_point);
    EXPECT_EQ(data_line.GetDataPointWithMaxValue(lower_than_y_based), first_data_point);
}

TEST_F(TestDataLine, CheckDataPointIndex)
{
    int x_value = 1;
    int y_value = 2;
    DataPoint_t first_data_point(x_value, y_value);
    DataLine_t data_line;
    EXPECT_EQ(data_line.GetTheNumberOfDataPoints(), std::size_t(0));
    data_line.AddNewDataPoint(first_data_point);
    EXPECT_EQ(data_line.GetTheNumberOfDataPoints(), std::size_t(1));

    // Testing the error checking for a non existing data point
    ASSERT_THROW(data_line.GetDataPoint(2), std::string);
}
