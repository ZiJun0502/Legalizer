import matplotlib.pyplot as plt
from tqdm import tqdm

# Function to parse draw.txt
def parse_draw_file(file_name):
    die_size = None
    cells = []

    with open(file_name, 'r') as file:
        lines = file.readlines()
        
        # Parse DieSize
        die_size_line = lines[0].strip().split()
        die_size = {
            'x0': float(die_size_line[1]),
            'y0': float(die_size_line[2]),
            'x1': float(die_size_line[3]),
            'y1': float(die_size_line[4]),
        }
        
        # Parse cells
        for line in lines[1:]:
            cell_data = line.strip().split()
            cell = {
                'name': cell_data[0],
                'x': float(cell_data[1]),
                'y': float(cell_data[2]),
                'width': float(cell_data[3]),
                'height': float(cell_data[4]),
            }
            cells.append(cell)
    
    return die_size, cells

# Function to plot the die and cells
def plot_die_and_cells(die_size, cells, output_filename='layout.png'):
    fig, ax = plt.subplots()

    # Plot the Die size (the overall bounding box)
    ax.add_patch(plt.Rectangle((die_size['x0'], die_size['y0']), 
                               die_size['x1'] - die_size['x0'], 
                               die_size['y1'] - die_size['y0'], 
                               linewidth=1, edgecolor='black', facecolor='none'))
    
    # Plot each cell as a rectangle inside the die
    for cell in tqdm(cells):
        ax.add_patch(plt.Rectangle((cell['x'], cell['y']), 
                                   cell['width'], cell['height'], 
                                   linewidth=1, edgecolor='blue', facecolor='lightblue'))
        ax.text(cell['x'] + cell['width']/2, cell['y'] + cell['height']/2, 
                cell['name'], color='black', ha='center', va='center')

    # Set plot limits based on die size
    ax.set_xlim(die_size['x0'], die_size['x1'])
    ax.set_ylim(die_size['y0'], die_size['y1'])
    ax.set_aspect('equal', 'box')
    
    # Save the plot to a PNG file
    plt.savefig(output_filename, dpi=300)
    plt.close()  # Close the plot to free up memory

# Main function to run the script
def main():
    # Replace 'draw.txt' with the path to your actual file
    file_name = 'draw.txt'
    output_filename = 'layout.png'  # Output filename for the PNG image
    
    die_size, cells = parse_draw_file(file_name)
    plot_die_and_cells(die_size, cells, output_filename)

if __name__ == "__main__":
    main()
