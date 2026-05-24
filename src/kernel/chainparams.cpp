// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kernel/chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/amount.h>
#include <consensus/merkle.h>
#include <consensus/params.h>
#include <hash.h>
#include <chainparamsbase.h>
#include <logging.h>
#include <primitives/block.h>
#include <primitives/transaction.h>
#include <script/interpreter.h>
#include <script/script.h>
#include <uint256.h>
#include <util/strencodings.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>

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
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
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
    const char* pszTimestamp = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

static CBlock CreateUniversalMoleculeGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // UniversalMolecule's own coinbase scriptSig text from coin-source-of-truth.md
    // and legacy 0.8 main.cpp. Mainnet genesis hash
    // 0x00000059f24d9e85501bd3873fac0cd6e8a43fd8c20eee856082dbdcc09a8e64 and
    // merkle root 0x11ad2754baede90db86d491ada7030551bd3f0d72a7486ef57fe8fcf44c3b6b4
    // are reproducible from this factory + (1405307607, 79480397, 503382015, 112, 1 * COIN).
    const char* pszTimestamp = "LaPatilla 13/07/2014 Gobierno argentino habia recibido denuncias sobre mega-guiso";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network on which people trade goods and services.
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = std::numeric_limits<int>::max();
        // UniversalMolecule-0.25.2 post-SegWit cleanup group. These are intentional
        // future-height mainnet activations after the verified 0.15.21 SegWit
        // activation at height 4108645.
        consensus.BIP34Height = 4141188;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 4141188;
        consensus.BIP66Height = 4141188;
        // TODO(blakestream-25.2-activation): CSV (BIP68/112/113) — atomic-swap timeout
        // primitive. ALWAYS_ACTIVE from genesis on Blakestream family per
        // coin-source-of-truth.md "Common rules". Do NOT change.
        consensus.CSVHeight = 1;
        // UniversalMolecule SegWit is inherited from the 0.15.21 mainnet activation.
        // 0.25.2 buries this height and does not re-signal SegWit.
        consensus.SegwitHeight = 4108645;
        // Ignore historical unknown-versionbit signaling before buried SegWit.
        // Warnings remain enabled after this point.
        consensus.MinBIP9WarningHeight = 4108645;
        consensus.powLimit = uint256S("000000ffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 600;
        consensus.nPowTargetSpacing = 120;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 4;
        consensus.nMinerConfirmationWindow = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;
        // Taproot bundle BIP340/BIP341/BIP342
        // (Schnorr + Taproot key/script commitments + Tapscript). This is what enables
        // PTLC-style atomic swaps and MuSig2 cross-chain DEX paths. ALL THE C++
        // MACHINERY IS ALREADY PRESENT in this codebase (verified 2026-04-25):
        // Schnorr verify, key-path, script-path, OP_CHECKSIGADD, TaggedHash byte-
        // identity to upstream Bitcoin Core. Activation values are assigned below.
        //
        // Taproot follows the 0.25.2 cleanup group in a separate BIP9 window.
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = 1782871200;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = 1814407200;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 4146228;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000658184225d0a716708");
        consensus.defaultAssumeValid = uint256S("0xb0c8a3625ed218dd8c168715c17cfc5bd4bd6a4d8b6273ceb27982b7c196ca77");

        // UniversalMolecule AuxPoW chain identity (consumed by Phase 2 AuxPoW core).
        // mainnet: strict chain-ID, AuxPoW activates at historical height 160000.
        consensus.fStrictChainId = true;
        consensus.nAuxpowChainId = 0x000F;
        consensus.nAuxpowStartHeight = 160000;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         *
         * UniversalMolecule mainnet has its own byte sequence.
         */
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xd3;
        pchMessageStart[2] = 0xe7;
        pchMessageStart[3] = 0xf4;
        nDefaultPort = 24785;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        genesis = CreateUniversalMoleculeGenesisBlock(1405307607, 79480397, 503382015, 112, 1 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000059f24d9e85501bd3873fac0cd6e8a43fd8c20eee856082dbdcc09a8e64"));
        assert(genesis.hashMerkleRoot == uint256S("0x11ad2754baede90db86d491ada7030551bd3f0d72a7486ef57fe8fcf44c3b6b4"));

        // Shared BlakeStream aux-coin DNS seeds.
        vSeeds.emplace_back("seed.blakestream.io");
        vSeeds.emplace_back("seed.blakecoin.org");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,130);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,7);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "umo";
        vFixedSeeds.clear();

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        // UniversalMolecule mainnet checkpoints, lifted from UniversalMolecule-0.15.21
        // and refreshed with live 0.25.2 anchors through height 4120000.
        checkpointData = {
            {
                {0,       uint256S("0x00000059f24d9e85501bd3873fac0cd6e8a43fd8c20eee856082dbdcc09a8e64")},
                {76213,   uint256S("0x4549b6d0818927ba374f2598aec5eb29837b44e5c28f6f96124acd16c4cc85ae")},
                {353493,  uint256S("0x33ccccb250b7aeec44c5e244b8e6dc7b5b67b44714bbc88cafc2e49f99e3e494")},
                {468432,  uint256S("0x5d477f42490c81a681489ceba66c4a811613a172d0e901590d206aecaea351c9")},
                {2509078, uint256S("0xf002204932fd73ecf2eaf6c662cb0a2ca3178752818d1f94dafe2b0a1da21be3")},
                {4082265, uint256S("0x59d088a108d7c870c4b8d4c957273e59092b9cf62bd26c8e12b69060c5c7ff0f")},
                {4108645, uint256S("0x0b5d80b6d5d72c43f16be3947bae07652eee910fb477af657f6e2e38ff954b78")},
                {4110000, uint256S("0xcfc76747b2b3aef3d2964617b9c39cd20040db1ddbc29ea71a8bde54d463a3bc")},
                {4115000, uint256S("0xe1817a0351f5ce82d0830fa42c8c49f9ee1f7516371d22e2d52697e376cc064d")},
                {4120000, uint256S("0xb0c8a3625ed218dd8c168715c17cfc5bd4bd6a4d8b6273ceb27982b7c196ca77")},
            }
        };

        m_assumeutxo_data = MapAssumeutxo{
         // TODO to be specified in a future patch.
        };

        chainTxData = ChainTxData{
            .nTime    = 1779498269,
            .nTxCount = 4604996,
            .dTxRate  = 0.01094604345594882,
        };
    }
};

