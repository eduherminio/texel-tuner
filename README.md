# Texel tuner

Thanks to [@toanth](https://github.com/toanth/) for helping me understand this tuner's behavior, for the minor fixes required to get it up and running and for helping me debugging my own code.

Regarding the tuner's behavior, in their words:

> From my understanding of Gedas' tuner, it assumes that your evaluation is a linear sum of features: Each feature (such as having a knight) can occur several times (or not at all) in a given position, and the tuner updates weights (such as the value of a knight) based on how often a position with this feature led to a win.

> The eval gets executed when you load the fens.
After that, the tuner only uses the stored weights and doesn't execute the eval again.

> One thing that kept tripping me up is that in an actual engine, you want to compute the score relative to the side to move, but in the tuner, you compute it relative to white.

Links to some training datasets:

- [Zurichess v7](https://bitbucket.org/zurichess/tuner/downloads/quiet-labeled.v7.epd.gz)

- ['lichess-big3.resolved'](https://discord.com/channels/1132289356011405342/1137454121876721755/1153433610527981598c)

- ['Stash data'](https://drive.google.com/file/d/1LaaW7bNHBnyEdt51MP6SAZCbSdPzlk8d/view) transformed into a format comptatible with this tuner by [cj5716](https://github.com/cj5716)

- [Ethereal data dump](https://www.talkchess.com/forum3/viewtopic.php?f=7&t=75350)

## Additions to the original tuner

### Build and run instructions

Instructions/examples about how to build and run the tuner are in [run.sh](./src/run.sh).
In sort:

- Make sure gcc v10 or higher is installed.

- Create a `sources.csv` file under `src/` with the following format:
  ```csv
  # Path, WDL from side playing, position limit
  #/home/edu/dev/texel-tuner/datasets/example.book,0,0
  ```

- Create a `build/` directory under `src/`

- Inside of `src/build/`, run:
  ```bash
  cmake ..
  ```
  or
  ```bash
  cmake -DCMAKE_BUILD_TYPE=Debug ..
  ```

- Inside of `src/build/`, run:
  ```bash
  cmake --build .
  ```

- Run the executable passing the path of `sources.csv` to it:
  ```
  ./tuner ../sources.csv
  ```

### Checking evaluation code correctness

`Is the ported evaluation code correct?` is a tricky question to answer.

`print_eval` variable has been added to [config.h](./src/config.h) to answer it.

When it's enabled, no tuning happens, but positions are printed next to their eval in a format that matches [Lynx's custom `staticeval` UCI command output](https://github.com/lynx-chess/Lynx).

This can be used with subset of a tuning dataset (i.e. 1000 entries) and the output redirected to a file, in order to be able to compare it with your engine's actual evaluation and detect any bugs that were written by porting your evaluation code to the tuner.

i.e.

- Tuner:

  ```bash
  cmake --build . && ./tuner ../sources.csv > quiet-labeled-tuner-evals.epd
  ```

- Engine:
  ```bash
  dotnet run -c Release "staticeval quiet-labeled-subset-1_1000.epd" > quiet-labeled-evals.epd
  ```

- Compare two files manually and see if there's any difference in the evaluations

----

This project is based on the linear evaluation ideas described in https://github.com/AndyGrant/Ethereal/blob/master/Tuning.pdf. The code is loosely based on an implementation in Weiss, which can be found at https://github.com/TerjeKir/weiss.

The project internally uses https://github.com/Disservin/chess-library

## Plugging in your evaluation
To add your own evaluation it is required to transform your evaluation into a linear system.

For each position in the training dataset, the evaluation should count the occurances of each evaluation term, and return a `coefficients_t` object where each entry is the count of times an evaluation term has been used per-side.

For a new engine it's required to implement an evaluation class with 4 functions and 2 constexpr variables. More on them at [Evaluation class](#evaluation-class)

```cpp
    class YourEval
    {
    public:
        constexpr static bool includes_additional_score = true;
        constexpr static bool supports_external_chess_eval = true;

        static parameters_t get_initial_parameters();
        static EvalResult get_fen_eval_result(const std::string& fen);
        static EvalResult get_external_eval_result(const Chess::Board& board);
        static void print_parameters(const parameters_t& parameters);
    };
```
Edit `config.h` to point `TuneEval` to your evaluation class. Edit thread_count to be equivalent to what you're comfortable with. If you're using a tapered evaluation, set `#define TAPERED 1` in both `base.h` and `config.h`, otherwhise set both to `#define TAPERED 0`.

Examples can be found in the `engines` directory. `ToyEval` and `ToyEvalTapered` are very minimal examples, while `Fourku` is a full example for the engine [4ku](https://github.com/kz04px/4ku).

## Evaluation class

### includes_additional_score
This parameter should be set to *true* if there are any terms in the evaluation which are not being tuned at the moment. If set to `false`, any additional terms would be ignored comepletely. If set to `true`, then the evaluation function should compute the score itself, and set it as `score` when returning an `EvalResult` from [get_*_eval_result](#get_fen_eval_result) functions.

The purpose of this is that if set to `false`, then the tuning can be faster, while if set to `true`, the terms being tuned would be tuned *around* any other existing terms that are not being tuned, and likely being more accurate.

### supports_external_chess_eval
This parameter indicates whether or not the engine supports translating from a board structure defined in the `external` directory. See more at [get_external_eval_result](#get_external_eval_result)

### get_initial_parameters
This function retrieves the initial parameters of the evaluation in a vector form. Each parameter is an entry in `parameters_t`.

If the evaluation used is tapered, each entry is an `std::array<double, 2>`, where the first value is the midgame value and the second value is the endgame value.

If the evaluationis not tapered, the entry is just the plain value of the parameter used in the evaluation.

### get_fen_eval_result
This function gets the linear coefficients for each parameter given a position in a FEN form. Instead of counting a score, count how many times an evaluation term was used for each side in a position in the data set.

The input is a FEN string because it's unreasonable to expect each engine to have the same structure for a board representation, so FEN parsing is left to the engine implementation.

Additionaly, you may return the score, this is used to tune around other existing parameters. 

### get_external_eval_result
Similar to [get_fen_eval_result](get_fen_eval_result), but instead of a FEN it gets a `Chess::Board` as a base parameter. Support for it is not required, but is recommended if tuning with qsearch enabled, because it will greatly increase the data loading speed.

### print_parameters
This function prints the results of the tuning, the input is given as a vector of the tuned parameters, and it's up to the engine to ptint it as as it desires.

## config.h

### thread_count
Maximum number of how many threads various tuning operations will take. Recommended to set to the amount of physical cores on the system the tuner is being run on.

### preferred_k
`K` is a scaling parameter, the lower the `K`, the higher the tuned evaluation scores will be overall. Setting `preferred_k = 0` will make the tuner try to auto-determine the optimal `K` in order to preserve the same scale as the existing eval terms.

Setting `preferred_k = 0` is not compatible with `retune_from_zero = true`.

### max_epoch
Maximum limit of how many epochs (iterations over the whole dataset) to run. Could be useful if for example you only ever run 5000 epochs, to keep the tuning consistent.

### retune_from_zero
If set to `true`, tuning will start with all evaluation terms set to `0`. It is still needed to implement [get_initial_parameters](#get_initial_parameters) in the evaluation class, even it is set to `true`. Setting it to `false` will make the tuner start with the current evaluation terms.

### enable_qsearch
If set to `true`, will use [quiescence search](https://www.chessprogramming.org/Quiescence_Search) when loading each entry from the data set, in order to get to quiet positions (positions where the best move is not a capture). When tuning with already only quiet positions this will have a minimal effect on the tuning outcome.

If set to `true`, data loading will be considerably slower. This can be mitigated by implementing [get_external_eval_result](#get_external_eval_result) in the evaluation class and setting [supports_external_chess_eval](#supports_external_chess_eval) to `true`, however the data loading will still be slower.

### print_data_entries
If set to `true`, will print information about each entry while loading the data set. Should only enable if debugging.

### data_load_print_interval
How often to print progress while loading data.

## Build
Cmake / make // TODO


## Data sources
This tuner does not provide data sources. Own data source must be used.
Each data source is a file where each line is a FEN and a WDL. (WDL = Win/Draw/Loss).

Supported formats:
1. Line containing WDL. Markers are `1.0`, `0.5`, and `0.0` for win, draw loss. Example:
```
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1; [1.0]
```
2. Line containing match result. Markers are `1-0`, `1/2-1/2`, and `0-1` for win, draw loss. Example:
```
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1; [1-0]
```
3. Line containing probability to win. Pretty much equivalent to option nr.1, except it doesn't need to be stricly win/draw/loss scores . Example:
```
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1; [0.6]
```

The brackets are not necessary, the WDL only has to be found somewhere in the line.

## Usage
Create a csv formatted file with data sources. `#` marks a comment line.

Columns:
1. Path to data file.
2. Whether or not the WDL is from the side playing. 1 = yes, 0 = no,
3. Limit of how may FENs to load from this data source (sequentially). 0 = unlimited

Example:
```
# Path, WDL from side playing, position limit
C:\Data1.epd,0,0
C:\Data2.epd,0,900000
```

Build the project and run `tuner.exe sources.csv` where sources.csv is the data source file mentioned previously.