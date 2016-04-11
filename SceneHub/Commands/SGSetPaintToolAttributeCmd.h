/*
 *  Copyright 2010-2016 Fabric Software Inc. All rights reserved.
 */

#ifndef __UI_SCENEHUB_SGSETPAINTTOOLATTRIBUTECMD_H__
#define __UI_SCENEHUB_SGSETPAINTTOOLATTRIBUTECMD_H__
 
#include "SHCmd.h"
using namespace std;
using namespace FabricCore;
 
namespace FabricUI {
namespace SceneHub {

class SGSetPaintToolAttributeCmd : public SHCmd {

  public: 
    SGSetPaintToolAttributeCmd() : SHCmd() {
      m_desctiption.cmdName = "setPaintToolAttributeCmd";
      m_desctiption.cmdType = "SGSetPaintToolAttributeCmd";
    }

    virtual SHCmdDescription registerCommand() {
      if(QMetaType::type(m_desctiption.cmdType.toUtf8().constData()) == 0)
        qRegisterMetaType<FabricUI::SceneHub::SGSetPaintToolAttributeCmd>(m_desctiption.cmdType.toUtf8().constData());
      return SHCmd::registerCommand();
    }

    virtual QString getFromRTVal(RTVal sgCmd) {
      QString cmd;
      try 
      {
        RTVal keyVal = RTVal::ConstructString(m_shGLScene->getClient(), "fullPath");
        RTVal fullPathVal = sgCmd.callMethod("String", "getStringParam", 1, &keyVal);
        QString fullPath = QString(fullPathVal.getStringCString());
        cmd = QString( m_desctiption.cmdName + "(" + fullPath + ")" );
      }
      catch(Exception e)
      {
        printf("SGSetPaintToolAttributeCmd::Get: exception: %s\n", e.getDesc_cstr());
      }
      return cmd;
    }
};

} // SceneHub
} // FabricUI

#endif // __UI_SCENEHUB_SGSETPAINTTOOLATTRIBUTECMD_H__