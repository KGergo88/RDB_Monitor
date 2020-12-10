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

#include <cstdint>

#include "../application/sources/ordered_dict.h"



TEST(TestOrderedDict, Constructor)
{
    OrderedDict<std::string> myDict({std::make_pair("key_1", "value_1"), std::make_pair("key_2", "value_2"), std::make_pair("key_3", "value_3")});
    EXPECT_EQ(myDict.size(), std::size_t(3));

    EXPECT_EQ(myDict["key_1"], "value_1");
    EXPECT_EQ(myDict["key_2"], "value_2");
    EXPECT_EQ(myDict["key_3"], "value_3");

    EXPECT_EQ(myDict[0], "value_1");
    EXPECT_EQ(myDict[1], "value_2");
    EXPECT_EQ(myDict[2], "value_3");
}

TEST(TestOrderedDict, GetIndexForKeyGetKeyForIndex)
{
    OrderedDict<uint8_t> myDict;
    OrderedDict<uint8_t>::key_t key;
    OrderedDict<uint8_t>::index_t index;

    key = "myKey";
    index = 1234;
    EXPECT_EQ(myDict.getIndexForKey(key, index), false);
    EXPECT_EQ(key, "myKey");
    EXPECT_EQ(index, OrderedDict<uint8_t>::index_t(1234));
    EXPECT_EQ(myDict.getKeyForIndex(index, key), false);
    EXPECT_EQ(key, "myKey");
    EXPECT_EQ(index, OrderedDict<uint8_t>::index_t(1234));

    myDict.push_back("first", 1);
    myDict.push_back("second", 2);
    myDict.push_back("third", 3);

    key = "second";
    index = 1;
    EXPECT_EQ(myDict.getIndexForKey(key, index), true);
    EXPECT_EQ(key, "second");
    EXPECT_EQ(index, OrderedDict<uint8_t>::index_t(1));

    key = "";
    index = 2;
    EXPECT_EQ(myDict.getKeyForIndex(index, key), true);
    EXPECT_EQ(key, "third");
    EXPECT_EQ(index, OrderedDict<uint8_t>::index_t(2));
}

TEST(TestOrderedDict, NonConstOperators)
{
    OrderedDict<uint8_t> myDict;
    OrderedDict<uint8_t>::key_t key;
    OrderedDict<uint8_t>::index_t index;
    OrderedDict<uint8_t>::value_t value;

    index = 0;
    value = 123;
    ASSERT_THROW(value = myDict[index], std::out_of_range);
    EXPECT_EQ(index, OrderedDict<uint8_t>::index_t(0));
    EXPECT_EQ(value, 123);

    key = "myKey";
    EXPECT_NO_THROW(
    {
        myDict[key] = value;
    });
    EXPECT_EQ(key, "myKey");
    EXPECT_EQ(myDict[key], value);

    EXPECT_NO_THROW(
    {
        myDict[key] = value;
    });
    EXPECT_EQ(key, "myKey");
    EXPECT_EQ(myDict[key], value);

    EXPECT_EQ(myDict[index], value);
    value = 212;
    EXPECT_NO_THROW(
    {
        myDict[index] = value;
    });
    EXPECT_EQ(myDict[index], value);
}

TEST(TestOrderedDict, ConstOperators)
{
    const OrderedDict<std::string> myDict({std::make_pair("key_1", "value_1"), std::make_pair("key_2", "value_2"), std::make_pair("key_3", "value_3")});
    EXPECT_EQ(myDict.size(), std::size_t(3));

    EXPECT_EQ(myDict["key_1"], "value_1");
    EXPECT_EQ(myDict["key_2"], "value_2");
    EXPECT_EQ(myDict["key_3"], "value_3");

    EXPECT_EQ(myDict[0], "value_1");
    EXPECT_EQ(myDict[1], "value_2");
    EXPECT_EQ(myDict[2], "value_3");
}

TEST(TestOrderedDict, UtilityFunctions)
{
    OrderedDict<uint8_t> myDict;

    EXPECT_EQ(myDict.empty(), true);
    EXPECT_EQ(myDict.size(), std::size_t(0));

    myDict.push_back("first", 1);
    myDict.push_back("second", 2);
    myDict.push_back("third", 3);

    EXPECT_EQ(myDict.empty(), false);
    EXPECT_EQ(myDict.size(), std::size_t(3));

    myDict.clear();

    EXPECT_EQ(myDict.empty(), true);
    EXPECT_EQ(myDict.size(), std::size_t(0));
}
