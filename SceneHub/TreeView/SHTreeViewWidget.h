/*
 *  Copyright 2010-2016 Fabric Software Inc. All rights reserved.
 */

#ifndef __UI_SCENEHUB_TREE_VIEW_WIDGET_H__
#define __UI_SCENEHUB_TREE_VIEW_WIDGET_H__

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <FabricUI/DFG/DFGController.h>
#include <FabricUI/SceneHub/SHGLScene.h>
#include <FabricUI/SceneHub/TreeView/SHTreeView.h>

namespace FabricUI
{
  namespace SceneHub
  {

    class SHTreeComboBox : public QComboBox {

      Q_OBJECT 
      
      public:
        SHTreeComboBox(QWidget *parent = 0) : QComboBox(parent){}
        
        virtual ~SHTreeComboBox() {}
 
        virtual void showPopup() {
          emit updateList();
          QComboBox::showPopup();
        }

      signals:
        void updateList();
    };

    class SHTreeViewWidget : public QWidget { 

      Q_OBJECT

      public:
        SHTreeViewWidget(SHGLScene *shGLScene, DFG::DFGController *controller, QWidget *parent = 0);

        SHTreeViewWidget(FabricCore::Client client, DFG::DFGController *controller, QWidget *parent = 0);

        ~SHTreeViewWidget() {}
        
        SHTreeView * getTreeView() { return m_shTreeView; }
        
        SHGLScene* getScene() { return m_shGLScene; }
  
        void setShowProperties( bool show );
        
        void setShowOperators( bool show );
  
  
      public slots:
        void onUpdateScene(const QString &sceneName);

        void onUpdateSceneList();

        void expandTree(uint32_t level);
        
        /// Calls when the SceneGraph hierachy changed.
        void onSceneHierarchyChanged();
        
        /// Updates the application when an item of the treeView is selected.
        /// \param item The selected item.
        void treeItemSelected(FabricUI::SceneHub::SHTreeItem *item);
        
        /// Updates the application when an item of the treeView is deselected.
        /// \param item The deselected item.
        void treeItemDeselected(FabricUI::SceneHub::SHTreeItem *item);
        
        /// Updates the treeView from the 3D scene selection.
        void updateFrom3DSelection();


      signals:
        void sceneHierarchyChanged();
  
        void sceneUpdated(FabricUI::SceneHub::SHGLScene *);


      private:
        void init();
        void resetTree();
        void constructTree();

	      FabricCore::Client m_client;
        SHGLScene *m_shGLScene;
        SHTreeView *m_shTreeView;
        SHGLScene *m_mainSHGLScene;
        SHTreeModel *m_treeModel;
        DFG::DFGController *m_controller;

        SHTreeComboBox *m_comboBox;
        bool m_bUpdatingSelectionFrom3D;
        bool m_showProperties;
        bool m_showOperators;

    };

  }
}

#endif //__UI_SCENEHUB_TREE_VIEW_WIDGET_H__