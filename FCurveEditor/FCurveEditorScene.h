//
// Copyright (c) 2010-2017 Fabric Software Inc. All rights reserved.
//

#ifndef FABRICUI_FCURVEEDITOR_FCURVEEDITORSCENE_H
#define FABRICUI_FCURVEEDITOR_FCURVEEDITORSCENE_H

#include <QGraphicsScene>
#include <FTL/Config.h>

namespace FabricUI
{
namespace FCurveEditor
{

class FCurveItem;
class AbstractFCurveModel;

enum Mode { SELECT, ADD, REMOVE, MODE_COUNT };

/*
  An FCurveEditorScene is the main scene of an FCurveEditor.
  It can contain one or several FCurveItems, and stores the current
  editing state.
  Several FCurveEditors can use the same scene, but they will share the
  same editing states (only the view matrix will change).
*/
class FCurveEditorScene : public QGraphicsScene
{
  Q_OBJECT

  typedef QGraphicsScene Parent;

public:

  FCurveEditorScene( AbstractFCurveModel* );
  void setMode( Mode m );
  inline Mode mode() const { return m_mode; }
  inline FCurveItem* curveItem() { return m_curveItem; }

private:
  Mode m_mode;
  bool m_isDraggingKey;

  // TODO : replace by an array of FCurveItems
  FCurveItem* m_curveItem;

protected:
  void mousePressEvent( QGraphicsSceneMouseEvent * ) FTL_OVERRIDE;
  void mouseMoveEvent( QGraphicsSceneMouseEvent * ) FTL_OVERRIDE;
  void mouseReleaseEvent( QGraphicsSceneMouseEvent * ) FTL_OVERRIDE;

signals:
  void modeChanged() const;
  void interactionBegin();
  void interactionEnd();
};

} // namespace FCurveEditor
} // namespace FabricUI

#endif // FABRICUI_FCURVEEDITOR_FCURVEEDITORSCENE_H
