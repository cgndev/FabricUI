
from PySide import QtCore, QtGui
from FabricEngine import Core, FabricUI
from FabricEngine.FabricUI import *
from SHBaseMenu import SHBaseMenu

class SHTreeViewMenu(SHBaseMenu):

  def __init__(self, treeViewWidget, title = None, parent = None):
    self.shTreeViewWidget = treeViewWidget
    super(SHTreeViewMenu, self).__init__(None, "Tree View", parent)
 
  def _constructMenu(self):
    self.showProperty = self.addAction("Show properties")
    self.addSeparator()
    self.showPropertyGenerator = self.addAction("Show property generators")

    self.showProperty.triggered.connect(self.updateTreeViewVisibility)
    self.showPropertyGenerator.triggered.connect(self.updateTreeViewVisibility)
 
  def updateTreeViewVisibility(self):   
    self.shTreeViewWidget.setShowProperties(self.showProperty.isChecked())
    self.shTreeViewWidget.setShowOperators(self.showPropertyGenerator.isChecked())