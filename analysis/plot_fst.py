import os
import argparse
from graphviz import Digraph

"""
Script for visualizing FST graph.
"""
BIN_FOLDER = "bin"
CPP_BIN = "./write_fst"
OUTPUT_DIR = "output_files"
VALID_TO_FILLED = {
    "0": None,
    "1": "filled",
}
# Parse arguments
parser = argparse.ArgumentParser()
parser.add_argument(
    "--input_graph_file", help="File with FST dictionary", type=str, required=True
)
args = parser.parse_args()

input_graph_file = args.input_graph_file
output_image_file = f"output_graph_{input_graph_file.split('/')[-1].split('.')[0]}"
print(output_image_file)
# Generate txt file with graph transitions,
# In format: "start_node_idx, end_node_idx, transition_label, start_node_valid, end_node_valid"

# Read graph from txt file
graph = Digraph(output_image_file, filename=f"../{OUTPUT_DIR}/{output_image_file}", format="png")
with open(input_graph_file, "r") as f:
    for line in f:
        line = line.strip()
        if line:
            elems = [x for x in line.split() if x]
            start_node, end_node, transition, base_node_valid, next_node_valid = elems
            graph.node(start_node, start_node, style=VALID_TO_FILLED[base_node_valid])
            graph.node(end_node, end_node, style=VALID_TO_FILLED[next_node_valid])
            graph.edge(start_node, end_node, label=transition)

# Generate graph visualization
graph.render(view=True)