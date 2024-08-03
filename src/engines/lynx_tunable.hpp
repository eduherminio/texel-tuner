#pragma once
#include "../base.h"
#include "../external/chess.hpp"
#include "lynx_base.hpp"
#include <string>
#include <cmath>
#include <climits>

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
           << "\t\"MG\": " << std::round(parameters[index][0]) << ",\n"
           << "\t\"EG\": " << std::round(parameters[index][1]) << "\n}";
    }

    void to_csharp(const parameters_t &parameters, std::stringstream &ss, const std::string &name)
    {
        ss << "\tpublic static TaperedEvaluationTerm " << name << " { [MethodImpl(MethodImplOptions.AggressiveInlining)] get; } = new(" << std::round(parameters[index][0]) << ", " << std::round(parameters[index][1]) << ");\n\n";
    }

    void to_cpp(const parameters_t &parameters, std::stringstream &ss, const std::string &name)
    {
        ss << "TunableSingle " << name << "(" << std::round(parameters[index][0]) << ", " << std::round(parameters[index][1]) << ");\n";
    }
};

class TunableSingleBucketed
{
    i32 _index;
    std::array<i32, PSQTBucketCount> _packed;

public:
    constexpr static i32 size = PSQTBucketCount;

    TunableSingleBucketed(std::array<i32, PSQTBucketCount> packed)
        : _packed(packed)
    {
    }

    void add(parameters_t &parameters)
    {
        _index = parameters.size();
        for (int i : _packed)
        {
            parameters.push_back({(double)mg_score(i), (double)eg_score(i)});
        }
    }

    int index(int bucket)
    {
        return _index + bucket;
    }

    int packed(int bucket)
    {
        return _packed[bucket];
    }

    void to_json(const parameters_t &parameters, std::stringstream &ss, const std::string &name)
    {
    }

    void to_csharp(const parameters_t &parameters, std::stringstream &ss, const std::string &name)
    {

        ss << "\tpublic static TaperedEvaluationTerm[] " << name << " { [MethodImpl(MethodImplOptions.AggressiveInlining)] get; } =" << std::endl
           << "\t[" << std::endl;

        for (int i = 0; i < size; ++i)
        {
            auto mg = std::round(parameters[_index + i][0]);
            auto eg = std::round(parameters[_index + i][1]);
            ss << "\t\tnew (" << mg << ", " << eg << ")," << std::endl;
        }

        ss << "\t];\n\n";
    }

    void to_cpp(const parameters_t &parameters, std::stringstream &ss, const std::string &name)
    {
        ss << "TunableSingleBucketed " << name << "(std::array<i32, PSQTBucketCount>(" << std::endl
           << "\t{" << std::endl;

        for (int i = 0; i < size; ++i)
        {
            auto mg = std::round(parameters[_index + i][0]);
            auto eg = std::round(parameters[_index + i][1]);
            ss << "\t\tS(" << mg << ", " << eg << "),\n";
        }

        ss << "\t}));\n\n";
    }
};

class TunableArray
{
    static inline std::array<std::array<tune_t, 12>, PSQTBucketCount> emptyArray;
    std::vector<i32> _mg;
    std::vector<i32> _eg;

public:
    std::vector<i32> packed;
    i32 pieceIndex;
    i32 index;
    i32 size;
    i32 tunableSize;
    i32 start;
    i32 end;

    TunableArray(chess::PieceType piece, const std::vector<i32> mg, const std::vector<i32> eg)
    {
        TunableArray(piece, mg, eg, 0, 0);
    }

    TunableArray(chess::PieceType piece, const std::vector<i32> mg, const std::vector<i32> eg, i32 start, i32 end)
        : pieceIndex(static_cast<int>(piece)), _mg(mg), _eg(eg), start(start), end(end)
    {
        for (int b = 0; b < PSQTBucketCount; ++b)
        {
            emptyArray[b] = std::array<tune_t, 12>{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        }

        if (mg.size() != eg.size())
        {
            throw std::invalid_argument("mg and eg size mismatch");
        }

        size = mg.size();
        tunableSize = size - start - end;

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

    /// Extracts first not-zero value
    pair_t extract_offset(const parameters_t &parameters)
    {
        pair_t packed;

        for (int phase = 0; phase <= 1; ++phase)
        {
            tune_t min = std::numeric_limits<double>::max();

            for (int i = 0; i < size; ++i)
            {
                if (parameters[index + i][phase] != 0)
                {
                    min = parameters[index + i][phase];
                    packed[phase] = min;

                    break;
                }
            }
        }

        return packed;
    }

    void to_json(const parameters_t &parameters, std::stringstream &ss, const std::string &name)
    {
        to_json(parameters, ss, name, emptyArray);
    }

    void to_json(const parameters_t &parameters, std::stringstream &ss, const std::string &name, const std::array<std::array<tune_t, 12>, PSQTBucketCount> &mobilityPieceValues)
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
            ss << "\t\t\"MG\": " << round(parameters[index + rank][0] - mobilityPieceValues[0][pieceIndex]) << ",\n";
            ss << "\t\t\"EG\": " << round(parameters[index + rank][1] - mobilityPieceValues[0][pieceIndex + 6]) << "\n\t}";

            if (rank != size - 1)
                ss << ",";
            ss << "\n";
        }

        for (int rank = size - end; rank < size; ++rank)
        {
            ss << "\t\"" << keyword << rank << "\": {\n";
            ss << "\t\t\"MG\": " << 0 << ",\n";
            ss << "\t\t\"EG\": " << 0 << "\n\t}\n";

            if (rank != size - 1)
                ss << ",";
            ss << "\n";
        }
        ss << "}";
    }

