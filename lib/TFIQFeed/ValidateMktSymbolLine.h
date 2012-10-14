/************************************************************************
 * Copyright(c) 2012, One Unified. All rights reserved.                 *
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

#pragma once

#include <vector>
#include <map>
#include<string>

#include <OUCommon/FastDelegate.h>
using namespace fastdelegate;

#include <OUCommon/KeywordMatch.h>

#include "ParseMktSymbolLine.h"
#include "ParseOptionDescription.h"

namespace ou { // One Unified
namespace tf { // TradeFrame
namespace iqfeed { // IQFeed

class ValidateMktSymbolLine {
public:

  typedef ou::tf::iqfeed::MarketSymbol::TableRowDef trd_t;
  typedef FastDelegate1<trd_t&> OnProcessLine_t;
  typedef FastDelegate1<const std::string&> OnProcessHasOption_t;

  void SetOnProcessLine( OnProcessLine_t function ) {
    m_OnProcessLine = function;
  }
  void SetOnProcessHasOption( OnProcessHasOption_t function ) {
    m_OnProcessHasOption = function;
  }

  ValidateMktSymbolLine( void );
  virtual ~ValidateMktSymbolLine( void ) {};

  template<typename Iterator>
  void Parse( Iterator begin, Iterator end );

  void PostProcess( void );

  void Summary( void );

  size_t LinesProcessed( void ) const { return cntLinesTotal; };
  void HandleParsedStructure( trd_t& trd ) {};  // override by inheriting class
  void HandleUpdateHasOption( const std::string& ) {}; // override by inheriting class
protected:
private:

  typedef ou::tf::iqfeed::MarketSymbol::enumSymbolClassifier sc_t;

  OnProcessLine_t m_OnProcessLine;
  OnProcessHasOption_t m_OnProcessHasOption;

  struct structCountPerString { // count cnt of string s
    size_t cnt;
    std::string s;
    structCountPerString( void ) : cnt( 0 ) {};
    bool operator<(const structCountPerString& rhs) { return (s < rhs.s); };
  };

  ou::KeyWordMatch<size_t> kwmExchanges;
  std::vector<structCountPerString> vSymbolsPerExchange;
  std::map<std::string, unsigned long> mapUnderlying;  // keeps track of optionable symbols, to fix bool at end

  unsigned short nUnderlyingSize;
  size_t cntLinesTotal;
  size_t cntLinesParsed;
  size_t cntSIC;
  size_t cntNAICS;

  ou::tf::iqfeed::MktSymbolLineParser<const char*> parserFullLine;
  ou::tf::iqfeed::OptionDescriptionParser<std::string::const_iterator> parserOptionDescription;
  std::vector<size_t> vSymbolTypeStats;  // number of symbols of this SymbolType
};

extern boost::uint8_t rFutureMonth[];

template<typename Iterator>
void ValidateMktSymbolLine::Parse( Iterator begin, Iterator end ) {

  ++cntLinesTotal;  // number data lines processed

  try {

    ou::tf::iqfeed::MarketSymbol::TableRowDef trd;

    bool b = qi::parse( begin, end, parserFullLine, trd );
    if ( !b ) {
      std::cout << "problems parsing" << std::endl;
    }
    else {

      cntLinesParsed++;

      size_t ix;

      vSymbolTypeStats[ trd.sc ]++;
      if ( sc_t::Unknown == trd.sc ) {
        // set marker not to save record?
        std::cout << "Unknown symbol type for:  " << trd.sSymbol << std::endl;
      }

      std::string sPattern( trd.sExchange );
      if ( trd.sExchange == trd.sListedMarket ) {
      }
      else {
        sPattern += "," + trd.sListedMarket;
      }

      if ( 0 == sPattern.length() ) {
        std::cout << trd.sSymbol << " has zero length exchange,market" << std::endl;
      }
      else {
        ix = kwmExchanges.FindMatch( sPattern );
        if ( ( 0 == ix ) || ( sPattern.length() != vSymbolsPerExchange[ ix ].s.length() ) ) {
          std::cout << "Adding Exchange " << sPattern << std::endl;
          size_t cnt = kwmExchanges.GetPatternCount();
          kwmExchanges.AddPattern( sPattern, cnt );
          structCountPerString cps;
          vSymbolsPerExchange.push_back( cps );
          vSymbolsPerExchange[ cnt ].cnt = 1;
          vSymbolsPerExchange[ cnt ].s = sPattern;
        }
        else {
          ++( vSymbolsPerExchange[ ix ].cnt );
        }
      }

      if ( ou::tf::iqfeed::MarketSymbol::Equity == trd.sc ) {
        if ( 0 != trd.nSIC ) cntSIC++;
        if ( 0 != trd.nNAICS ) cntNAICS++;
      }

      // parse out contract expiry information
      // � For combined session symbols, the first character is "+".
      //� For Night/Electronic sessions, the first character is "@".
      // � Replace the Month and Year code with "#" for Front Month (ie. @ES# instead of @ESU10).
      // � NEW!-Replace the Month and Year code with "#C" for Front Month back-adjusted history (ie. @ES#C instead of @ESU10). 
      // http://www.iqfeed.net/symbolguide/index.cfm?symbolguide=guide&displayaction=support&section=guide&web=iqfeed&guide=commod&web=IQFeed&symbolguide=guide&displayaction=support&section=guide&type=comex&type2=comex_gbx
      if ( ou::tf::iqfeed::MarketSymbol::Future == trd.sc ) {
        bool bDecode = true;
  //          if ( '+' == sSymbol[0] ) {
  //          }
  //          if ( '@' == sSymbol[0] ) {
  //          }
        if ( '#' == trd.sSymbol[ trd.sSymbol.length() - 1 ] ) {
          bDecode = false;
        }
        if ( bDecode ) {
          std::string sYear = trd.sSymbol.substr( trd.sSymbol.length() - 2 );
          char mon = trd.sSymbol[ trd.sSymbol.length() - 3 ];
          if ( ( 'F' > mon ) || ( 'Z' < mon ) || ( 0 == rFutureMonth[ mon - 'A' ] ) ) {
            std::cout << "Bad futures month on " << trd.sSymbol << ": " << trd.sDescription << std::endl;
          }
          else {
            trd.nMonth = rFutureMonth[ mon - 'A' ];
            trd.nYear = 2000 + atoi( sYear.c_str() );
          }
        }
      }

      // parse out option contract information
      if ( ou::tf::iqfeed::MarketSymbol::IEOption == trd.sc ) {
        ou::tf::iqfeed::structParsedOptionDescription structOption( trd.sUnderlying, trd.nMonth, trd.nYear, trd.eOptionSide, trd.dblStrike );
        std::string::const_iterator sb( trd.sDescription.begin() );
        std::string::const_iterator se( trd.sDescription.end() );
        bool b = parse( sb, se, parserOptionDescription, structOption );
        if ( b && ( sb == se ) ) {
          if ( 0 == trd.sUnderlying ) {
            std::cout << "Option Decode:  Zero length underlying for " << trd.sSymbol << std::endl;
          }
          else {
            mapUnderlying[ structOption.sUnderlying ] = 1;  // simply create an entry for later use
          }
          nUnderlyingSize = std::max<unsigned short>( nUnderlyingSize, structOption.sUnderlying.size() );
        }
        else {
          std::cout  << "Option Decode:  Incomplete, " << trd.sSymbol << ", " << trd.sDescription << std::endl;
        }
      }

      if ( 0 == trd.sDescription.length() ) {
        std::cout << trd.sSymbol << ": missing description" << std::endl;
      }

      if ( 0 != m_OnProcessLine ) m_OnProcessLine( trd );
//      if ( &ValidateMktSymbolLine<CRTP,IteratorLines>::HandleParsedStructure != &CRTP::HandleParsedStructure ) {
//        static_cast<CRTP*>( this )->HandleParsedStructure( trd );
//      }
    }
  }
  catch (...) {
    std::cout << "parserFullLine broken" << std::endl;
  }

}

} // namespace iqfeed
} // namespace tf
} // namespace ou