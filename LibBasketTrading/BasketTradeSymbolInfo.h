#pragma once

#include <string>
#include <sstream>
#include <map>

#include "boost/date_time/posix_time/posix_time.hpp"
using namespace boost::posix_time;
using namespace boost::gregorian;

#include "BarFactory.h"
#include "TimeSeries.h"
#include "ProviderInterface.h"
#include "Instrument.h"
#include "Order.h"
#include "OrderManager.h"
#include "Delegate.h"
#include "ChartRealTimeTreeView.h"

#include "ChartEntryBars.h"
#include "ChartEntryIndicator.h"
#include "ChartEntryMark.h"
#include "ChartEntrySegments.h"
#include "ChartEntryShape.h"
#include "ChartDataView.h"

class CBasketTradeSymbolInfo {
public:
  explicit CBasketTradeSymbolInfo( 
    const std::string &sSymbolName, const std::string &sPath, const std::string &sStrategy );
  explicit CBasketTradeSymbolInfo( std::stringstream *pStream );
  ~CBasketTradeSymbolInfo( void );

  Delegate<CBasketTradeSymbolInfo *> OnBasketTradeSymbolInfoChanged;

  double GetProposedEntryCost() { return m_dblProposedEntryCost; };
  int GetQuantityForEntry() { return m_nQuantityForEntry; };
  const std::string &GetSymbolName( void ) { return m_status.sSymbolName; };
  void StreamSymbolInfo( std::ostream *pStream );
  void WriteTradesAndQuotes( const std::string &sPathPrefix );

  void StartTrading( void );
  void StopTrading( void );

  struct structFieldsForDialog { // used for updating dialog
    std::string sSymbolName;
    double dblCurrentPrice;
    double dblHigh;
    double dblOpenRangeHigh;  // ACD Opening Range
    double dblOpen;
    double dblOpenRangeLow;   // ACD Opening Range
    double dblLow;
    double dblFilledPrice;
    double dblStop;
    int    nPositionSize; //+ or -
    double dblPositionSize;  
    double dblAverageCost;
    double dblUnRealizedPL;
    double dblRealizedPL;
    std::string sHit;  // 0/1 for long, 0/1 for short (two characters here)
    structFieldsForDialog( void ) : 
      dblHigh( 0 ), dblOpenRangeHigh( 0 ), dblOpen( 0 ),
      dblOpenRangeLow( 0 ), dblLow( 0 ), dblFilledPrice( 0 ), dblCurrentPrice( 0 ), dblStop( 0 ),
      nPositionSize( 0 ), dblPositionSize( 0 ), dblAverageCost( 0 ),
      dblUnRealizedPL( 0 ), dblRealizedPL( 0 ) {};
    structFieldsForDialog( const std::string &sSymbolName_ ) : sSymbolName( sSymbolName_ ),
      dblHigh( 0 ), dblOpenRangeHigh( 0 ), dblOpen( 0 ),
      dblOpenRangeLow( 0 ), dblLow( 0 ), dblFilledPrice( 0 ), dblCurrentPrice( 0 ), dblStop( 0 ),
      nPositionSize( 0 ), dblPositionSize( 0 ), dblAverageCost( 0 ),
      dblUnRealizedPL( 0 ), dblRealizedPL( 0 ) {};
  };
  const structFieldsForDialog &GetDialogFields( void ) { return m_status; };  // needs come after structure definition

  struct structCommonModelInformation {
    enum enumCalcStep { Prelim, Final } nCalcStep;
    bool bRTH;  // regular trading hours only
    ptime dtRTHBgn;
    ptime dtRTHEnd;
    ptime dtOpenRangeBgn;
    ptime dtOpenRangeEnd;
    ptime dtEndActiveTrading;
    ptime dtBgnNoMoreTrades;
    ptime dtBgnCancelTrades;
    ptime dtBgnCloseTrades;
    ptime dtTradeDate; // date of trading, previous days provide the history
    double dblFunds;  // funds available for use
    CProviderInterface *pDataProvider;
    CProviderInterface *pExecutionProvider;
    CChartRealTimeTreeView *pTreeView;
  };
  void CalculateTrade( structCommonModelInformation *pParameters  );

protected:
  void HandleQuote( const CQuote &quote );
  void HandleTrade( const CTrade &trade );
  void HandleOpen( const CTrade &trade );