    void to_csharp(const parameters_t &parameters, std::stringstream &ss, const std::string &name)
    {
        to_csharp(parameters, ss, name, emptyArray);
    }

    void to_csharp(const parameters_t &parameters, std::stringstream &ss, const std::string &name, const std::array<std::array<tune_t, 12>, PSQTBucketCount> &mobilityPieceValues)
    {
        std::string variable_name;

        if (size == 8)
        {
            variable_name = "TaperedEvaluationTermByRank";
        }
        else if (size == 9)
        {
            variable_name = "TaperedEvaluationTermByCount8";
        }
        else if (size == 15)
        {
            variable_name = "TaperedEvaluationTermByCount14";
        }
        else if (size == 28)
        {
            variable_name = "TaperedEvaluationTermByCount27";
        }
        else
        {
            throw std::invalid_argument("wrong size provided: " + size);
        }

        ss << "\tpublic " << variable_name << " " << name << " { get; set; } = new(\n";
        for (int rank = 0; rank < start; ++rank)
        {
            ss << "\t\tnew(0, 0),\n";
        }

        for (int rank = 0; rank < size - end - start; ++rank)
        {
            ss << "\t\tnew(" << round(parameters[index + rank][0] - mobilityPieceValues[0][pieceIndex]) << ", " << round(parameters[index + rank][1] - mobilityPieceValues[0][pieceIndex + 6]) << ")";
            if (rank == size - start - 1)
                ss << ");";
            else
                ss << ",\n";
        }

        for (int rank = size - end; rank < size; ++rank)
        {
            ss << "\t\tnew(0, 0)";
            if (rank == size - 1)
                ss << ");";
            else
                ss << ",\n";
        }

        ss << "\n\n";
    }

    void to_cpp(const parameters_t &parameters, std::stringstream &ss, const std::string &name)
    {
        to_cpp(parameters, ss, name, emptyArray);
    }

    void to_cpp(const parameters_t &parameters, std::stringstream &ss, const std::string &name, const std::array<std::array<tune_t, 12>, PSQTBucketCount> &mobilityPieceValues)
    {
        std::string pieceType;
        switch (pieceIndex)
        {
        case static_cast<int>(chess::PieceType::PAWN):
        {
            pieceType = "PAWN";
            break;
        }
        case static_cast<int>(chess::PieceType::KNIGHT):
        {
            pieceType = "KNIGHT";
            break;
        }
        case static_cast<int>(chess::PieceType::BISHOP):
        {
            pieceType = "BISHOP";
            break;
        }
        case static_cast<int>(chess::PieceType::ROOK):
        {
            pieceType = "ROOK";
            break;
        }
        case static_cast<int>(chess::PieceType::QUEEN):
        {
            pieceType = "QUEEN";
            break;
        }
        case static_cast<int>(chess::PieceType::KING):
        {
            pieceType = "KING";
            break;
        }
        }

        ss << "TunableArray " << name << "(\n";
        ss << "\tchess::PieceType::" << pieceType << ",\n";
        ss << "\tstd::vector<int>{";
        for (int rank = 0; rank < start; ++rank)
        {
            ss << "0, ";
        }

        for (int rank = 0; rank < size - end - start; ++rank)
        {
            ss << std::round(parameters[index + rank][0] - mobilityPieceValues[0][pieceIndex]);
            if (rank == size - start - 1)
                ss << "},\n";
            else
                ss << ", ";
        }

        for (int rank = size - end; rank < size; ++rank)
        {
            ss << "0";
            if (rank == size - 1)
                ss << "},\n";
            else
                ss << ", ";
        }

        ss << "\tstd::vector<int>{";

        for (int rank = 0; rank < start; ++rank)
        {
            ss << "0, ";
        }

        for (int rank = 0; rank < size - end - start; ++rank)
        {
            ss << std::round(parameters[index + rank][1] - mobilityPieceValues[0][pieceIndex + 6]);
            if (rank == size - start - 1)
                ss << "},\n";
            else
                ss << ", ";
        }

        for (int rank = size - end; rank < size; ++rank)
        {
            ss << "0";
            if (rank == size - 1)
                ss << "},\n";
            else
                ss << ", ";
        }

        ss << "\t" << start << ",\n";
        ss << "\t" << end << ");\n\n";
    }
};
