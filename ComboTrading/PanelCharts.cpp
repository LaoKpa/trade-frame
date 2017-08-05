/************************************************************************
 * Copyright(c) 2015, One Unified. All rights reserved.                 *
 * email: info@oneunified.net                                           *
 *                                                                      *
 * This file is provided as is WITHOUT ANY WARRANTY                     *
 *  without even the implied warranty of                                *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                *
 *                                                                      *
 * This software may not be used nor distributed without proper license *
 * agreement.                                                           *
 *                                                                      *
 * See the file LICENSE.txt for redistribution information.             *
 ************************************************************************/

// started December 6, 2015, 1:26 PM

#include <vector>
#include <algorithm>

#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/phoenix/core/argument.hpp>

#include <wx/sizer.h>
//#include <wx/dc.h>
#include <wx/icon.h>
#include <wx/menu.h>

#include <TFVuTrading/DialogPickSymbol.h>
#include <TFIQFeed/BuildSymbolName.h>
#include <TFIQFeed/MarketSymbol.h>

#include <TFOptions/Option.h>

#include <TFVuTrading/WinChartView.h>
#include <TFVuTrading/NotebookOptionChains.h>

// need to check why this exists
//#include <wx/toplevel.h>
//#include <wx/sizer.h>
//#include <wx/list.h>

#include "PanelCharts.h"
#include "TreeItemGroup.h"

// 2016/10/16 todo:
//   check serialization of instruments
//   save and retrieve data for charting
//   load data for charting
//   watch/unwatch live data on the charts
//   implement zoom/pan ability on chart

