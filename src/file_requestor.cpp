/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2014 Arizona Board of Regents
 *
 * This file is part of ndn-tlv-ping (Ping Application for Named Data Networking).
 *
 * ndn-tlv-ping is a free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-tlv-ping is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndn-tlv-ping, e.g., in LICENSE file.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author: Jerald Paul Abraham <jeraldabraham@email.arizona.edu>
 */

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/name.hpp>
#include <fstream>
#include <ctime>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/noncopyable.hpp>

namespace ndn {

class NdnTlvFileRequest : boost::noncopyable
{
public:

  explicit
  NdnTlvFileRequest(char* programName)
    : m_programName(programName)
    , m_isAllowCachingSet(false)
    , m_isPrintTimestampSet(false)
    , m_hasError(false)
    , m_totalRequest(0)
    , m_requestInterval(getRequestMinimumInterval())
    , m_clientIdentifier(0)
    , m_requestTimeoutThreshold(getrequestTimeoutThreshold())
    , m_face(m_ioService)
    , m_file_requested("receiving_file.txt",std::ios::binary)
    , m_file_left(1)
  {
  }


  void
  usage()
  {
    std::cout << "\n Usage:\n " << m_programName << " ndn:/name/prefix [options]\n"
        " Request an file via NDN name prefix using Interests with name"
        " ndn:/name/prefix/filename.\n"
        "   [-a]            - allow routers to return stale Data from cache\n";
    exit(1);
  }

  time::milliseconds
  getrequestTimeoutThreshold()
  {
    return time::milliseconds(100000);
  }

  time::milliseconds
  getRequestMinimumInterval()
  {
    return time::milliseconds(1000);
  }

  void
  setAllowCaching()
  {
    m_isAllowCachingSet = true;
  }

  void
  setPrintTimestamp()
  {
    m_isPrintTimestampSet = true;
  }

  void
  setClientIdentifier(char* clientIdentifier)
  {
    m_clientIdentifier = clientIdentifier;
    if (strlen(clientIdentifier) == 0)
      usage();
    while (*clientIdentifier != '\0') {
      if( isalnum(*clientIdentifier) == 0 )
        usage();
      clientIdentifier++;
    }
  }

  void
  setPrefix(char* prefix)
  {
    m_prefix = prefix;
  }

  bool
  hasError() const
  {
    return m_hasError;
  }


  void
  onData(Data& data)
  {
    std::string request;
    //std::string filecopy = interest.getName().getSubName(interest.getName().size()-1).toUri().substr(1);
    if(!m_file_requested.is_open()){
      m_file_requested.open("checking.pdf",std::ofstream::binary);
    }
    //std::ofstream myfile ("checking.pdf",std::ofstream::binary);
    //request = interest.getName().toUri();
    std::cout << "Content From " << m_prefix<<std::endl;
    //we are writing the content of the received data on the terminal itself
    Block block = data.getContent();
    //m_totalRequest++;
    //std::cout.write((const char*)block.value(), block.value_size());
    m_file_requested.write((const char*)block.value(),block.value_size());
    //m_file_requested.write((const char*)(block.value()), block.value_size());
    if(block.value_size()!=8000){
        std::cout<<"block size is "<<block.value_size()<<" total request are "<<m_totalRequest<<std::endl;
        m_file_left=0;
        m_file_requested.close();

        m_end = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration td_start = m_start.time_of_day();
        boost::posix_time::time_duration td_end = m_end.time_of_day();
        long milliscnd = td_end.total_milliseconds() - td_start.total_milliseconds();
        std::cout<<" total time to transfer is "<< milliscnd;
        /*time::system_clock::time_point today = time::system_clock::now();
        time_t tt;
        tt = time::system_clock::to_time_t(today);
        std::cout<<" time of sending "<<ctime(&tt)<<std::endl;*/
        this->finish();
    }
    else{
      //m_totalRequest++;
        Interest interest_new(Name(m_prefix).appendSegment(m_totalRequest++));
        interest_new.setMustBeFresh(false);
        interest_new.setInterestLifetime(m_requestTimeoutThreshold);
        interest_new.setNonce(std::rand());
      
      //std::cout<<"Sending_interest "<<interest_new.getName()<<std::endl;
      m_face.expressInterest(interest_new,
                std::bind(&NdnTlvFileRequest::onData, this,_2),
                          std::bind(&NdnTlvFileRequest::onTimeout, this, _1));
      //m_ioService.poll();
    }
   //this->finish();
     
  }

