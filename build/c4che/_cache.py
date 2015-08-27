AR = ['/usr/bin/ar']
ARFLAGS = ['rcs']
BINDIR = '/usr/local/bin'
CC_VERSION = ('4', '8', '4')
COMPILER_CXX = 'g++'
CPPPATH_ST = '-I%s'
CXX = ['/usr/bin/g++']
CXXFLAGS = ['-std=c++0x', '-std=c++11', '-pedantic', '-Wall', '-O2', '-g']
CXXFLAGS_MACBUNDLE = ['-fPIC']
CXXFLAGS_cxxshlib = ['-fPIC']
CXXLNK_SRC_F = []
CXXLNK_TGT_F = ['-o']
CXX_NAME = 'gcc'
CXX_SRC_F = []
CXX_TGT_F = ['-c', '-o']
DATADIR = '/usr/local/share'
DATAROOTDIR = '/usr/local/share'
DEFINES_NDN_CXX = ['HAVE_NDN_CXX=1']
DEFINES_ST = '-D%s'
DEST_BINFMT = 'elf'
DEST_CPU = 'x86_64'
DEST_OS = 'linux'
DOCDIR = '/usr/local/share/doc/ndn-tlv-ping'
DVIDIR = '/usr/local/share/doc/ndn-tlv-ping'
EXEC_PREFIX = '/usr/local'
HAVE_NDN_CXX = 1
HTMLDIR = '/usr/local/share/doc/ndn-tlv-ping'
INCLUDEDIR = '/usr/local/include'
INCLUDES_NDN_CXX = ['/usr/local/include']
INFODIR = '/usr/local/share/info'
LIBDIR = '/usr/local/lib'
LIBEXECDIR = '/usr/local/libexec'
LIBPATH_NDN_CXX = ['/usr/local/lib']
LIBPATH_ST = '-L%s'
LIB_NDN_CXX = ['ndn-cxx', 'boost_system', 'boost_filesystem', 'boost_date_time', 'boost_iostreams', 'boost_regex', 'boost_program_options', 'boost_chrono', 'boost_random', 'cryptopp', 'sqlite3', 'rt', 'pthread']
LIB_ST = '-l%s'
LINKFLAGS = []
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINKFLAGS_cxxshlib = ['-shared']
LINKFLAGS_cxxstlib = ['-Wl,-Bstatic']
LINK_CXX = ['/usr/bin/g++']
LOCALEDIR = '/usr/local/share/locale'
LOCALSTATEDIR = '/usr/local/var'
MANDIR = '/usr/local/share/man'
OLDINCLUDEDIR = '/usr/include'
PACKAGE = 'ndn-tlv-ping'
PDFDIR = '/usr/local/share/doc/ndn-tlv-ping'
PKGCONFIG = ['/usr/bin/pkg-config']
PREFIX = '/usr/local'
PSDIR = '/usr/local/share/doc/ndn-tlv-ping'
RPATH_ST = '-Wl,-rpath,%s'
SBINDIR = '/usr/local/sbin'
SHAREDSTATEDIR = '/usr/local/com'
SHLIB_MARKER = '-Wl,-Bdynamic'
SONAME_ST = '-Wl,-h,%s'
STLIBPATH_ST = '-L%s'
STLIB_MARKER = '-Wl,-Bstatic'
STLIB_ST = '-l%s'
SYSCONFDIR = '/usr/local/etc'
cxxprogram_PATTERN = '%s'
cxxshlib_PATTERN = 'lib%s.so'
cxxstlib_PATTERN = 'lib%s.a'
macbundle_PATTERN = '%s.bundle'