/**
 * Testnet (v3): public test network which is reset from time to time.
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = std::numeric_limits<int>::max();
        // TODO(blakestream-25.2-activation): testnet mirrors mainnet's BIP34/BIP65/BIP66
        // disabled-by-far-future-height pattern. See mainnet block above for full
        // rationale + activation procedure.
        consensus.BIP34Height = 100000000;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 100000000;
        consensus.BIP66Height = 100000000;
        // TODO(blakestream-25.2-activation): CSV (BIP68/112/113) ALWAYS_ACTIVE on
        // Blakestream family — atomic-swap timeout primitive. Do NOT change.
        consensus.CSVHeight = 1;
        // TODO(blakestream-25.2-activation): testnet SegWit ALWAYS_ACTIVE from height
        // 1 so testnet AuxPoW + atomic-swap regression coverage works without
        // waiting on the 0.15.21 mainnet activation. Do NOT change.
        consensus.SegwitHeight = 1;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("000000ffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 600;
        consensus.nPowTargetSpacing = 120;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 4;
        consensus.nMinerConfirmationWindow = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;
        // TODO(blakestream-25.2-activation): testnet Taproot — when mainnet Taproot
        // activation values get set (post-0.15.21-SegWit), set testnet to a slightly
        // earlier window so testnet covers activation before mainnet. Currently
        // NEVER_ACTIVE. See mainnet block above for full procedure.
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0;

        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};

        // UniversalMolecule AuxPoW: testnet does NOT enforce strict chain-ID, and AuxPoW
        // is acceptable from genesis (no historical pre-AuxPoW height).
        consensus.fStrictChainId = false;
        consensus.nAuxpowChainId = 0x000F;
        consensus.nAuxpowStartHeight = 0;

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x39;
        pchMessageStart[3] = 0x38;
        nDefaultPort = 18449;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        // UniversalMolecule testnet has its OWN distinct genesis (different nTime/nNonce
        // from mainnet/regtest). Lifted verbatim from UniversalMolecule-0.15.21
        // chainparams.cpp:248-250.
        genesis = CreateUniversalMoleculeGenesisBlock(1405307607, 79480397, 503382015, 112, 1 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000059f24d9e85501bd3873fac0cd6e8a43fd8c20eee856082dbdcc09a8e64"));
        assert(genesis.hashMerkleRoot == uint256S("0x11ad2754baede90db86d491ada7030551bd3f0d72a7486ef57fe8fcf44c3b6b4"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // UniversalMolecule testnet seeds to be added when available.

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,142);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,170);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tumo";

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0, uint256S("0x00000059f24d9e85501bd3873fac0cd6e8a43fd8c20eee856082dbdcc09a8e64")},
            }
        };

        m_assumeutxo_data = MapAssumeutxo{
            // TODO to be specified in a future patch.
        };

        chainTxData = ChainTxData{
            .nTime    = 1392351202,
            .nTxCount = 1,
            .dTxRate  = 0.01,
        };
    }
};

/**
 * Signet: test network with an additional consensus parameter (see BIP325).
 */
