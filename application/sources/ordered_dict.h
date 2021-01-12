//==============================================================================//
//                                                                              //
//    RDB Monitor                                                               //
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



#ifndef ORDERED_DICT_H
#define ORDERED_DICT_H



#include <string>
#include <vector>
#include <utility>
#include <stdexcept>



template<typename T_VALUE, typename T_INDEX = std::size_t, typename T_KEY = std::string>
class OrderedDict
{
public:
    using key_t = T_KEY;
    using value_t = T_VALUE;
    using index_t = T_INDEX;
    using data_element_t = std::pair<key_t, value_t>;
    using data_container_t = std::vector<data_element_t>;

    OrderedDict() = default;

    OrderedDict(std::initializer_list<data_element_t> initial_data) : data(initial_data) {}

    OrderedDict(const OrderedDict&) = default;
    OrderedDict(OrderedDict&&) = default;

    virtual ~OrderedDict() = default;

    OrderedDict& operator=(const OrderedDict&) = default;
    OrderedDict& operator=(OrderedDict&&) = default;

    bool hasKey(const key_t& key) const
    {
        bool result = false;

        for(index_t i = 0; i < data.size(); i++)
        {
            if(key == data[i].first)
            {
                result = true;
                break;
            }
        }

        return result;
    }

    bool getIndexForKey(const key_t& key, index_t& index) const
    {
        bool result = false;

        for(index_t i = 0; i < data.size(); i++)
        {
            if(key == data[i].first)
            {
                index = i;
                result = true;
                break;
            }
        }

        return result;
    }

    bool getKeyForIndex(const index_t& index, key_t& key) const
    {
        bool result = false;

        if(index < data.size())
        {
            key = data[index].first;
            result = true;
        }

        return result;
    }

    value_t& operator[](const index_t& index)
    {
        if(index < data.size())
        {
            return data[index].second;
        }
        else
        {
            throw std::out_of_range("OrderedDict::operator[]: index is out of range!");
        }
    }

    value_t operator[](const index_t& index) const
    {
        if(index < data.size())
        {
            return data[index].second;
        }
        else
        {
            throw std::out_of_range("OrderedDict::operator[]: index is out of range!");
        }
    }

    value_t& operator[](const key_t& key)
    {
        index_t index;

        if(getIndexForKey(key, index))
        {
            return data[index].second;
        }
        else
        {
            data.emplace_back(key, value_t());
            return data.back().second;
        }
    }

    value_t operator[](const key_t& key) const
    {
        index_t index;

        if(getIndexForKey(key, index))
        {
            return data[index].second;
        }
        else
        {
            throw std::out_of_range("OrderedDict::operator[]: index is out of range!");
        }
    }

    bool push_back(const data_element_t new_element)
    {
        bool result = false;

        if(!hasKey(new_element.first))
        {
            data.push_back(new_element);
        }

        return result;
    }

    bool emplace_back(const key_t& key, const value_t& value)
    {
        bool result = false;

        if(!hasKey(key))
        {
            data.emplace_back(key, value);
        }

        return result;
    }

    bool empty(void) const { return data.empty(); }

    std::size_t size(void) const { return data.size(); }

    void clear(void) { data.clear(); }

    auto begin(void) const { return data.begin(); }
    auto end(void) const { return data.end(); }


private:
    data_container_t data;
};



#endif // ORDERED_DICT_H
