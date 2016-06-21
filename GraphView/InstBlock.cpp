//
// Copyright (c) 2010-2016, Fabric Software Inc. All rights reserved.
//

#include <FabricUI/GraphView/Graph.h>
#include <FabricUI/GraphView/GraphConfig.h>
#include <FabricUI/GraphView/InstBlock.h>
#include <FabricUI/GraphView/InstBlockPort.h>
#include <FabricUI/GraphView/InstBlockHeader.h>

#include <QtCore/QDebug>
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QPainter>

namespace FabricUI {
namespace GraphView {

InstBlock::InstBlock(
  Node *node,
  FTL::StrRef name
  )
  : m_node( node )
  , m_name( name )
{
  Graph const *graph = node->graph();
  GraphConfig const &config = graph->config();

  m_pinRadius = config.pinRadius;

  m_layout = new QGraphicsLinearLayout();
  m_layout->setContentsMargins( 0, 0, 0, 0 );
  m_layout->setSpacing( 2 );
  m_layout->setOrientation( Qt::Vertical );

  m_instBlockHeader = new InstBlockHeader( this, name );
  m_layout->addItem( m_instBlockHeader );
  m_layout->setAlignment( m_instBlockHeader, Qt::AlignHCenter | Qt::AlignVCenter );

  setContentsMargins( 0, 1, 0, 1 );
  setMinimumWidth( config.instBlockMinWidth );
  setMinimumHeight( config.instBlockMinHeight );
  setSizePolicy( QSizePolicy(
    QSizePolicy::MinimumExpanding,
    QSizePolicy::MinimumExpanding
    ) );
  setLayout( m_layout );


  // if ( !isBackDropNode() )
  // {
  //   QObject::connect(m_header, SIGNAL(headerButtonTriggered(FabricUI::GraphView::NodeHeaderButton*)), 
  //     m_graph->controller(), SLOT(onNodeHeaderButtonTriggered(FabricUI::GraphView::NodeHeaderButton*)));
  // }

  // m_pinsWidget = new QGraphicsWidget(rectangle);
  // layout->addItem(m_pinsWidget);
  // layout->setAlignment(m_pinsWidget, Qt::AlignHCenter | Qt::AlignVCenter);

  // m_pinsLayout = new QGraphicsLinearLayout();
  // m_pinsLayout->setOrientation(Qt::Vertical);
  // m_pinsLayout->setContentsMargins(0, m_graph->config().nodeSpaceAbovePorts, 0, m_graph->config().nodeSpaceBelowPorts);
  // m_pinsLayout->setSpacing(m_graph->config().nodePinSpacing);
  // m_pinsWidget->setLayout(m_pinsLayout);
}

std::string InstBlock::path() const
{
  std::string result = m_node->name();
  result += '.';
  result += m_name;
  return result;
}

void InstBlock::insertInstBlockPortAtIndex(
  unsigned index,
  InstBlockPort *instBlockPort
  )
{
  m_instBlockPorts.insert( m_instBlockPorts.begin() + index, instBlockPort );
  m_layout->insertItem( 1 + index, instBlockPort );
}

void InstBlock::reorderInstBlockPorts(
  FTL::ArrayRef<unsigned> newOrder
  )
{
  assert( newOrder.size() == m_instBlockPorts.size() );
  InstBlockPortVec newInstBlockPorts;
  newInstBlockPorts.resize( newOrder.size() );
  for ( unsigned i = 0; i < m_instBlockPorts.size(); ++i )
    newInstBlockPorts[i] = m_instBlockPorts[newOrder[i]];
  newInstBlockPorts.swap( m_instBlockPorts );

  while ( m_layout->count() > 1 )
    m_layout->removeAt( m_layout->count() - 1 );
  for ( unsigned i = 0; i < m_instBlockPorts.size(); ++i )
    m_layout->insertItem( 1 + i, m_instBlockPorts[i] );
}

void InstBlock::removeInstBlockPortAtIndex(
  unsigned index
  )
{
  m_layout->removeAt( 1 + index );
  InstBlockPort *instBlockPort = m_instBlockPorts[index];
  m_instBlockPorts.erase( m_instBlockPorts.begin() + index );
  instBlockPort->deleteLater();
}

void InstBlock::paint(
  QPainter *painter,
  QStyleOptionGraphicsItem const *option,
  QWidget *widget
  )
{
  QPen pen;
  if ( m_node->selected() )
    pen = m_node->selectedPen();
  else
    pen = m_node->defaultPen();
  painter->setPen( pen );
  
  QRectF rect = contentsRect();
  rect.adjust( m_pinRadius, 0, -m_pinRadius, 0 );

  QRectF headerRect = rect;
  qreal headerHeight = m_instBlockHeader->boundingRect().height();
  headerRect.setBottom( headerHeight );
  headerRect.adjust( 0, pen.width() * 0.5f, 0, pen.width() * 0.5f );
  painter->setBrush( m_node->titleColor() );
  painter->drawRect( headerRect );

  // QRectF portsRect = rect;
  // portsRect.setTop( headerRect.bottom() );
  // painter->setPen( QPen() );
  // painter->setBrush( m_node->color() );
  // painter->drawRect( portsRect );

  QGraphicsWidget::paint(painter, option, widget);
}

void InstBlock::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  if ( node()->onMousePress(
    event->button(),
    event->modifiers(),
    event->scenePos(),
    event->lastScenePos()
    ) )
  {
    event->accept();
    return;
  }

  QGraphicsWidget::mousePressEvent( event );
}

void InstBlock::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
  if ( event->button() == Qt::LeftButton )
  {
    emit doubleClicked( this, event->button(), event->modifiers() );
    event->accept();
    return;
  }

  QGraphicsWidget::mouseDoubleClickEvent( event );
}

void InstBlock::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
  if ( node()->onMouseMove(
    event->button(),
    event->modifiers(),
    event->scenePos(),
    event->lastScenePos()
    ) )
  {
    event->accept();
    return;
  }

  QGraphicsWidget::mouseMoveEvent(event);
}

void InstBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if ( node()->onMouseRelease(
    event->button(),
    event->modifiers(),
    event->scenePos(),
    event->lastScenePos()
    ) )
  {
    event->accept();
    return;
  }

  QGraphicsWidget::mouseReleaseEvent(event);
}

InstBlockPort *InstBlock::instBlockPort( FTL::StrRef name )
{
  for ( size_t index = 0; index < m_instBlockPorts.size(); ++index )
    if ( name == m_instBlockPorts[index]->name() )
      return m_instBlockPorts[index];
  return NULL;
}

void InstBlock::setName( FTL::StrRef newName )
{
  m_name = newName;
  m_instBlockHeader->setName(
    QString::fromUtf8( newName.data(), newName.size() )
    );
}

} // namespace GraphView
} // namespace FabricUI
