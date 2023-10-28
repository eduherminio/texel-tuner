#ifndef CONFIG_H
#define CONFIG_H 1

#include <cstdint>
#include "engines/fourku.h"
#include "engines/toy.h"
#include "engines/toy_tapered.h"
#include "engines/lynx.hpp"

#define TAPERED 1
//using TuneEval = Toy::ToyEval;
//using TuneEval = Toy::ToyEvalTapered;
//using TuneEval = Fourku::FourkuEval;
using TuneEval = Lynx;
constexpr int32_t thread_count = 8;
constexpr double preferred_k = 0;
constexpr int32_t max_epoch = 10001;
constexpr bool retune_from_zero = false;
constexpr bool enable_qsearch = false;
constexpr bool print_data_entries = false;
constexpr bool print_eval = false;
constexpr int32_t data_load_print_interval = 10000;

#endif // !CONFIG_H
