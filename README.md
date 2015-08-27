Traffic Generator For NDN (ndn-traffic-generator)
=================================================

This tool is designed to send and receive large files in an NDN network.

## Prerequisites ##

Compiling and running ndn-traffic-generator requires the following dependencies:

1. C++ Boost Libraries version >= 1.48 <http://www.boost.org>

    On Ubuntu 12.04:

        sudo apt-get install libboost1.48-all-dev

    On Ubuntu 13.10 and later

        sudo apt-get install libboost-all-dev

    On OSX with macports

        sudo port install boost

    On OSX with brew

        brew install boost

On other platforms Boost Libraries can be installed from the packaged version for the
distribution, if the version matches requirements, or compiled from source

2. ndn-cxx library <https://github.com/named-data/ndn-cxx>

    For detailed installation instructions, please refer README file

3. NDN forwarding daemon <https://github.com/named-data/NFD>

-----------------------------------------------------

## 1. Compile & Installation Instructions: ##

    ./waf configure
    ./waf
    sudo ./waf install

## 2. Tool Run Instructions & Command Line Options: ##

    Usage: ndnfilesender <path to sender>/filename
    
    Usage:  ndnfilerequest <path to sender>/filename

Presently the filename that needs to be send has to be entered in the src/file-sender.cpp at onInputfile() only
and then have to be recompiled and installed again
and the name of the file which will be stored at the receiver side also has to be entered in src/file-requestor.cpp itself.
This is just the optimization which needs to be done which can take it from the command line args only.

* These tools need not be used together and can be used individually as well.
* Please refer sample configuration files provided for details on how to create your own.
* Use command line options shown above to adjust traffic configuration.
-----------------------------------------------------