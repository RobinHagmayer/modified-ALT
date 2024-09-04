# modified-ALT
Bachelor Thesis: Efficient route planning in transportation networks using extended triangle inequalities

## Usage:
- Compile the project with cmake/make
- Copy the *.fmi graph files to data/graphs and optionally the *.que and the *.sol files to data/benchs
- Create the necessary binary files for the graphs via `./build/modified-ALT-bench GRAPH_NAME -b`
- Preprocess the graph for ALT and modified-ALT via `./build/modified-ALT-bench GRAPH_NAME -p`
- Optionally create random benchmarks via `./build/modified-ALT-bench GRAPH_NAME -r REQUEST_COUNT`
- Bench the three different algorithms (use `./build/modified-ALT-bench --help` for more info)