  void
  onTimeout(const ndn::Interest& interest)
  {
    if (m_isPrintTimestampSet)
      std::cout << time::toIsoString(time::system_clock::now())  << " - ";
    std::cout << "Timeout From " << m_prefix;
    std::cout << " - Request Reference = " <<
      interest.getName().getSubName(interest.getName().size()-1).toUri().substr(1);
    std::cout << std::endl;
    this->finish();
  }


  void
  requestfile(boost::asio::deadline_timer* deadlineTimer)
  {

        Name requestfileName(m_prefix);
        //if(m_clientIdentifier != 0)
          //requestfileName.append(m_clientIdentifier);
       /* std::string s = std::to_string(m_totalRequest);
        requestfileName.append(s);*/
        ndn::Interest interest(requestfileName.appendSegment(m_totalRequest++));
        std::cout<<" interest sent "<<interest.getName()<<std::endl;
        if (m_isAllowCachingSet)
          interest.setMustBeFresh(true);
        else
          interest.setMustBeFresh(false);
        interest.setInterestLifetime(m_requestTimeoutThreshold);
        interest.setNonce(std::rand());
      
        m_start = boost::posix_time::microsec_clock::local_time();
        //time::system_clock::time_point today = time::system_clock::now();
        //time_t tt;
        //tt = time::system_clock::to_time_t(today);
        //std::cout<<" time of sending "<<ctime(&tt);

          try {
            m_face.expressInterest(interest,
                std::bind(&NdnTlvFileRequest::onData, this, _2),
                          std::bind(&NdnTlvFileRequest::onTimeout, this, _1));
            /*if(m_file_left){

                deadlineTimer->expires_at(deadlineTimer->expires_at() + boost::posix_time::millisec(1));
                deadlineTimer->async_wait(bind(&NdnTlvFileRequest::requestfile, this, deadlineTimer));
              }*/
            }
          catch (std::exception& e) {
           std::cerr << "ERROR: " << e.what() << std::endl;
          }
        //deadlineTimer->expires_at(deadlineTimer->expires_at() + boost::posix_time::millisec(m_file_left));
        //deadlineTimer->async_wait(std::bind(&NdnTlvFileRequest::requestfile, this,deadlineTimer));
      //this->finish();
    
  }

  void
  finish()
  {
    m_face.shutdown();
    m_ioService.stop();
  }

  void
  signalHandler()
  {
    m_face.shutdown();
    exit(1);
  }

  void
  run()
  {
    std::cout << "\n=== Requesting file " << m_prefix  << " ===\n" <<std::endl;

    //Name requestfileName(m_prefix);
     //m_file_requested.open("example.pdf",std::ios::binary);

    boost::asio::signal_set signalSet(m_ioService, SIGINT, SIGTERM);
    signalSet.async_wait(bind(&NdnTlvFileRequest::signalHandler, this));

    boost::asio::deadline_timer deadlineTimer(m_ioService,
                                              boost::posix_time::millisec(0));
    deadlineTimer.async_wait(bind(&NdnTlvFileRequest::requestfile,
                                  this,
                                  &deadlineTimer));
    try {
      m_face.processEvents();
    }
    catch (std::exception& e) {
      std::cerr << "ERROR: " << e.what() << std::endl;
      m_hasError = true;
      m_ioService.stop();
    }
  }

private:
  char* m_programName;
  bool m_isAllowCachingSet;
  bool m_isPrintTimestampSet;
  bool m_hasError;
  size_t m_totalRequest;
  time::milliseconds m_requestInterval;
  char* m_clientIdentifier;
  time::milliseconds m_requestTimeoutThreshold;
  char* m_prefix;
  boost::asio::io_service m_ioService;
  Face m_face;
  std::ofstream m_file_requested;
  int m_file_left;
  boost::posix_time::ptime m_start;
  boost::posix_time::ptime m_end;
};

}


int
main(int argc, char* argv[])
{
  std::srand(::time(0));
  int res;

  ndn::NdnTlvFileRequest ndnTlvFileRequest(argv[0]);
  while ((res = getopt(argc, argv, "htai:c:n:p:")) != -1)
    {
      switch (res) {
      case 'h':
        ndnTlvFileRequest.usage();
        break;
      case 'p':
        ndnTlvFileRequest.setClientIdentifier(optarg);
        break;
      case 't':
        ndnTlvFileRequest.setPrintTimestamp();
        break;
      default:
        ndnTlvFileRequest.usage();
        break;
      }
    }

  argc -= optind;
  argv += optind;

  if (argv[0] == 0)
    ndnTlvFileRequest.usage();

  ndnTlvFileRequest.setPrefix(argv[0]);
  ndnTlvFileRequest.run();

  std::cout << std::endl;

  if (ndnTlvFileRequest.hasError())
    return 1;
  else
    return 0;
}
