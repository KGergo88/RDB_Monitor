#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cctype>
#include <regex>

#include "global.hpp"
#include "diagram.hpp"

#ifndef DATA_PROCESSOR_HPP
#define DATA_PROCESSOR_HPP

class DataProcessor
{
public:
    static inline DataProcessor& GetInstance(void)
    {
        static DataProcessor Singleton;
        return Singleton;
    }

    DataProcessor(const DataProcessor&) = delete;
    DataProcessor(DataProcessor&&) = delete;

    DataProcessor& operator=(const DataProcessor&) = delete;
    DataProcessor& operator=(DataProcessor&&) = delete;

    std::vector<std::unique_ptr<DiagramSpecialized> > ProcessData(std::string data_source, std::istream& input_data);

private:
    enum class ProcessingStates : uint8_t
    {
        WaitingForStartLine,
        ProcessingHeadline,
        ProcessingDataLines
    };

#warning "This part needs to be reworked...the <<<START>>> and <<<END>>> strings are duplicated here and in the global.hpp"
    // REGEX strings to search the input data for valid measurement session
    std::string regex_start_line         = R"(^\s*<<<START>>>$)";
    std::string regex_headline           = R"(^\s*(\w+,){2,}$)";
    std::string regex_headline_analyzer  = R"(^\s*(\w+),)";
    std::string regex_data_line          = R"(^\s*(((?:\+|\-)?\d+),){2,}$)";
    std::string regex_data_line_analyzer = R"(^\s*((?:\+|\-)?\d+),)";
    std::string regex_end_line           = R"(^\s*<<<END>>>$)";

    DataProcessor() = default;

    std::shared_ptr<std::vector<std::string> > ProcessHeadLine(std::string& headline);
    std::shared_ptr<std::vector<DataPointType> > ProcessDataLine(std::string& data_line);
};

#endif // DATA_PROCESSOR_HPP
