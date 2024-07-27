from matplotlib import pyplot as plt
from matplotlib.backends.backend_agg import FigureCanvasAgg


class Plotter:
    def __init__(self, freq=1):
        plt.ion()

        self.fig = plt.figure()

        self.subplots = []
        self.x = []
        self.plot_arrays = []

    def add_graph(self, title, x_label, y_label):
        self.plot_arrays.append([])
        self.subplots.append(self.fig.add_subplot(
            2, 2, len(self.subplots) + 1))
        self.subplots[-1].set_title(title)
        self.x.append([])

    def add_plot(self, graph_index):
        self.plot_arrays[graph_index].append([])

    def start(self):
        pass

    def update(self):
        for i in range(0, len(self.subplots)):
            for plot_data in self.plot_arrays[i]:
                self.subplots[i].plot(self.x[i], plot_data, "-")

        self.fig.canvas.draw()
        self.fig.canvas.flush_events()

    def add_value_x(self, val):
        for i in range(0, len(self.x)):
            self.x[i].append(val)

    def add_values(self, graph_index, vals):
        for i in range(0, len(self.plot_arrays[graph_index])):
            self.plot_arrays[graph_index][i].append(vals[i])
