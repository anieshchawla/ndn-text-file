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
#include <ndn-cxx/security/key-chain.hpp>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

namespace ndn {

class NdnTlvFileSender : boost::noncopyable
{
public:

  explicit
  NdnTlvFileSender(char* programName)
    : m_programName(programName)
    , m_hasError(false)
    , m_freshnessPeriod(getMinimumFreshnessPeriod())
    , m_face(m_ioService)
  {
  }

  void
  usage()
  {
    std::cout << "\n Usage:\n " << m_programName << " ndn:/name/prefix [options]\n"
        " Starts a NDN file sender that responds to Interests with name"
        " ndn:/name/prefix/filename.\n"
        "   [-x freshness] - set FreshnessSeconds\n"
        "   [-h]           - print this message and exit\n\n";
    exit(1);
  }

  time::milliseconds
  getMinimumFreshnessPeriod()
  {
    return time::milliseconds(1000);
  }

  void
  setFreshnessPeriod(int freshnessPeriod)
  {
    if (freshnessPeriod <= 0)

    m_freshnessPeriod = time::milliseconds(freshnessPeriod);
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
  onInterest(const Interest& interest)
  {
    Name interestName = interest.getName();
    //interestName.append(".txt");
  //  std::cout << "Interest Received = "<< interestName << std::endl;
    size_t segnum = static_cast<size_t>(interest.getName().rbegin()->toSegment());
    if(segnum<m_store.size()){
      m_face.put(*m_store[segnum]);
    }
  
    }

  void
  onInputFile(){
    std::ifstream file("test_file.pdf", std::ios::binary);
    std::string responseContent;
    int file_size,read_sz=0;
    int number_of_blocks=0;
    file.seekg(0, std::ios::end);
    responseContent.resize(file.tellg());
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (file){
      read_sz = file.tellg();
      while(read_sz!=file_size){
        int last_read_sz = read_sz;
        file.read(&responseContent[0], 8000);
        read_sz = file.tellg();
        if(read_sz<0) read_sz=file_size;
        int size = read_sz - last_read_sz;
        //std::string s = std::to_string(number_of_blocks);
        //interestName.append(s);
        //char responseContent[] = "NDN TLV File send Response";
        shared_ptr<Data> data = make_shared<Data>(Name(m_prefix).appendSegment(number_of_blocks));
        data->setFreshnessPeriod(m_freshnessPeriod);
        data->setContent(reinterpret_cast<const uint8_t*>(&responseContent[0]),size);
        m_keyChain.sign(*data);
        m_store.push_back(data);
        number_of_blocks++;
      }
    }
    file.close();

  }


  void
  onRegisterFailed(const ndn::Name& prefix, const std::string& reason)
  {
    std::cerr << "ERROR: Failed to register prefix in local hub's daemon" << std::endl;
    std::cerr << "REASON: " << reason << std::endl;
    m_hasError = true;
    m_face.shutdown();
    m_ioService.stop();
  }

  void
  signalHandler()
  {
    std::cout << "\n Shutting Down File Sender (" << m_name.toUri() << ").\n" << std::endl;
    m_face.shutdown();
    exit(1);
  }

  void
  run()
  {
    std::cout << "\n=== File Sender " << m_prefix <<" ===\n" << std::endl;

    boost::asio::signal_set signalSet(m_ioService, SIGINT, SIGTERM);
    signalSet.async_wait(bind([this]() { signalHandler(); }));
    m_name = m_prefix;
    m_face.setInterestFilter(m_name,
                             bind(&NdnTlvFileSender::onInterest,
                                  this, _2),
                             bind(&NdnTlvFileSender::onRegisterFailed,
                                  this, _1,_2));
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
  bool m_hasError;
  time::milliseconds m_freshnessPeriod;
  char* m_prefix;
  Name m_name;
  boost::asio::io_service m_ioService;
  KeyChain m_keyChain;
  Face m_face;
  std::vector< shared_ptr<Data> > m_store;


};

}

int
main(int argc, char* argv[])
{
  int res;

  ndn::NdnTlvFileSender ndnTlvFileSender(argv[0]);
  while ((res = getopt(argc, argv, "hdtp:x:")) != -1)
    {
      switch (res) {
      case 'h':
        ndnTlvFileSender.usage();
        break;
      case 'x':
        ndnTlvFileSender.setFreshnessPeriod(atoi(optarg));
        break;
      default:
        ndnTlvFileSender.usage();
        break;
      }
    }

  argc -= optind;
  argv += optind;

  if (argv[0] == 0)
    ndnTlvFileSender.usage();

  ndnTlvFileSender.setPrefix(argv[0]);
  ndnTlvFileSender.onInputFile();

  ndnTlvFileSender.run();

  std::cout << std::endl;

  if (ndnTlvFileSender.hasError())
    return 1;
  else
    return 0;
}
