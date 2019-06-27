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



#include "test_data_point.hpp"

#include "../application/sources/data_point.hpp"



void TestDataPoint::ConstructorsAndGetters(void)
{
    // Testing the default constructor
    int zero = 0;
    DataPoint<int> default_constructed;
    QCOMPARE(default_constructed.GetX(), zero);
    QCOMPARE(default_constructed.GetY(), zero);
    QCOMPARE(DataPoint<int>::GetXValueOf(default_constructed), zero);
    QCOMPARE(DataPoint<int>::GetYValueOf(default_constructed), zero);

    // Testing the construction from values
    double x_double = 3.5;
    double y_double = 7.6;
    const DataPoint<double> value_constructed(x_double, y_double);
    QCOMPARE(value_constructed.GetX(), x_double);
    QCOMPARE(value_constructed.GetY(), y_double);
    QCOMPARE(DataPoint<double>::GetXValueOf(value_constructed), x_double);
    QCOMPARE(DataPoint<double>::GetYValueOf(value_constructed), y_double);

    // Testing the copy construction
    int x_int = 456;
    int y_int = 987;
    DataPoint<int> point_to_copy(x_int, y_int);
    DataPoint<int> copied_point(point_to_copy);
    QCOMPARE(copied_point.GetX(), x_int);
    QCOMPARE(copied_point.GetY(), y_int);

    // Testing the move construction
    DataPoint<int> moved_point(DataPoint(x_int, y_int));
    QCOMPARE(moved_point.GetX(), x_int);
    QCOMPARE(moved_point.GetY(), y_int);
}

void TestDataPoint::Operators(void)
{
    // Testing the equals operator
    DataPoint<int> first(5,9);
    DataPoint<int> second(9,5);
    QCOMPARE((first == second), false);

    // Testing the copy assignment operator
    first = second;
    QCOMPARE((first == second), true);

    // Testing the move assignment operator
    second = DataPoint<int>(42,52);
    QCOMPARE((second == DataPoint<int>(42,52)), true);
}

void TestDataPoint::Setters(void)
{
    // Testing the SetX and SetY setters
    DataPoint<std::size_t> data_point;
    QCOMPARE(data_point.GetX(), std::size_t(0));
    QCOMPARE(data_point.GetY(), std::size_t(0));
    data_point.SetX(std::size_t(6));
    QCOMPARE(data_point.GetX(), std::size_t(6));
    data_point.SetY(std::size_t(78));
    QCOMPARE(data_point.GetY(), std::size_t(78));

    // Testing the SetXValueOf and SetYValueOf setters
    DataPoint<std::size_t>::SetXValueOf(data_point, std::size_t(45));
    QCOMPARE(data_point.GetX(), std::size_t(45));
    DataPoint<std::size_t>::SetYValueOf(data_point, std::size_t(81));
    QCOMPARE(data_point.GetY(), std::size_t(81));
}

void TestDataPoint::Comparators(void)
{
    DataPoint<int> lower_left_point(1, 3);
    DataPoint<int> upper_right_point(8, 15);

    // Testing the CompareXValues
    QCOMPARE(DataPoint<int>::CompareXValues(lower_left_point, upper_right_point), true);
    QCOMPARE(DataPoint<int>::CompareXValues(upper_right_point, lower_left_point), false);

    // Testing the CompareYValues
    QCOMPARE(DataPoint<int>::CompareYValues(lower_left_point, upper_right_point), true);
    QCOMPARE(DataPoint<int>::CompareYValues(upper_right_point, lower_left_point), false);
}

QTEST_MAIN(TestDataPoint)