namespace ou { // One Unified
namespace tf { // TradeFrame

PanelCharts::PanelCharts( void ): wxPanel() {
  Init();
}

PanelCharts::PanelCharts( 
  wxWindow* parent, wxWindowID id, 
  const wxPoint& pos, 
  const wxSize& size, 
  long style ): wxPanel()
{
  
  Init();
  Create(parent, id, pos, size, style);
  
}

PanelCharts::~PanelCharts() {
}

void PanelCharts::Init( void ) {
  
  m_panelSplitterRightPanel = 0;
  m_splitter = 0;
  m_sizerRight = 0;
  m_winRightDetail = 0;
  
  m_pTreeOps = 0;
  m_pDialogPickSymbol = 0;
  m_pInstrumentActions.reset( new InstrumentActions );
}

bool PanelCharts::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) {

  wxPanel::Create( parent, id, pos, size, style );

  CreateControls();
  if (GetSizer())     {
      GetSizer()->SetSizeHints(this);
  }
  Centre();
  return true;
}

/*
   m_sizerPM = new wxBoxSizer(wxVERTICAL);
  SetSizer(m_sizerPM);

  //m_scrollPM = new wxScrolledWindow( m_pFPPOE, wxID_ANY, wxDefaultPosition, wxSize(200, 400), wxVSCROLL );
  m_scrollPM = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL );
  m_sizerPM->Add(m_scrollPM, 1, wxGROW|wxALL, 5);
  m_scrollPM->SetScrollbars(1, 1, 0, 0);

  m_sizerScrollPM = new wxBoxSizer(wxVERTICAL);
  m_scrollPM->SetSizer( m_sizerScrollPM );
*/

void PanelCharts::CreateControls() {    

  PanelCharts* itemPanel1 = this;

  wxBoxSizer* sizerMain = new wxBoxSizer(wxVERTICAL);
  itemPanel1->SetSizer(sizerMain);

  m_splitter = new wxSplitterWindow( this );
  m_splitter->SetMinimumPaneSize(10);
  m_splitter->SetSashGravity(0.2);

  // tree
  //wxTreeCtrl* tree;
//  m_pTreeSymbols = new wxTreeCtrl( splitter );
  //wxTreeItemId idRoot = m_pTreeSymbols->AddRoot( "/", -1, -1, new CustomItemData( CustomItemData::Root, m_eLatestDatumType ) );
//  wxTreeItemId idRoot = m_pTreeSymbols->AddRoot( "Collections", -1, -1, new CustomItemBase );
  //m_pHdf5Root->Bind( wxEVT_COMMAND_TREE_SEL_CHANGED, &PanelChartHdf5::HandleTreeEventItemActivated, this, m_pHdf5Root->GetId() );
//  m_pFrameMain->Bind( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, &AppLiveChart::HandleTreeEventItemActivated, this, m_pHdf5Root->GetId()  );
//  m_pFrameMain->Bind( wxEVT_COMMAND_TREE_SEL_CHANGED, &AppLiveChart::HandleTreeEventItemActivated, this, m_pHdf5Root->GetId()  );
  //m_pTreeSymbols->Bind( wxEVT_COMMAND_TREE_ITEM_MENU, &PanelCharts::HandleTreeItemMenu, this, m_pTreeSymbols->GetId()  );
  //m_pTreeSymbols->AppendItem( idRoot, "equities" );
  //m_pTreeSymbols->AppendItem( idRoot, "futures" );
  //m_pTreeSymbols->AppendItem( idRoot, "foptions" );
  //m_pTreeSymbols->AppendItem( idRoot, "portfolios" );
  
  m_pTreeOps = new ou::tf::TreeOps( m_splitter );
  m_pTreeOps->PopulateResources( m_baseResources );
  
  wxTreeItemId item = m_pTreeOps->AddRoot( "Instruments" );  // can be renamed
  boost::shared_ptr<TreeItemRoot> p( new TreeItemRoot( item, m_baseResources, m_resources ) );
  m_pTreeOps->SetRoot( p );
  //m_pTreeItemRoot.reset( new TreeItemRoot( id, m_resources ) );
  //m_mapDecoder.insert( mapDecoder_t::value_type( id.GetID(), m_pTreeItemRoot ) );

  // panel for right side of m_splitter
  m_panelSplitterRightPanel = new wxPanel( m_splitter );

  m_splitter->SplitVertically( m_pTreeOps, m_panelSplitterRightPanel, 0 );
  sizerMain->Add( m_splitter, 1, wxGROW|wxALL, 5 );

  // sizer for right side of m_splitter
  m_sizerRight = new wxBoxSizer( wxVERTICAL );
  m_panelSplitterRightPanel->SetSizer( m_sizerRight );

  // initialize the tree
  //m_pHdf5Root->DeleteChildren( m_pHdf5Root->GetRootItem() );

  // close doesn't seem to be hit with panels
  //Bind( wxEVT_CLOSE_WINDOW, &PanelCharts::OnClose, this );  // start close of windows and controls
  Bind( wxEVT_DESTROY, &PanelCharts::OnWindowDestroy, this );
  
  // maybe use std::bind now
  namespace args = boost::phoenix::arg_names;
  
  m_connGetInstrumentActions = m_resources.signalGetInstrumentActions.connect( boost::phoenix::bind( &PanelCharts::HandleGetInstrumentActions, this, args::arg1 ) );
  
  m_connNewInstrument = m_pInstrumentActions->signalNewInstrument.connect( boost::phoenix::bind( &PanelCharts::HandleNewInstrumentRequest, this, args::arg1, args::arg2, args::arg3 ) );
  m_connLoadInstrument = m_pInstrumentActions->signalLoadInstrument.connect( boost::phoenix::bind( &PanelCharts::HandleLoadInstrument, this, args::arg1, args::arg2, args::arg3 ) );
  m_connEmitValues = m_pInstrumentActions->signalEmitValues.connect( boost::phoenix::bind( &PanelCharts::HandleEmitValues, this, args::arg1 ) );
  m_connLiveChart = m_pInstrumentActions->signalLiveChart.connect( boost::phoenix::bind( &PanelCharts::HandleInstrumentLiveChart, this, args::arg1 ) );
  m_connOptionList = m_pInstrumentActions->signalOptionList.connect( boost::phoenix::bind( &PanelCharts::HandleOptionChainList, this, args::arg1 ) );
  m_connDelete = m_pInstrumentActions->signalDelete.connect( boost::phoenix::bind( &PanelCharts::HandleMenuItemDelete, this, args::arg1 ) );
  
  m_connLookupDescription = m_de.signalLookupDescription.connect( boost::phoenix::bind( &PanelCharts::HandleLookUpDescription, this, args::arg1, args::arg2 ) );
  m_connComposeComposite = m_de.signalComposeComposite.connect( boost::phoenix::bind( &PanelCharts::HandleComposeComposite, this, args::arg1 ) );
  
  m_connChanging = m_pTreeOps->signalChanging.connect( boost::phoenix::bind( &PanelCharts::HandleTreeOpsChanging, this, args::arg1 ) );
  
}

void PanelCharts::SetProviders( pProvider_t pData1Provider, pProvider_t pData2Provider, pProvider_t pExecutionProvider ) {
  bool bChangedData1Provider( m_pData1Provider.get() != pData1Provider.get() );
  m_pData1Provider = pData1Provider;
  m_pData2Provider = pData2Provider;
  m_pExecutionProvider = pExecutionProvider;
  if ( bChangedData1Provider ) {
    std::for_each( m_mapInstrumentEntry.begin(), m_mapInstrumentEntry.end(),
      [this](mapInstrumentEntry_t::value_type& vt){
        vt.second.m_pWatchInfo.reset();  // clear out existing WatchInfo, as it depends upon provider type
        vt.second.m_mapSelectedChainOptions.clear();  // clear out existing chains, as they depend upon provider type
        vt.second.m_pWatch->SetProvider( m_pData1Provider ); // set new provider
        vt.second.m_pWatchInfo.reset( new WatchInfo( vt.second.m_pWatch ) ); // build new WatchInfo
      }
      );
  }
}

void PanelCharts::CalcIV( boost::posix_time::ptime dtUtcNow, ou::tf::LiborFromIQFeed& libor ) {
  std::for_each( m_mapInstrumentEntry.begin(), m_mapInstrumentEntry.end(), 
    [this,dtUtcNow,&libor](mapInstrumentEntry_t::value_type& vt){
      InstrumentEntry& entry( vt.second );
      if ( entry.m_pWatch->Watching() ) {
        if ( entry.m_pWatch->GetInstrument()->IsOption() || entry.m_pWatch->GetInstrument()->IsFuturesOption() ) {
          if ( 0 != entry.m_pWatchUnderlying.use_count() ) {
            if ( entry.m_pWatchUnderlying->Watching() ) {
              ou::tf::option::binomial::structInput input;
              ou::tf::option::Option* pOption = dynamic_cast<ou::tf::option::Option*>( entry.m_pWatch.get() );
              ou::tf::Quote quoteUnderlying = entry.m_pWatchUnderlying->LastQuote();
              if ( quoteUnderlying.IsValid() ) {  // should also check for a valid range
                input.S = quoteUnderlying.Midpoint();
                pOption->CalcRate( input, dtUtcNow, libor );  // can this be moved out of the loop? -- no, depends upon expiry
                pOption->CalcGreeks( input, dtUtcNow, true );
              }
            }
          }
        }
        else { // no option or foption
          if ( 0 != entry.m_mapSelectedChainOptions.size() ) {
            ou::tf::option::binomial::structInput input;
            input.S = entry.m_pWatch->LastQuote().Midpoint();
            std::for_each( entry.m_mapSelectedChainOptions.begin(), entry.m_mapSelectedChainOptions.end(), 
              [this,dtUtcNow,&libor,&input](mapOption_t::value_type& vtOption) {
                ou::tf::option::Option::pOption_t pOption( vtOption.second );
                if ( pOption->Watching() ) {
                  pOption->CalcRate( input, dtUtcNow, libor );
                  pOption->CalcGreeks( input, dtUtcNow, true );
                }
              });
          }
        }
      }
  });
}

void PanelCharts::RemoveRightDetail() {
  auto winRightDetail = m_winRightDetail;
  if ( 0 != winRightDetail ) {
    assert( 0 != winRightDetail );
    dynamic_cast<InterfaceBoundEvents*>( winRightDetail )->UnbindEvents(); 
    // perform this afterwards as the DynamicEventTable seems to have problems
    CallAfter([this,winRightDetail](){ 
      assert( 0 != winRightDetail );
      m_sizerRight->Detach( winRightDetail );
      assert( winRightDetail->Destroy() );
    } );
    m_winRightDetail = 0; 
  }
}


void PanelCharts::ReplaceRightDetail( wxWindow* pWindow ) {
  assert( 0 != pWindow );
  CallAfter([this,pWindow](){ // Delayed actions in RemoveRightDetail, so same delay here
    assert( 0 == m_winRightDetail );
    m_winRightDetail = pWindow;
    m_sizerRight->Add( pWindow, 1, wxALL|wxEXPAND, 5);
    m_sizerRight->Layout(); 
  });
}

void PanelCharts::HandleTreeOpsChanging( wxTreeItemId item ) {
  RemoveRightDetail();
}

// TODO: has various indirect and side actions.  can this be cleaned up?
// TOOD: does need fix ups as WatchInfo is no longer initialized
PanelCharts::pInstrumentActions_t PanelCharts::HandleGetInstrumentActions( const wxTreeItemId& item ) {
  mapItemToInstrument_t::iterator iter = m_mapItemToInstrument.find( item.GetID() );
  // create an entry for the menu item with its specific instrument and watch and indicators
  if ( m_mapItemToInstrument.end() == iter ) {
    //pWatchInfo_t pWatchInfo( new WatchInfo );
//    m_mapGuiEntry.insert( mapGuiEntry_t::value_type( item.GetID(), GuiEntry( new WatchInfo ) ) );
  }
  else {
    std::cout << "PanelCharts::HandleGetInstrumentActions: menu item already in existence" << std::endl;
  }
  return m_pInstrumentActions;  // TODO: maybe turn this into a lambda now
}

// signaled in TreeItemInstrument::~TreeItemInstrument
void PanelCharts::HandleMenuItemDelete( const wxTreeItemId& item ) {
  mapItemToInstrument_t::iterator iterIdItem = m_mapItemToInstrument.find( item.GetID() );
  if ( m_mapItemToInstrument.end() == iterIdItem ) {
    // this might be ok if deleting a group
    std::cout << "PanelCharts::HandleMenuItemDelete: no menuitem to delete" << std::endl;
  }
  else {
    mapInstrumentEntry_t::iterator iterIdInstrument = m_mapInstrumentEntry.find( iterIdItem->second );
    if ( m_mapInstrumentEntry.end() == iterIdInstrument ) {
      std::cout << "PanelCharts::HandleMenuItemDelete: no instrument found: " << iterIdItem->second << std::endl;;
    }
    else {
      InstrumentEntry& entry( iterIdInstrument->second );
      if ( 0 == entry.m_cntMenuDependents ) {
        std::cout << "PanelCharts::HandleMenuItemDelete: inconsistent m_cntMenuDependents" << std::endl;
      }
      else {
        if ( 2 < entry.m_pWatch.use_count() ) {
          std::cout << "PanelCharts::HandleMenuItemDelete use_count:" << iterIdItem->second << "," << entry.m_pWatch.use_count() << std::endl;
        }
        else {
          entry.m_cntMenuDependents--;
          if ( 0 == entry.m_cntMenuDependents ) {
            m_mapInstrumentEntry.erase( iterIdInstrument );
          }
        }
      }
    }
    RemoveRightDetail();
  }
}

void PanelCharts::HandleInstrumentLiveChart( const wxTreeItemId& item ) {
  
  mapItemToInstrument_t::iterator iterIdItem = m_mapItemToInstrument.find( item.GetID() );
  if ( m_mapItemToInstrument.end() == iterIdItem ) {
    std::cout << "PanelCharts::HandleInstrumentLiveChart: no menuitem" << std::endl;
  }
  else {
    
    RemoveRightDetail();

    m_ChartDataView.Clear();
    mapInstrumentEntry_t::iterator iterIdInstrument = m_mapInstrumentEntry.find( iterIdItem->second );
    if ( m_mapInstrumentEntry.end() == iterIdInstrument ) {
      std::cout << "PanelCharts::HandleInstrumentLiveChart: no instrument " << iterIdItem->second << std::endl;
    }
    else {
      iterIdInstrument->second.m_pWatchInfo->ApplyDataTo( &m_ChartDataView );

      WinChartView* pWinChartView = nullptr;
      pWinChartView = new WinChartView( m_panelSplitterRightPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER );

      // has postponed gui related assignments
      ReplaceRightDetail( pWinChartView );

      // starts update thread
      CallAfter( [this,pWinChartView](){ pWinChartView->SetChartDataView( &m_ChartDataView ); } );
    }
  }
}

void PanelCharts::HandleOptionChainList( const wxTreeItemId& item ) {
  
  struct OptionList {
    
    typedef std::map<boost::gregorian::date,int> mapDate_t;
    
    OptionList(): cnt(0) {}
    ~OptionList() {
      if ( 0 == map.size() ) {
        std::cout << "Empty Option List" << std::endl;
      }
      else {
        std::for_each( map.begin(), map.end(), [](const mapDate_t::value_type& a){ 
          std::cout << a.first << "," << a.second << std::endl; 
        });
        std::cout << cnt << std::endl;
      }
    }
    
    int cnt;
    mapDate_t map;
    void operator()( const ou::tf::iqfeed::MarketSymbol::TableRowDef& row ) {
      boost::gregorian::date date( row.nYear, row.nMonth, row.nDay );
      mapDate_t::iterator iter = map.find( date );
      if ( map.end() == iter ) {
        map.insert( mapDate_t::value_type( date, 1 ) );
      }
      else {
        iter->second++;
      }
      cnt++;
    }
  };
  
  RemoveRightDetail();
  auto pNotebookOptionChains = new NotebookOptionChains( m_panelSplitterRightPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT, _T( "a name" ) );
  ReplaceRightDetail( pNotebookOptionChains );

  mapItemToInstrument_t::iterator iterIdItem = m_mapItemToInstrument.find( item.GetID() );
  if ( m_mapItemToInstrument.end() == iterIdItem ) {
    std::cout << "PanelCharts::HandleOptionChainList: no menuitem" << std::endl;
  }
  else {
    mapInstrumentEntry_t::iterator iterIdInstrument = m_mapInstrumentEntry.find( iterIdItem->second );
    if ( m_mapInstrumentEntry.end() == iterIdInstrument ) {
      std::cout << "PanelCharts::HandleOptionChainList: no " << iterIdItem->second << std::endl;
    }
    else {
      CallAfter([this,pNotebookOptionChains,iterIdInstrument]{ // ensure iter is not invalidated in the meantime
        InstrumentEntry& entry( iterIdInstrument->second );
        // obtain instrument name (future requires special handling)
        ou::tf::Instrument::pInstrument_t p = entry.m_pWatch->GetInstrument();
        std::string sSymbol;
        switch ( p->GetInstrumentType() ) { 
          case ou::tf::InstrumentType::Stock:
            sSymbol = p->GetInstrumentName( ou::tf::ProviderInterfaceBase::eidProvider_t::EProviderIQF );
            break;
          case ou::tf::InstrumentType::Future: {
            //why not the Provider name here as well?
            //   -- because it supplies something like QGCZ17, where we want only QGC for the seach
            //sSymbol = p->GetInstrumentName( ou::tf::ProviderInterfaceBase::eidProvider_t::EProviderIQF );
            std::string sTemp = p->GetInstrumentName();
            size_t pos = sTemp.find( '-' );
            assert( 0 != pos );
            sSymbol = sTemp.substr( 0, pos );
            }
            break;
          default:
            assert(0);
            break;
        }
        assert( 0 != sSymbol.length() );

        // obtain the option list
    //    OptionList list;
    //    signalRetrieveOptionList( 
    //      sSymbol, 
    //      [&list](const ou::tf::iqfeed::MarketSymbol::TableRowDef& row ){ list( row ); }
    //      );

        pNotebookOptionChains->SetName( sSymbol );

        // populate tabs of notebook
        signalRetrieveOptionList(
          sSymbol,
          [pNotebookOptionChains]( const ou::tf::iqfeed::MarketSymbol::TableRowDef& row ) {
            boost::gregorian::date date( row.nYear, row.nMonth, row.nDay );
            pNotebookOptionChains->Add( date, row.dblStrike, row.eOptionSide, row.sSymbol );
          }
          );

        namespace args = std::placeholders;
        pNotebookOptionChains->m_fOnPageChanging = std::bind( &PanelCharts::OnOptionChainPageChanging, this, args::_1 );
        pNotebookOptionChains->m_fOnPageChanged = std::bind( &PanelCharts::OnOptionChainPageChanged, this, args::_1 );
        pNotebookOptionChains->m_fOnRowClicked 
          = std::bind( &PanelCharts::HandleGridClick, this, iterIdInstrument->first, args::_1, args::_2, args::_3, args::_4 );
      });
    }
  }
}

void PanelCharts::OnOptionChainPageChanging( boost::gregorian::date date ) {
}

void PanelCharts::OnOptionChainPageChanged( boost::gregorian::date date ) {
}

void PanelCharts::HandleGridClick( 
  idInstrument_t idInstrument,
  boost::gregorian::date date, double strike, 
  const ou::tf::GridOptionChain::OptionUpdateFunctions& funcCall,
  const ou::tf::GridOptionChain::OptionUpdateFunctions& funcPut ) 
{
  std::cout << "GridClick: " << date << "," << strike << "," << funcCall.sSymbolName << "," << funcPut.sSymbolName << std::endl;
  if ( ou::tf::keytypes::EProviderIQF != m_pData1Provider->ID() ) {
    std::cout << funcCall.sSymbolName << "," << funcPut.sSymbolName << ": IQFeed provider not available" << std::endl;
  }
  else {
    mapInstrumentEntry_t::iterator iterInstrument = m_mapInstrumentEntry.find( idInstrument );
    if ( m_mapInstrumentEntry.end() == iterInstrument ) {
      std::cout << "PanelCharts::HandleGridClick: " << idInstrument << " not found" << std::endl;
    }
    else {
      InstrumentEntry& entry( iterInstrument->second );
      std::vector<const ou::tf::GridOptionChain::OptionUpdateFunctions*> vFuncs = { &funcCall, &funcPut };
      std::for_each( vFuncs.begin(), vFuncs.end(),
        [this,&entry](const ou::tf::GridOptionChain::OptionUpdateFunctions* func){
          mapOption_t::iterator iterOption = entry.m_mapSelectedChainOptions.find( func->sSymbolName );
          if ( entry.m_mapSelectedChainOptions.end() == iterOption ) {
            pInstrument_t pInstrument = m_funcBuildInstrumentFromIqfeed( func->sSymbolName );
            assert( pInstrument->IsOption() || pInstrument->IsFuturesOption() );
            ou::tf::option::Option::pOption_t pOption( new ou::tf::option::Option( pInstrument, m_pData1Provider ) );
            iterOption 
              = entry.m_mapSelectedChainOptions.insert( 
                entry.m_mapSelectedChainOptions.begin(), mapOption_t::value_type( func->sSymbolName, pOption ) );
          }
          ou::tf::option::Option* pOption( iterOption->second.get() );
          assert( pOption->GetInstrument()->GetInstrumentName() == func->sSymbolName );
          assert( 0 != pOption );
          if ( pOption->Watching() ) { // go off
            pOption->StopWatch();
            pOption->OnQuote.Remove( func->fQuote );
            pOption->OnTrade.Remove( func->fTrade );
            pOption->OnGreek.Remove( func->fGreek );
          }
          else { // go on
            pOption->OnQuote.Add( func->fQuote );
            pOption->OnTrade.Add( func->fTrade );
            pOption->OnGreek.Add( func->fGreek );
            pOption->StartWatch();
          }
        });
    }
  }
}

void PanelCharts::HandleEmitValues( const wxTreeItemId& item ) {
  mapItemToInstrument_t::iterator iterIdItem = m_mapItemToInstrument.find( item.GetID() );
  if ( m_mapItemToInstrument.end() == iterIdItem ) {
    std::cout << "PanelCharts::HandleEmitValues: no menuitem" << std::endl;
  }
  else {
    mapInstrumentEntry_t::iterator iterIdInstrument = m_mapInstrumentEntry.find( iterIdItem->second );
    if ( m_mapInstrumentEntry.end() == iterIdInstrument ) {
      std::cout << "PanelCharts::HandleEmitValues no " << iterIdItem->second << " found" << std::endl;
    }
    else {
      iterIdInstrument->second.m_pWatch->EmitValues();
    }
    
  }
}

// constructs entry in m_mapInstrumentEntry as necessary
PanelCharts::pWatch_t PanelCharts::ConstructWatch( pInstrument_t pInstrument ) {
  
  const ou::tf::Instrument::idInstrument_t idInstrument( pInstrument->GetInstrumentName() );
  mapInstrumentEntry_t::iterator iterInstrument = m_mapInstrumentEntry.find( idInstrument );
  if ( m_mapInstrumentEntry.end() == iterInstrument ) {
    pWatch_t pWatch;
    if ( pInstrument->IsOption() || pInstrument->IsFuturesOption() ) {
      pWatch.reset( new ou::tf::option::Option( pInstrument, m_pData1Provider ) );
    }
    else {
      pWatch.reset( new ou::tf::Watch( pInstrument, m_pData1Provider ) );
    }
    
    signalRegisterInstrument( pInstrument );
    pWatchInfo_t pWatchInfo( new WatchInfo( pWatch ) );
    iterInstrument 
      = m_mapInstrumentEntry.insert( m_mapInstrumentEntry.begin(), 
          mapInstrumentEntry_t::value_type( idInstrument, InstrumentEntry( pWatch, pWatchInfo ) ) );
  }
  else {
    std::cout << "PanelCharts::ConstructWatch: " << idInstrument << " exists." << std::endl;
  }
  
  return iterInstrument->second.m_pWatch;
}

void PanelCharts::ConstructInstrumentEntry( void* id, pInstrument_t pInstrument, const std::string& sUnderlying ) {
  
  const ou::tf::Instrument::idInstrument_t idInstrument( pInstrument->GetInstrumentName() );
  mapInstrumentEntry_t::iterator iterInstrument = m_mapInstrumentEntry.find( idInstrument );
  if ( m_mapInstrumentEntry.end() == iterInstrument ) {
    pWatch_t pWatch;
    if ( pInstrument->IsOption() || pInstrument->IsFuturesOption() ) {
      pWatch.reset( new ou::tf::option::Option( pInstrument, m_pData1Provider ) );
    }
    else {
      pWatch.reset( new ou::tf::Watch( pInstrument, m_pData1Provider ) );
    }
    
    signalRegisterInstrument( pInstrument );
    pWatchInfo_t pWatchInfo( new WatchInfo( pWatch ) );
    iterInstrument 
      = m_mapInstrumentEntry.insert( m_mapInstrumentEntry.begin(), 
          mapInstrumentEntry_t::value_type( idInstrument, InstrumentEntry( pWatch, pWatchInfo ) ) );
  }
  else {
    std::cout << "PanelCharts::ConstructInstrumentEntry: " << idInstrument << " exists." << std::endl;
  }
  
}

// TODO: might be able to tune this further depending upon who call this
//   things have been simplified as an extra data structure is no longer in use
void PanelCharts::AddOptionWatch( 
  const std::string& sUnderlying, pWatch_t pInstrumentWatch ) {

  if ( pInstrumentWatch->GetInstrument()->IsOption() || pInstrumentWatch->GetInstrument()->IsFuturesOption() ) {
    mapInstrumentEntry_t::iterator iterUnderlying = m_mapInstrumentEntry.find( sUnderlying );
    if ( m_mapInstrumentEntry.end() == iterUnderlying ) {
      std::cout << "PanelCharts::AddOptionWatch: didn't find underlying (" << sUnderlying << ")" << std::endl;
    }
    else {
      Instrument::idInstrument_cref idOption( pInstrumentWatch->GetInstrument()->GetInstrumentName() );
      mapInstrumentEntry_t::iterator iterOption = m_mapInstrumentEntry.find( idOption );
      if ( m_mapInstrumentEntry.end() == iterOption ) {
        std::cout << "PanelCharts::AddOptionWatch: option " << idOption << " doesn't exist" << std::endl;
      }
      else {
        if ( 0 != iterOption->second.m_pWatchUnderlying.use_count() ) {
          std::cout << "PanelCharts::AddOptionWatch underlying already set " << sUnderlying << std::endl;
        }
        else {
          iterOption->second.m_pWatchUnderlying = iterUnderlying->second.m_pWatch;
        }
      }
    }
  }
  else {
    std::cout << "PanelCharts::AddOptionWatch: supplied instrument not option or foption: " 
      << pInstrumentWatch->GetInstrument()->GetInstrumentName() << std::endl;
  }
}

// called by anything more than the serialization in TreeItemInstrument?
// TODO: merge with the other caller of ConstructWatch?  
//   ConstructWatch creates the data structure for the basic watch
void PanelCharts::HandleLoadInstrument( 
  const wxTreeItemId& item, const std::string& sName, const std::string& sUnderlying 
) {
  
  pWatch_t pInstrumentWatch = ConstructWatch( signalLoadInstrument( sName ) );
  
  mapItemToInstrument_t::iterator iterIdItem = m_mapItemToInstrument.find ( item.GetID() );
  if ( m_mapItemToInstrument.end() == iterIdItem ) {
    std::cout << "HandleLoadInstrument: creating item entry" << std::endl;
    m_mapItemToInstrument.insert( mapItemToInstrument_t::value_type( item.GetID(), sName ) );
  }
  else {
    std::cout << "HandleLoadInstrument: item entry exists: " << sName << std::endl;
  }
  
  if ( !sUnderlying.empty() ) {
    AddOptionWatch( sUnderlying, pInstrumentWatch );
  }
  
}

InstrumentActions::values_t PanelCharts::HandleNewInstrumentRequest( 
  const wxTreeItemId& item, 
  const InstrumentActions::ENewInstrumentLock lock,
  const wxString& wxsUnderlying // optional
) {
  
  // the item coming in represents the existing menu item 
  //   which might be a group item, or an instrument item

  assert( 0 == m_pDialogPickSymbol );
  
  InstrumentActions::values_t values;
  
  if ( !wxsUnderlying.empty() ) {
    m_de.sIQFSymbolName = wxsUnderlying;
  }
  
  m_pDialogPickSymbol = new ou::tf::DialogPickSymbol( this );
  m_pDialogPickSymbol->SetDataExchange( &m_de );
  
  switch ( lock ) {
    case InstrumentActions::ENewInstrumentLock::LockFuturesOption:
      m_pDialogPickSymbol->SetFuturesOptionOnly();
      break;
    case InstrumentActions::ENewInstrumentLock::LockOption:
      m_pDialogPickSymbol->SetOptionOnly();
      break;
    case InstrumentActions::ENewInstrumentLock::NoLock:
      break;
  }
  
  int status = m_pDialogPickSymbol->ShowModal();
  
  pWatch_t pInstrumentWatch;
  
  switch ( status ) {
    case wxID_CANCEL:
      //m_pDialogPickSymbolCreatedInstrument.reset();
      // menu item should be deleting
      break;
    case wxID_OK:
      if ( 0 != m_pDialogPickSymbolCreatedInstrument.get() ) {
        
        pInstrumentWatch = ConstructWatch( m_pDialogPickSymbolCreatedInstrument );
        
        Instrument::idInstrument_cref idInstrument( m_pDialogPickSymbolCreatedInstrument->GetInstrumentName() );
        
        mapItemToInstrument_t::iterator iter = m_mapItemToInstrument.find ( item.GetID() );
        if ( m_mapItemToInstrument.end() != iter ) {
          std::cout << "PanelCharts::HandleNewInstrumentRequest: itemid already present" << std::endl;
        }
        else {
          m_mapItemToInstrument.insert( mapItemToInstrument_t::value_type( item.GetID(), idInstrument ) );
          values.name_ = idInstrument;
          // are these lock types propagated properly?
          //  ie, on load from file, are they set there?
          if ( m_pDialogPickSymbolCreatedInstrument->IsStock() ) values.lockType_ = InstrumentActions::ENewInstrumentLock::LockOption;
          if ( m_pDialogPickSymbolCreatedInstrument->IsFuture() ) values.lockType_ = InstrumentActions::ENewInstrumentLock::LockFuturesOption;
          //iter->second->Set( pInstrumentWatch );
        }

        if ( !wxsUnderlying.empty() ) {
          const std::string sUnderlying( wxsUnderlying );
          AddOptionWatch( sUnderlying, pInstrumentWatch );
        }
        
      }
      else {
        std::cout << "PanelCharts::HandleNewInstrumentRequest has wxID_OK but no instrument" << std::endl;
      }
      break;
  }
  
  m_pDialogPickSymbol->Destroy();
  m_pDialogPickSymbol = 0;
  
  m_pDialogPickSymbolCreatedInstrument.reset();
  
  return values;
}

// extract this sometime because the string builder might be used elsewhere
void PanelCharts::BuildInstrument( const DialogPickSymbol::DataExchange& pde, pInstrument_t& pInstrument ) {
  std::string sKey( pde.sIQFSymbolName );
  switch ( pde.it ) {
    case InstrumentType::Stock: {
      ValuesForBuildInstrument values( sKey, pde.sCompositeName, pde.sIBSymbolName, pInstrument, 0 );
      signalBuildInstrument( values );
    }
      break;
    case InstrumentType::Option:
    case InstrumentType::FuturesOption:
    {
      boost::uint16_t month( pde.month + 1 ); // month is 0 based
      boost::uint16_t day( pde.day ); // day is 1 based
      sKey += "-" + boost::lexical_cast<std::string>( pde.year )
        + ( ( 9 < month ) ? "" : "0" ) + boost::lexical_cast<std::string>( month ) 
        + ( ( 9 < day ) ? "" : "0" ) + boost::lexical_cast<std::string>( day );
      sKey += "-";
      sKey += pde.os;
      sKey += "-" + boost::lexical_cast<std::string>( pde.dblStrike )
        ;
      ValuesForBuildInstrument values( sKey, pde.sCompositeName, pde.sIBSymbolName, pInstrument, day );
      signalBuildInstrument( values );
    }
      break;
    case InstrumentType::Future:
    {
      boost::uint16_t month( pde.month + 1 ); // month is 0 based
      boost::uint16_t day( pde.day ); // day is 1 based
      sKey += "-" + boost::lexical_cast<std::string>( pde.year )
        + ( ( 9 < month ) ? "" : "0" ) + boost::lexical_cast<std::string>( month )
        + ( ( 0 == day ) ? "" : ( ( ( 9 < day ) ? "" : "0" ) + boost::lexical_cast<std::string>( day ) ) );
        ;
      ValuesForBuildInstrument values( sKey, pde.sCompositeName, pde.sIBSymbolName, pInstrument, day );
      signalBuildInstrument( values );
    }
      break;
  }
}

void PanelCharts::HandleLookUpDescription( const std::string& sSymbol, std::string& sDescription ) {
  signalLookUpDescription( sSymbol, sDescription );
}

void PanelCharts::HandleComposeComposite( DialogPickSymbol::DataExchange* pde ) {
  pde->sCompositeName = "";
  pde->sCompositeDescription = "";
  switch ( pde->it ) {
    case ou::tf::InstrumentType::Stock:
      pde->sCompositeName = pde->sIQFSymbolName;
      break;
    case ou::tf::InstrumentType::Option:
      pde->sCompositeName 
        = ou::tf::iqfeed::BuildOptionName( pde->sIQFSymbolName, pde->year, pde->month + 1, pde->day, pde->dblStrike, pde->os );
      break;
    case ou::tf::InstrumentType::Future:
      pde->sCompositeName
        = ou::tf::iqfeed::BuildFuturesName( pde->sIQFSymbolName, pde->year, pde->month + 1 );
      break;
    case ou::tf::InstrumentType::FuturesOption:
      pde->sCompositeName 
        = ou::tf::iqfeed::BuildFuturesOptionName( pde->sIQFSymbolName, pde->year, pde->month + 1, pde->dblStrike, pde->os );
      break;
    default: 
      throw std::runtime_error( "PanelCharts::HandleComposeComposite: unknown instrument type" );
      break;
  }
  if ( "" != pde->sCompositeName ) {
    signalLookUpDescription( pde->sCompositeName, pde->sCompositeDescription );
    if ( "" != pde->sCompositeDescription ) { // means we have a satisfactory iqfeed symbol
      BuildInstrument( m_de, m_pDialogPickSymbolCreatedInstrument );
    }
  }
}

// IB has populated instrument with ContractID
void PanelCharts::InstrumentUpdated( pInstrument_t pInstrument ) {
  if ( 0 != m_pDialogPickSymbol ) {
    if ( pInstrument.get() == m_pDialogPickSymbolCreatedInstrument.get() ) {
      // expecting contract id to already exist in instrument
      // might put a lock on instrument changes until contract comes back
      //assert( 0 != pInstrument->GetContract() );
      if ( 0 == pInstrument->GetContract() ) {
        std::cout << "PanelCharts::InstrumentUpdated missing contract " << pInstrument->GetInstrumentName() << std::endl;
      }
      else {
        m_pDialogPickSymbol->UpdateContractId( pInstrument->GetContract() );
      }
      
    }
    else {
      std::cout << "PanelCharts::InstrumentUpdated error:  not expected instrument" << std::endl;
    }
  }
}

void PanelCharts::OnClose( wxCloseEvent& event ) {
  // Exit Steps: #2 -> FrameMain::OnClose
//  if ( 0 != OnPanelClosing ) OnPanelClosing();
  // event.Veto();  // possible call, if needed
  // event.CanVeto(); // if not a 
  
  event.Skip();  // auto followed by Destroy();
}

void PanelCharts::OnWindowDestroy( wxWindowDestroyEvent& event ) {
  
  //m_pInstrumentActions.reset();
  
  int id1 = event.GetId();
  wxWindowID id2 = this->GetId();
  
  if ( id1 == id2 ) { // ignores 'destroy' of any child window
    m_connGetInstrumentActions.disconnect();

    m_connNewInstrument.disconnect();
    m_connLoadInstrument.disconnect();
    m_connEmitValues.disconnect();
    m_connLiveChart.disconnect();
    m_connDelete.disconnect();

    m_connLookupDescription.disconnect();
    m_connComposeComposite.disconnect();

    m_connChanging.disconnect();

    //Unbind( wxEVT_CLOSE_WINDOW, &PanelCharts::OnClose, this );
    Unbind( wxEVT_DESTROY, &PanelCharts::OnWindowDestroy, this );

  }
  
  event.Skip(); 

}

void PanelCharts::SaveSeries( const std::string& sPrefix ) {
  std::for_each( m_mapInstrumentEntry.begin(), m_mapInstrumentEntry.end(),
    [](mapInstrumentEntry_t::value_type& vt) {
      InstrumentEntry& entry( vt.second );
      entry.m_pWatch->EmitValues();
    });
}

void PanelCharts::Save( boost::archive::text_oarchive& oa) {
  //auto p = dynamic_cast<TreeItemRoot*>( m_pTreeOps->GetRoot().get() );
  //oa & *p;
  oa & m_splitter->GetSashPosition();
  m_pTreeOps->Save<TreeItemRoot>( oa );
}

void PanelCharts::Load( boost::archive::text_iarchive& ia) {
  //auto p = dynamic_cast<TreeItemRoot*>( m_pTreeOps->GetRoot().get() );
  //ia & *p;
  int pos;
  ia & pos;
  m_splitter->SetSashPosition( pos );
  m_pTreeOps->Load<TreeItemRoot>( ia );
}

wxBitmap PanelCharts::GetBitmapResource( const wxString& name ) {
    wxUnusedVar(name);
    return wxNullBitmap;
}

wxIcon PanelCharts::GetIconResource( const wxString& name ) {
    wxUnusedVar(name);
    return wxNullIcon;
}

} // namespace tf
} // namespace ou