  void Initialize( void );
  structFieldsForDialog m_status;
  std::string m_sPath;
  std::string m_sStrategy;
  double m_dblDayOpenPrice;
  double m_dblPriceForEntry;
  double m_dblAveragePriceOfEntry;
  double m_dblMarketValueAtEntry;
  double m_dblCurrentMarketPrice;
  double m_dblCurrentMarketValue;
  int m_nQuantityForEntry;
  int m_nWorkingQuantity;
  double m_dblAllocatedWorkingFunds;
  double m_dblExitPrice;
  double m_dblProposedEntryCost;
  enum enumPositionState { 
    Init, WaitingForOpen, 
    WaitingForThe3Bars, 
    WaitingForOrderFulfillmentLong, WaitingForOrderFulfillmentShort,
    WaitingForLongExit, WaitingForShortExit,
    WaitingForExit, Exited } m_PositionState;
  enum enumTradingState {
    WaitForFirstTrade,
    WaitForOpeningTrade, 
    WaitForOpeningBell, // 9:30 exchange time
    SetOpeningRange,  // spend 5 minutes here
    ActiveTrading,  // through the day
    NoMoreTrades, // 15:40 exchange time
    CancelTrades, // 15:45 exchange time
    CloseTrades,  // 15:50 exchange time
    DoneTrading   // 15:50 exchange time
  } m_TradingState;
  double m_dblAveBarHeight;
  double m_dblTrailingStopDistance;

  bool m_bDoneTheLong, m_bDoneTheShort;
  bool m_bFoundOpeningTrade;

  size_t m_nBarsInSequence;
  size_t m_nOpenCrossings;
  static const size_t m_nMaxCrossings = 2;  
  static const size_t m_nBarWidth = 30;  //seconds

  structFieldsForDialog m_FieldsForDialog;
  structCommonModelInformation *m_pModelParameters;
  enum enumStateForRangeCalc {
    WaitForRangeStart,
    CalculatingRange, 
    DoneCalculatingRange
  } m_OpeningRangeState, m_RTHRangeState;

  CInstrument *m_pInstrument;
  COrderManager m_OrderManager;

  void HandleBarFactoryBar( const CBar &bar );
  void HandleOrderFilled( COrder *pOrder );

  std::map<unsigned long, COrder*> m_mapActiveOrders;
  std::map<unsigned long, COrder*> m_mapCompletedOrders;

  CBarFactory m_1MinBarFactory;
  CQuotes m_quotes;
  CTrades m_trades;
  CBars m_bars;

  CChartEntryBars m_ceBars;
  CChartEntryVolume m_ceBarVolume;
  CChartEntryIndicator m_ceTrades;
  CChartEntryVolume m_ceTradeVolume;
  CChartEntryIndicator m_ceQuoteBids;
  CChartEntryIndicator m_ceQuoteAsks;
  CChartEntryMark m_ceLevels; // open, pivots
  CChartEntryShape m_ceOrdersBuy;
  CChartEntryShape m_ceOrdersSell;
  CChartEntrySegments m_ceZigZag;
  CChartEntryIndicator m_ceBollinger20TickAverage;
  CChartEntryIndicator m_cdBollinger20TickUpper;
  CChartEntryIndicator m_cdBollinger20TickLower;
  CChartEntryIndicator m_ceHi;
  CChartEntryIndicator m_ceLo;

  //CChartEntryIndicator  // some sort of indicator for order flow:  trade direction vs quotes, etc

  CChartDataView *m_pdvChart;


private:
  CBasketTradeSymbolInfo( const CBasketTradeSymbolInfo & );  // disallow copy construction
};
