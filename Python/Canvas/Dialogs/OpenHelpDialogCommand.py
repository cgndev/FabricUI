#
# Copyright (c) 2010-2017 Fabric Software Inc. All rights reserved.
#

import os
from PySide import QtCore, QtGui
from FabricEngine.FabricUI import Commands as CppCommands
from FabricEngine.Canvas.Dialogs.HelpDialog import HelpDialog
 
class OpenHelpDialogCommand(CppCommands.BaseRTValScriptableCommand):

    def __init__(self):
        super(OpenHelpDialogCommand, self).__init__()
        self.declareArg("title")
        self.declareArg("text")

    def canUndo(self):
        return False

    def canLog(self):
        return True

    def doIt(self):
        dialog = HelpDialog(self.getArg("title"), self.getArg("text"))
        dialog.show()
        return True
 