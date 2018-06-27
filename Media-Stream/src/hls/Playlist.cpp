//
//  Playlist.cpp
//  Media-Stream
//
//  Created by Leonid on 4/1/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#include "Playlist.hpp"
#include "../Utils.hpp"
#include <cmath>

Playlist Playlist::parse(oatpp::parser::ParsingCaret& caret) {
  
  auto result = RecordMarkerList::createShared();
  
  while (caret.canContinue()) {
    caret.findChar('#');
    if(caret.proceedIfFollowsText("#EXTINF:")) {
      v_float64 secs = caret.parseFloat64();
      caret.findNextLine();
      oatpp::parser::ParsingCaret::Label uriLabel(caret);
      caret.findChar('\n');
      result->pushBack({secs, oatpp::String(uriLabel.toString())});
    }
    caret.findNextLine();
  }
  
  return Playlist(result);
  
}

Playlist Playlist::parseFromFile(const char* filename) {
  auto text = loadFromFile(filename);
  if(!text){
    throw std::runtime_error("Can't find playlist file. Make sure you specified full file path's for video and playlist in AppComponent.hpp");
  }
  oatpp::parser::ParsingCaret caret(text);
  return parse(caret);
}

std::shared_ptr<oatpp::data::stream::ChunkedBuffer> Playlist::generateForTime(v_int64 millis, v_int32 numRecords) {
  if(m_records->count() == 0) {
    throw std::runtime_error("Empty playlist");
  }
  auto stream = oatpp::data::stream::ChunkedBuffer::createShared();
  
  v_float64 secs = (v_float64) millis / 1000.0;
  
  v_int64 rounds = std::floor(secs / m_totalDuration);
  v_float64 loopTime = secs - rounds * m_totalDuration;
  v_float64 currTime = 0;
  
  v_int64 mediaSequence = 1 + rounds * m_records->count();
  
  auto currRecordNode = m_records->getFirstNode();
  while (currRecordNode->getNext() != nullptr) {
    if(currRecordNode->getData().duration + currTime >= loopTime) {
      break;
    }
    currTime += currRecordNode->getData().duration;
    currRecordNode = currRecordNode->getNext();
    mediaSequence ++;
  }
  
  v_float64 targetDuration = 0;
  RecordMarkerList list;
  for(v_int32 i = 0; i < numRecords; i++) {
    auto& marker = currRecordNode->getData();
    list.pushBack(marker);
    
    if(marker.duration > targetDuration) {
      targetDuration = marker.duration;
    }
    
    currRecordNode = currRecordNode->getNext();
    if(currRecordNode == nullptr) {
      currRecordNode = m_records->getFirstNode();
    }
  }
  
  OATPP_LOGD("playlist", "generating sequence %d", mediaSequence);
  
  stream
  << "#EXTM3U\n"
  << "#EXT-X-TARGETDURATION:" << (v_int32)(targetDuration + 1) << "\n"
  << "#EXT-X-VERSION:3\n"
  << "#EXT-X-MEDIA-SEQUENCE:" << mediaSequence << "\n";
  
  list.forEach([stream](const RecordMarker& marker){
    
    stream
    << "#EXTINF:" << marker.duration << ",\n"
    << marker.uri << "\n";
    
  });
  
  return stream;
}
