// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The ZionCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"
// ZionCoin includes for gen block
#include "uint256.h"
#include "arith_uint256.h"
#include "hash.h"
#include "streams.h"
#include <time.h>
static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The Times 28/January/2018 Trying to make this coin work";
    const CScript genesisOutputScript = CScript() << ParseHex("54321afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

static void GenerateGenesisBlock(CBlockHeader &genesisBlock, uint256 *phash)
{
    // Write the first 76 bytes of the block header to a double-SHA256 state.
    genesisBlock.nTime    = time(NULL);
    CHash256 hasher;
    CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
    ss << genesisBlock;
    assert(ss.size() == 80);
    hasher.Write((unsigned char*)&ss[0], 76);
    arith_uint256 hashTarget = arith_uint256().SetCompact(genesisBlock.nBits);
    while (true) {
        

        // Write the last 4 bytes of the block header (the nonce) to a copy of
        // the double-SHA256 state, and compute the result.
        CHash256(hasher).Write((unsigned char*)&genesisBlock.nNonce, 4).Finalize((unsigned char*)phash);

        // Return the nonce if the hash has at least some zero bits,
        // check if it has enough to reach the target
        if (((uint16_t*)phash)[15] == 0 && UintToArith256(*phash) <= hashTarget)
            break;
        genesisBlock.nNonce++;
        if (genesisBlock.nNonce == 0) {
            printf("NONCE WRAPPED, incrementing time\n");
            ++genesisBlock.nTime;
        }
        // If nothing found after trying for a while, return -1
        if ((genesisBlock.nNonce & 0xfff) == 0)
            printf("nonce %08X: hash = %s (target = %s)\n",
                    genesisBlock.nNonce, (*phash).ToString().c_str(),
                    hashTarget.ToString().c_str());
    }
    printf("genesis.nTime = %u \n", genesisBlock.nTime);
    printf("genesis.nNonce = %u \n", genesisBlock.nNonce);
    printf("Generate hash = %s\n", (*phash).ToString().c_str());
    printf("genesis.hashMerkleRoot = %s\n", genesisBlock.hashMerkleRoot.ToString().c_str());
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
// ZionCoin parameters
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan =  6 * 60 * 60; // 6h retarget
        consensus.nPowTargetSpacing = 1 * 60; // 1 minute
        consensus.nAuxpowChainId = 0x1000;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916;
        consensus.nMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141 and BIP143)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1510704000; // November 15th, 2017.
        consensus.nMinimumChainWork = uint256S("0x00");
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xc4;
        pchMessageStart[1] = 0xb2;
        pchMessageStart[2] = 0xa3;
        pchMessageStart[3] = 0xc5;
        nDefaultPort = 1337;
        nPruneAfterHeight = 1000000;
        uint256 hash;

        genesis = CreateGenesisBlock(1517155795, 12805017, 0x1e0ffff0, 1, 8.88 * COIN);

        //CBlockHeader genesisHeader = genesis.GetBlockHeader();
        //GenerateGenesisBlock(genesisHeader, &hash);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000603568d5f7d569ee2c139c71e559052ee2d477679a48fe57581ebd66ec1"));
        assert(genesis.hashMerkleRoot == uint256S("0x9892302ff76adc0eaf010edf8476dcb87911e2f17f64670bcfb0c428bdaa4b76"));

        vFixedSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("85.10.208.156"));

        // ZionCoin/BITCOIN key prefixes
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,128);

        base58Prefixes[PUBKEY_ADDRESS_ZEC] = {0x1C,0xB8};
        base58Prefixes[SCRIPT_ADDRESS_ZEC] = {0x1C,0xBD};
        base58Prefixes[PUBKEY_ADDRESS_SYS] = std::vector<unsigned char>(1,63);
        
        base58Prefixes[SECRET_KEY_SYS] = std::vector<unsigned char>(1,191);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        
        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        vFixedSeeds.clear();

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

      /*  checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 11111, uint256S("0x444dbed9c994b2f88ccc8980f8d8952bb699de0a3ccb39633fd373289314c0e7"))
            ( 33333, uint256S("0x627457ac67fe173a66eaeb0f8d310c2b53743e95774a96b71bee44a10e854377"))
            ( 74000, uint256S("0x974f73d108f391bc80423b2a1191f081851631040e2faa354c4e558b1462ddbe"))
            (105000, uint256S("0x679ba4d205c15e07955bfe01bca38ce6e518a1974a29592482dd39b31d9886f7"))
            (134444, uint256S("0xf8a8e89e52ed90e8cdb70e5db462affb17ef4603eb637fb6ff0d2871b66ec526"))
            (168000, uint256S("0x42fc637a32f49db545dc462f26dc61c4445bf4d5f0e03f490f26e2ee089fe757"))
            (193000, uint256S("0x7537961e2c8cf0c0e08243305cf7d8e456a64909d9c279a39bf37df9928dd745"))
            ,
            1474160629, // * UNIX timestamp of last checkpoint block
            100000,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            1000.0     // * estimated number of transactions per day after checkpoint
        };*/
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x");
        consensus.powLimit = uint256S("0x"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 6 * 60 * 60; // 6h retarget
        consensus.nPowTargetSpacing = 1 * 60; // 1 minute
        consensus.nAuxpowChainId = 0x1000;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1493596800; // May 1st 2017
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");
        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        nDefaultPort = 1338;
        nPruneAfterHeight = 1000;
        uint256 hash;

        genesis = CreateGenesisBlock(1517156060, 15866405, 0x1e0ffff0, 1, 5000 * COIN);

        //CBlockHeader genesisHeader = genesis.GetBlockHeader();
        //GenerateGenesisBlock(genesisHeader, &hash);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000004b16c433428dee40161f9d6a93c16efd85802a29af75afda730fc3b01e6"));
        assert(genesis.hashMerkleRoot == uint256S("0x5c2ae7e1379e60a8be648d53aabad7b41b1719a3c800306b9d18f997bcc6f030"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);

        base58Prefixes[PUBKEY_ADDRESS_ZEC] = std::vector<unsigned char>(0x1C,0xB8);
        base58Prefixes[PUBKEY_ADDRESS_SYS] = std::vector<unsigned char>(1,65);
        base58Prefixes[SCRIPT_ADDRESS_ZEC] = std::vector<unsigned char>(0x1C,0xBD);
        base58Prefixes[SECRET_KEY_SYS] =     std::vector<unsigned char>(1,193);

        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        fRequireStandard = false;      
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;


    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 40000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 6 * 60 * 60; // 6h
        consensus.nPowTargetSpacing = 1 * 60; // 1 minute
        consensus.nAuxpowChainId = 0x1000;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 1339;
        nPruneAfterHeight = 1000;
        uint256 hash;

        genesis = CreateGenesisBlock(1517156221, 7250315, 0x207fffff, 1, 50000 * COIN);

        //CBlockHeader genesisHeader = genesis.GetBlockHeader();
        //GenerateGenesisBlock(genesisHeader, &hash);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000039ff2b926ef174d2bd4b29fd7e6b4efef0863c9f993648f3468c99d772a4"));
        assert(genesis.hashMerkleRoot == uint256S("0x0e090c9e45dffe4f938e8cfb967bd1e6542843bc58d7bd31093d7d91ba191f3c")); 

        vFixedSeeds.clear();
        vSeeds.clear();

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);

        
        base58Prefixes[PUBKEY_ADDRESS_ZEC] = {0x1C,0xB8};
        base58Prefixes[SCRIPT_ADDRESS_ZEC] = {0x1C,0xBD};
        base58Prefixes[PUBKEY_ADDRESS_SYS] = std::vector<unsigned char>(1,65);
        base58Prefixes[SECRET_KEY_SYS] =     std::vector<unsigned char>(1,193);

        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
 
