DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

          
Zioncoin_CONFIG_INCLUDES=-I$(builddir)/config
Zioncoin_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

Zioncoin_INCLUDES += -I$(srcdir)/secp256k1/include
Zioncoin_INCLUDES += $(UNIVALUE_CFLAGS)

LIBZioncoin_SERVER=libZioncoin_server.a
LIBZioncoin_COMMON=libZioncoin_common.a
LIBZioncoin_CONSENSUS=libZioncoin_consensus.a
LIBZioncoin_CLI=libZioncoin_cli.a
LIBZioncoin_UTIL=libZioncoin_util.a
LIBZioncoin_CRYPTO=crypto/libZioncoin_crypto.a
LIBZioncoinQT=qt/libZioncoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_ZMQ
LIBZioncoin_ZMQ=libZioncoin_zmq.a
endif
if BUILD_Zioncoin_LIBS
LIBZioncoinCONSENSUS=libZioncoinconsensus.la
endif
if ENABLE_WALLET
LIBZioncoin_WALLET=libZioncoin_wallet.a
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBZioncoin_CRYPTO) \
  $(LIBZioncoin_UTIL) \
  $(LIBZioncoin_COMMON) \
  $(LIBZioncoin_CONSENSUS) \
  $(LIBZioncoin_SERVER) \
  $(LIBZioncoin_CLI) \
  $(LIBZioncoin_WALLET) \
  $(LIBZioncoin_ZMQ)

lib_LTLIBRARIES = $(LIBZioncoinCONSENSUS)

bin_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_ZioncoinD
  bin_PROGRAMS += Zioncoind
endif

if BUILD_Zioncoin_UTILS
  bin_PROGRAMS += Zioncoin-cli Zioncoin-tx
endif

.PHONY: FORCE check-symbols check-security
# Zioncoin core #
Zioncoin_CORE_H = \
  alias.h \
  offer.h \
  message.h \
  cert.h \
  escrow.h \
  feedback.h \
  auxpow.h \
  addrman.h \
  base58.h \
  bloom.h \
  blockencodings.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  core_io.h \
  core_memusage.h \
  httprpc.h \
  httpserver.h \
  indirectmap.h \
  init.h \
  key.h \
  keystore.h \
  dbwrapper.h \
  limitedmap.h \
  main.h \
  memusage.h \
  merkleblock.h \
  miner.h \
  net.h \
  netbase.h \
  noui.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
  primitives/block.h \
  primitives/pureheader.h \
  primitives/transaction.h \
  protocol.h \
  random.h \
  reverselock.h \
  rpc/client.h \
  rpc/protocol.h \
  rpc/server.h \
  rpc/register.h \
  scheduler.h \
  script/interpreter.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/ismine.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/pagelocker.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utiltime.h \
  validationinterface.h \
  versionbits.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/rpcwallet.h \
  wallet/wallet.h \
  wallet/walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libZioncoin_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between Zioncoind and Zioncoin-qt
libZioncoin_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libZioncoin_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libZioncoin_server_a_SOURCES = \
  alias.cpp \
  offer.cpp \
  message.cpp \
  cert.cpp \
  escrow.cpp \
  auxpow.cpp \
  addrman.cpp \
  bloom.cpp \
  blockencodings.cpp \
  chain.cpp \
  checkpoints.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  dbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
  rest.cpp \
  rpc/blockchain.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtransaction.cpp \
  rpc/server.cpp \
  script/sigcache.cpp \
  script/ismine.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  ui_interface.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  $(Zioncoin_CORE_H)

if ENABLE_ZMQ
libZioncoin_zmq_a_CPPFLAGS = $(Zioncoin_INCLUDES) $(ZMQ_CFLAGS)
libZioncoin_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libZioncoin_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between Zioncoind and Zioncoin-qt, but only linked
# when wallet enabled
libZioncoin_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_INCLUDES)
libZioncoin_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libZioncoin_wallet_a_SOURCES = \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/walletdb.cpp \
  policy/rbf.cpp \
  $(Zioncoin_CORE_H)

# crypto primitives library
crypto_libZioncoin_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_CONFIG_INCLUDES)
crypto_libZioncoin_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libZioncoin_crypto_a_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

