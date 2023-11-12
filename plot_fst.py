import os
import argparse
import subprocess
from graphviz import Digraph

BIN_FOLDER = "bin"
CPP_BIN = "./write_fst"
OUTPUT_TXT_FILE = "graph.txt"
VALID_TO_FILLED = {
    "0": None,
    "1": "filled",
}
# Parse arguments
parser = argparse.ArgumentParser()
parser.add_argument(
    "--input_dict_file", help="File with FST dictionary", type=str, required=True
)
args = parser.parse_args()

input_dict_file = args.input_dict_file
output_image_file = f"output_graph_{input_dict_file.split('.')[0].split('/')[-1]}"
# Generate txt file with graph transitions,
# In format: "start_node_idx, end_node_idx, transition_label"
os.chdir(BIN_FOLDER)
process = subprocess.run(
    [CPP_BIN, f"./../{input_dict_file}", f"./../{OUTPUT_TXT_FILE}"]
)
os.chdir("..")

# Read graph from txt file
graph = Digraph(output_image_file, filename=output_image_file, format="png")
with open(OUTPUT_TXT_FILE, "r") as f:
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

# Remove unnecessary files
os.remove(OUTPUT_TXT_FILE)
os.remove(output_image_file)
