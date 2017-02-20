// Copyright (c) 2010-2017 Fabric Software Inc. All rights reserved.

#include "DFGTabSearchWidget.h"

#include <FTL/JSONValue.h>
#include <QLayout>

using namespace FabricUI::DFG;

DFGTabSearchWidget2::DFGTabSearchWidget2( FabricCore::DFGHost* host )
  : m_host( host )
{
  this->setWindowFlags( Qt::Popup );

  QVBoxLayout* layout = new QVBoxLayout();

  m_queryEdit = new TabSearch::QueryEdit();
  layout->addWidget( m_queryEdit );
  m_queryEdit->setFocus();
  connect( m_queryEdit, SIGNAL( queryChanged( QString ) ),
    this, SLOT( onQueryChanged( QString ) ) );

  m_resultsView = new TabSearch::ResultsView();
  layout->addWidget( m_resultsView );

  layout->setMargin( 0 );
  this->setLayout( layout );
}

void DFGTabSearchWidget2::showForSearch( QPoint globalPos )
{
  move( mapFromGlobal( globalPos ) );

  emit enabled( true );
  show();
}

void DFGTabSearchWidget2::onQueryChanged( QString query )
{
  // Splitting the search string into a char**
  const std::string searchStr = query.toStdString().data();

  std::vector<std::string> tagsStr;
  unsigned int start = 0;
  for( unsigned int end = 0; end < searchStr.size(); end++ )
  {
    const char c = searchStr[end];
    if( c == '.' || c == ' ' ) // delimiters
    {
      if( end - start > 0 )
        tagsStr.push_back( searchStr.substr( start, end - start ) );
      start = end + 1;
    }
  }
  if( start < searchStr.size() )
    tagsStr.push_back( searchStr.substr( start, searchStr.size() - start ) );

  std::vector<char const*> tags( tagsStr.size() );

  // Debug : TODO remove
  for( unsigned int i = 0; i < tagsStr.size(); i++ )
    std::cout << "\"" << tagsStr[i] << "\" ";
  std::cout << std::endl;

  for( unsigned int i = 0; i < tagsStr.size(); i++ )
    tags[i] = tagsStr[i].data();

  // Querying the DataBase of presets
  FabricCore::DFGHost* host = m_host;
  FEC_StringRef jsonStr = FEC_DFGHostSearchPresets(
    host->getFECDFGHostRef(),
    tags.size(),
    tags.data(),
    0,
    16
  );
  FTL::StrRef jsonStrR( FEC_StringGetCStr( jsonStr ), FEC_StringGetSize( jsonStr ) );
  const FTL::JSONValue* json = FTL::JSONValue::Decode( jsonStrR );
  const FTL::JSONObject* root = json->cast<FTL::JSONObject>();
  const FTL::JSONArray* resultsJson = root->getArray( "results" );

  std::vector<std::string> results;
  for( unsigned int i = 0; i < resultsJson->size(); i++ )
    results.push_back( resultsJson->getArray( i )->getString( 0 ) );

  m_resultsView->setResults( results );
}

