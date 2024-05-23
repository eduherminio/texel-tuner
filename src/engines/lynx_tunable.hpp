#include "../base.h"
#include "../external/chess.hpp"
#include <string>
#include <cmath>

#define NAME(a) #a;
using i32 = int32_t;

class TunableSingle
{
    i32 _mg;
    i32 _eg;

public:
    i32 packed;
    i32 index;
    constexpr static i32 size = 1;

    TunableSingle(const i32 mg, const i32 eg)
        : _mg(mg), _eg(eg)
    {
        packed = S(_mg, _eg);
    }

    void add(parameters_t &parameters)
    {
        index = parameters.size();
        parameters.push_back({(double)_mg, (double)_eg});
    }

    void to_json(const parameters_t &parameters, std::stringstream &ss, const std::string &name)
    {
        ss << "\"" << name << "\": {\n"
           << "\t\"MG\": " << round(parameters[index][0]) << ",\n"
           << "\t\"EG\": " << round(parameters[index][1]) << "\n}";
    }
};

class TunableArray
{
    std::vector<i32> _mg;
    std::vector<i32> _eg;

public:
    std::vector<i32> packed;
    i32 index;
    i32 size;
    i32 start;
    i32 end;

    TunableArray(const std::vector<i32> mg, const std::vector<i32> eg, i32 size)
    {
        TunableArray(mg, eg, size, 0, 0);
    }

    TunableArray(const std::vector<i32> mg, const std::vector<i32> eg, i32 size, i32 start, i32 end)
        : _mg(mg), _eg(eg), size(size), start(start), end(end)
    {
        packed = std::vector<i32>(size);

        for (int rank = 0 + start; rank < size - end; ++rank)
        {
            packed[rank] = S(mg[rank], eg[rank]);
        }
    }

    void add(parameters_t &parameters)
    {
        index = parameters.size();
        for (int rank = 0 + start; rank < size - end; ++rank)
        {
            parameters.push_back({(double)_mg[rank], (double)_eg[rank]});
        }
    }

    void to_json(const parameters_t &parameters, std::stringstream &ss, const std::string &name)
    {
        const std::string keyword = size == 8
                                        ? "Rank"
                                        : "Count";

        ss << "\"" << name << "\": {\n";
        for (int rank = 0; rank < start; ++rank)
        {
            ss << "\t\"" << keyword << rank << "\": {\n";
            ss << "\t\t\"MG\": " << 0 << ",\n";
            ss << "\t\t\"EG\": " << 0 << "\n\t},\n";
        }

        for (int rank = 0; rank < size - end - start; ++rank)
        {
            ss << "\t\"" << keyword << rank + start << "\": {\n";
            ss << "\t\t\"MG\": " << round(parameters[index + rank][0]) << ",\n";
            ss << "\t\t\"EG\": " << round(parameters[index + rank][1]) << "\n\t},\n";
        }

        for (int rank = size - end; rank < size; ++rank)
        {
            ss << "\t\"" << keyword << rank << "\": {\n";
            ss << "\t\t\"MG\": " << 0 << ",\n";
            ss << "\t\t\"EG\": " << 0 << "\n\t}\n";
            ss << "}";
        }
    }
};