import sys, os, random

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5 import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure

import numpy as np
import pixeldata

simulateEnable = True

class AppForm(QMainWindow):
    def __init__(self, parent=None):
        QMainWindow.__init__(self, parent)
        self.setWindowTitle('Demo: PyQt with matplotlib')

        self.create_menu()
        self.create_main_frame()
        self.create_status_bar()

        self.textbox.setText('100')
        self.pd = pixeldata.PixelData()
        self.on_draw()

        #Updating State
        self.timer = QTimer()
        self.timer.start(10)
        self.timer.timeout.connect(self.on_draw)
        self.timer.timeout.connect(self.update)

    def save_plot(self):
        file_choices = "PNG (*.png)|*.png"

        path = QFileDialog.getSaveFileName(self,
                        'Save file', '',
                        file_choices)
        if path:
            self.canvas.print_figure(path, dpi=self.dpi)
            self.statusBar().showMessage('Saved to %s' % path, 2000)

    def on_about(self):
        msg = """ A demo of using PyQt with matplotlib:

         * Use the matplotlib navigation bar
         * Add values to the text box and press Enter (or click "Draw")
         * Show or hide the grid
         * Drag the slider to modify the width of the bars
         * Save the plot to a file using the File menu
         * Click on a bar to receive an informative message
        """
        QMessageBox.about(self, "About the demo", msg.strip())

    def on_pick(self, event):
        # The event received here is of the type
        # matplotlib.backend_bases.PickEvent
        #
        # It carries lots of information, of which we're using
        # only a small amount here.
        #
        box_points = event.artist.get_bbox().get_points()
        msg = "You've clicked on a bar with coords:\n %s" % box_points

        QMessageBox.information(self, "Click!", msg)

    def on_draw(self):
        """ Redraws the figure
        """
        #str = unicode(self.textbox.text())
        #self.data = list(map(int, self.textbox.text().split()))

        #x = range(len(self.data))

        # clear the axes and redraw the plot anew
        #
        self.axes.clear()
        self.axes.grid(self.grid_cb.isChecked())

        #self.axes.bar(
        #    left=x,
        #    height=self.data,
        #    width=self.slider.value() / 100.0,
        #    align='center',
        #    alpha=0.44,
        #    picker=5)

        #self.data = int(self.textbox.text())
        #x = np.random.standard_normal(self.data)
        #y = 2.0 + 3.0 * x + 4.0 * np.random.standard_normal(self.data)
        #self.axes.hexbin(x,y)
        self.nevent = int(self.textbox.text())
        self.pd.simulate(self.nevent)
        self.axes.imshow(self.pd.pixelevent[self.nevent-1])

        self.canvas.draw()

    def create_main_frame(self):
        self.main_frame = QWidget()

        # Create the mpl Figure and FigCanvas objects.
        # 5x4 inches, 100 dots-per-inch
        #
        self.dpi = 100
        self.fig = Figure((5.0, 4.0), dpi=self.dpi)
        self.canvas = FigureCanvas(self.fig)
        self.canvas.setParent(self.main_frame)

        # Since we have only one plot, we can use add_axes
        # instead of add_subplot, but then the subplot
        # configuration tool in the navigation toolbar wouldn't
        # work.
        #
        self.axes = self.fig.add_subplot(111)

        # Bind the 'pick' event for clicking on one of the bars
        #
        self.canvas.mpl_connect('pick_event', self.on_pick)

        # Create the navigation toolbar, tied to the canvas
        #
        self.mpl_toolbar = NavigationToolbar(self.canvas, self.main_frame)

        # Other GUI controls
        #
        self.textbox = QLineEdit()
        self.textbox.setMinimumWidth(200)
        self.textbox.editingFinished.connect(self.on_draw)

        self.draw_button = QPushButton("&Draw")
        self.draw_button.clicked.connect(self.on_draw)


        self.grid_cb = QCheckBox("Show &Grid")
        self.grid_cb.setChecked(False)
        self.grid_cb.stateChanged.connect(self.on_draw) #int

        slider_label = QLabel('Bar width (%):')
        self.slider = QSlider(Qt.Horizontal)
        self.slider.setRange(1, 100)
        self.slider.setValue(20)
        self.slider.setTracking(True)
        self.slider.setTickPosition(QSlider.TicksBothSides)
        self.slider.valueChanged.connect(self.on_draw)#int

        #
        # Layout with box sizers
        #
        hbox = QHBoxLayout()

        for w in [  self.textbox, self.draw_button, self.grid_cb,
                  slider_label, self.slider]:
            hbox.addWidget(w)
            hbox.setAlignment(w, Qt.AlignVCenter)

        vbox = QVBoxLayout()
        vbox.addWidget(self.mpl_toolbar)
        vbox.addWidget(self.canvas)
        vbox.addLayout(hbox)

        self.main_frame.setLayout(vbox)
        self.setCentralWidget(self.main_frame)

    def create_status_bar(self):
        self.status_text = QLabel("This is a demo")
        self.statusBar().addWidget(self.status_text, 1)

    def create_menu(self):
        self.file_menu = self.menuBar().addMenu("&File")

        load_file_action = self.create_action("&Save plot",
                                              shortcut="Ctrl+S", slot=self.save_plot,
                                              tip="Save the plot")
        quit_action = self.create_action("&Quit", slot=self.close,
                                         shortcut="Ctrl+Q", tip="Close the application")

        self.add_actions(self.file_menu,
                         (load_file_action, None, quit_action))

        self.help_menu = self.menuBar().addMenu("&Help")
        about_action = self.create_action("&About",
                                          shortcut='F1', slot=self.on_about,
                                          tip='About the demo')

        self.add_actions(self.help_menu, (about_action,))

    def add_actions(self, target, actions):
        for action in actions:
            if action is None:
                target.addSeparator()
            else:
                target.addAction(action)

    def create_action(  self, text, slot=None, shortcut=None,
                        icon=None, tip=None, checkable=False,
                        signal="triggered()"):
        action = QAction(text, self)
        if icon is not None:
            action.setIcon(QIcon(":/%s.png" % icon))
        if shortcut is not None:
            action.setShortcut(shortcut)
        if tip is not None:
            action.setToolTip(tip)
            action.setStatusTip(tip)
        if slot is not None:
            action.triggered.connect(slot)
        if checkable:
            action.setCheckable(True)
        return action

    def update(self):
        upnevent = int(self.textbox.text())
        if simulateEnable:
            self.textbox.setText(str(upnevent+1))

def main():
    app = QApplication(sys.argv)
    form = AppForm()
    form.show()
    app.exec_()

if __name__ == "__main__":
    main()