class SigNetParams : public CChainParams {
public:
    explicit SigNetParams(const SigNetOptions& options)
    {
        std::vector<uint8_t> bin;
        vSeeds.clear();

        if (!options.challenge) {
            // UniversalMolecule signet defaults to a local/private developer network.
            // Keep the default challenge trivial and ship no global seeds,
            // assumevalid, or chainwork so we do not point at Bitcoin signet.
            bin = ParseHex("51");
            consensus.nMinimumChainWork = uint256{};
            consensus.defaultAssumeValid = uint256{};
            m_assumed_blockchain_size = 0;
            m_assumed_chain_state_size = 0;
            chainTxData = ChainTxData{0, 0, 0};
        } else {
            bin = *options.challenge;
            consensus.nMinimumChainWork = uint256{};
            consensus.defaultAssumeValid = uint256{};
            m_assumed_blockchain_size = 0;
            m_assumed_chain_state_size = 0;
            chainTxData = ChainTxData{
                0,
                0,
                0,
            };
            LogPrintf("Signet with challenge %s\n", HexStr(bin));
        }

        if (options.seeds) {
            vSeeds = *options.seeds;
        }

        strNetworkID = CBaseChainParams::SIGNET;
        consensus.signet_blocks = true;
        consensus.signet_challenge.assign(bin.begin(), bin.end());
        consensus.nSubsidyHalvingInterval = std::numeric_limits<int>::max();
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 1;
        consensus.nPowTargetTimespan = 600;
        consensus.nPowTargetSpacing = 120;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 4;
        consensus.nMinerConfirmationWindow = 5;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("00000377ae000000000000000000000000000000000000000000000000000000");
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        // Keep Taproot always active on signet so developer coverage matches
        // regtest/testnet, while mainnet activation policy waits on
        // UniversalMolecule-0.15.21 mainnet SegWit activation results.
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay

        // message start is defined as the first 4 bytes of the sha256d of the block script
        HashWriter h{};
        h << consensus.signet_challenge;
        uint256 hash = h.GetHash();
        memcpy(pchMessageStart, hash.begin(), 4);

        nDefaultPort = 38733;
        nPruneAfterHeight = 1000;

        // UniversalMolecule signet: defaults to the testnet genesis params (UniversalMolecule-0.15.21
        // never shipped mainnet signet; signet is a Bitcoin-Core-25.2 inherited
        // facility used here for developer experimentation only).
        consensus.fStrictChainId = false;
        consensus.nAuxpowChainId = 0x000F;
        consensus.nAuxpowStartHeight = 0;
        genesis = CreateUniversalMoleculeGenesisBlock(1405307607, 79480397, 503382015, 112, 1 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000059f24d9e85501bd3873fac0cd6e8a43fd8c20eee856082dbdcc09a8e64"));
        assert(genesis.hashMerkleRoot == uint256S("0x11ad2754baede90db86d491ada7030551bd3f0d72a7486ef57fe8fcf44c3b6b4"));

        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,142);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,170);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tumo";

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = false;
    }
};

