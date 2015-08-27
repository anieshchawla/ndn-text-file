This file requires that you have installed all the requirements for ndn-cxx and NFD.
There is no other requirement for this.

Install path on client (nfdc create udp://ipaddr_of_sender and nfdc register <path to sender> udp://ipaddr_of_sender )to reach the location

To put any file on the server: ndnfilesender <path to sender>/filename
and to get the file from the : ndnfilerequest <path to sender>/filename