# consensus: shared between all executables that validate any consensus rules.
libZioncoin_consensus_a_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_INCLUDES)
libZioncoin_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libZioncoin_consensus_a_SOURCES = \
  amount.h \
  arith_uint256.cpp \
  arith_uint256.h \
  consensus/merkle.cpp \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  hash.cpp \
  hash.h \
  prevector.h \
  primitives/block.cpp \
  primitives/block.h \
  primitives/pureheader.cpp \
  primitives/pureheader.h \
  primitives/transaction.cpp \
  primitives/transaction.h \
  pubkey.cpp \
  pubkey.h \
  script/Zioncoinconsensus.cpp \
  script/interpreter.cpp \
  script/interpreter.h \
  script/script.cpp \
  script/script.h \
  script/script_error.cpp \
  script/script_error.h \
  serialize.h \
  tinyformat.h \
  uint256.cpp \
  uint256.h \
  utilstrencodings.cpp \
  utilstrencodings.h \
  version.h

# common: shared between Zioncoind, and Zioncoin-qt and non-server tools
libZioncoin_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_INCLUDES)
libZioncoin_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libZioncoin_common_a_SOURCES = \
  amount.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/sign.cpp \
  script/standard.cpp \
  $(Zioncoin_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libZioncoin_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_INCLUDES)
libZioncoin_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libZioncoin_util_a_SOURCES = \
  support/pagelocker.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpc/protocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(Zioncoin_CORE_H)

if GLIBC_BACK_COMPAT
libZioncoin_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between Zioncoin-cli and Zioncoin-qt
libZioncoin_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_INCLUDES)
libZioncoin_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libZioncoin_cli_a_SOURCES = \
  rpc/client.cpp \
  $(Zioncoin_CORE_H)

nodist_libZioncoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# Zioncoind binary #
Zioncoind_SOURCES = Zioncoind.cpp
Zioncoind_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_INCLUDES)
Zioncoind_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
Zioncoind_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
Zioncoind_SOURCES += Zioncoind-res.rc
endif

Zioncoind_LDADD = \
  $(LIBZioncoin_SERVER) \
  $(LIBZioncoin_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBZioncoin_UTIL) \
  $(LIBZioncoin_WALLET) \
  $(LIBZioncoin_ZMQ) \
  $(LIBZioncoin_CONSENSUS) \
  $(LIBZioncoin_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

Zioncoind_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

# Zioncoin-cli binary #
Zioncoin_cli_SOURCES = Zioncoin-cli.cpp
Zioncoin_cli_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_INCLUDES) $(EVENT_CFLAGS)
Zioncoin_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
Zioncoin_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
Zioncoin_cli_SOURCES += Zioncoin-cli-res.rc
endif

Zioncoin_cli_LDADD = \
  $(LIBZioncoin_CLI) \
  $(LIBUNIVALUE) \
  $(LIBZioncoin_UTIL) \
  $(LIBZioncoin_CRYPTO)

Zioncoin_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# Zioncoin-tx binary #
Zioncoin_tx_SOURCES = Zioncoin-tx.cpp
Zioncoin_tx_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_INCLUDES)
Zioncoin_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
Zioncoin_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
Zioncoin_tx_SOURCES += Zioncoin-tx-res.rc
endif

Zioncoin_tx_LDADD = \
  $(LIBZioncoin_SERVER) \
  $(LIBZioncoin_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBZioncoin_UTIL) \
  $(LIBZioncoin_WALLET) \
  $(LIBZioncoin_ZMQ) \
  $(LIBZioncoin_CONSENSUS) \
  $(LIBZioncoin_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

Zioncoin_tx_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

#

# Zioncoinconsensus library #
if BUILD_Zioncoin_LIBS
include_HEADERS = script/Zioncoinconsensus.h
libZioncoinconsensus_la_SOURCES = $(crypto_libZioncoin_crypto_a_SOURCES) $(libZioncoin_consensus_a_SOURCES) $(libZioncoin_server_a_SOURCES)

if GLIBC_BACK_COMPAT
  libZioncoinconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libZioncoinconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libZioncoinconsensus_la_LIBADD = $(LIBSECP256K1)
libZioncoinconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) $(Zioncoin_INCLUDES) -DBUILD_Zioncoin_INTERNAL $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libZioncoinconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CTAES_DIST =  crypto/ctaes/bench.c
CTAES_DIST += crypto/ctaes/ctaes.c
CTAES_DIST += crypto/ctaes/ctaes.h
CTAES_DIST += crypto/ctaes/README.md
CTAES_DIST += crypto/ctaes/test.c

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = $(CTAES_DIST)

clean-local:
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf test/__pycache__

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