/**
 * Regression test: intended for private networks only. Has minimal difficulty to ensure that
 * blocks can be found instantly.
 */
class CRegTestParams : public CChainParams
{
public:
    explicit CRegTestParams(const RegTestOptions& opts)
    {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = std::numeric_limits<int>::max();
        consensus.BIP34Height = 100000000;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 1351;
        consensus.BIP66Height = 1251;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 600;
        consensus.nPowTargetSpacing = 120;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108;
        consensus.nMinerConfirmationWindow = 144;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay

        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};

        // UniversalMolecule regtest: AuxPoW machinery available from genesis, no
        // strict chain-ID enforcement (mirrors UniversalMolecule-0.15.21).
        consensus.fStrictChainId = false;
        consensus.nAuxpowChainId = 0x000F;
        consensus.nAuxpowStartHeight = 0;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = opts.fastprune ? 100 : 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        for (const auto& [dep, height] : opts.activation_heights) {
            switch (dep) {
            case Consensus::BuriedDeployment::DEPLOYMENT_SEGWIT:
                consensus.SegwitHeight = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_HEIGHTINCB:
                consensus.BIP34Height = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_DERSIG:
                consensus.BIP66Height = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_CLTV:
                consensus.BIP65Height = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_CSV:
                consensus.CSVHeight = int{height};
                break;
            }
        }

        for (const auto& [deployment_pos, version_bits_params] : opts.version_bits_parameters) {
            consensus.vDeployments[deployment_pos].nStartTime = version_bits_params.start_time;
            consensus.vDeployments[deployment_pos].nTimeout = version_bits_params.timeout;
            consensus.vDeployments[deployment_pos].min_activation_height = version_bits_params.min_activation_height;
        }

        // UniversalMolecule regtest reuses the UniversalMolecule mainnet genesis parameters
        // (UniversalMolecule-0.15.21 chainparams.cpp:332-334). Testnet has its own
        // distinct genesis and is NOT shared with regtest/mainnet.
        genesis = CreateUniversalMoleculeGenesisBlock(1405307607, 79480397, 503382015, 112, 1 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000059f24d9e85501bd3873fac0cd6e8a43fd8c20eee856082dbdcc09a8e64"));
        assert(genesis.hashMerkleRoot == uint256S("0x11ad2754baede90db86d491ada7030551bd3f0d72a7486ef57fe8fcf44c3b6b4"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();
        vSeeds.emplace_back("dummySeed.invalid.");

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = true;

        checkpointData = {
            {
                {0, uint256S("0x00000059f24d9e85501bd3873fac0cd6e8a43fd8c20eee856082dbdcc09a8e64")},
            }
        };

        // UniversalMolecule regtest assumeutxo snapshot at height 110. The donor
        // hash does not apply: UniversalMolecule regtest
        // has a different genesis, so the height-110 chainstate (txoutset)
        // hash is different. Captured by running the regtest 110-block
        // TestChain100Setup sequence and dumping the resulting snapshot's
        // `txoutset_hash`.
        m_assumeutxo_data = MapAssumeutxo{
            {
                110,
                {
                    AssumeutxoHash{uint256S("0x09d3598d6409eb21800ec1f2fa6b8666442590fc54e04750a732598754d8cd42")},
                    111,
                },
            },
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,130);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,7);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "rumo";
    }
};

std::unique_ptr<const CChainParams> CChainParams::SigNet(const SigNetOptions& options)
{
    return std::make_unique<const SigNetParams>(options);
}

std::unique_ptr<const CChainParams> CChainParams::RegTest(const RegTestOptions& options)
{
    return std::make_unique<const CRegTestParams>(options);
}

std::unique_ptr<const CChainParams> CChainParams::Main()
{
    return std::make_unique<const CMainParams>();
}

std::unique_ptr<const CChainParams> CChainParams::TestNet()
{
    return std::make_unique<const CTestNetParams>();
}
