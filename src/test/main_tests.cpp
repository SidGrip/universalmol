// Copyright (c) 2014-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "chain.h"
#include "validation.h"
#include "net.h"

#include "test/test_bitcoin.h"

#include <boost/signals2/signal.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(main_tests, TestingSetup)

static std::vector<CBlockIndex> BuildLinearChain(int height, unsigned int nBits)
{
    std::vector<CBlockIndex> chain(height + 1);
    for (int i = 0; i <= height; ++i) {
        chain[i].nHeight = i;
        chain[i].nBits = nBits;
        chain[i].pprev = i > 0 ? &chain[i - 1] : nullptr;
    }
    return chain;
}

static void TestLegacySubsidySchedule(const Consensus::Params& consensusParams)
{
    const unsigned int easyBits = 0x1d00ffff;
    const unsigned int hardBits = 0x1c00ffff;

    BOOST_CHECK_EQUAL(GetBlockSubsidy(0, consensusParams, nullptr), 1 * COIN);

    auto launchChain = BuildLinearChain(1440, easyBits);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(1, consensusParams, &launchChain[0]), COIN / 1000);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(1440, consensusParams, &launchChain[1439]), COIN / 1000);

    auto stableChain = BuildLinearChain(1441, easyBits);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(1441, consensusParams, &stableChain[1440]), 2 * COIN);

    auto borkChain = BuildLinearChain(3600, easyBits);
    borkChain[3540].nBits = easyBits;
    borkChain[3560].nBits = easyBits;
    borkChain[3580].nBits = easyBits;
    borkChain[3599].nBits = hardBits;
    BOOST_CHECK_EQUAL(GetBlockSubsidy(3600, consensusParams, &borkChain[3599]), 2 * COIN);

    auto risingChain = BuildLinearChain(4000, easyBits);
    risingChain[3940].nBits = easyBits;
    risingChain[3960].nBits = easyBits;
    risingChain[3980].nBits = easyBits;
    risingChain[3999].nBits = hardBits;
    BOOST_CHECK_EQUAL(GetBlockSubsidy(4000, consensusParams, &risingChain[3999]), COIN / 10);
}

BOOST_AUTO_TEST_CASE(block_subsidy_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    TestLegacySubsidySchedule(chainParams->GetConsensus());
}

BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    const auto& consensusParams = chainParams->GetConsensus();
    auto borkChain = BuildLinearChain(3600, 0x1d00ffff);
    auto launchChain = BuildLinearChain(1441, 0x1d00ffff);
    auto risingChain = BuildLinearChain(4000, 0x1d00ffff);

    BOOST_CHECK(MoneyRange(GetBlockSubsidy(0, consensusParams, nullptr)));
    BOOST_CHECK(MoneyRange(GetBlockSubsidy(1, consensusParams, &launchChain[0])));
    BOOST_CHECK(MoneyRange(GetBlockSubsidy(1440, consensusParams, &launchChain[1439])));
    BOOST_CHECK(MoneyRange(GetBlockSubsidy(1441, consensusParams, &launchChain[1440])));
    BOOST_CHECK(MoneyRange(GetBlockSubsidy(3600, consensusParams, &borkChain[3599])));
    BOOST_CHECK(MoneyRange(GetBlockSubsidy(4000, consensusParams, &risingChain[3999])));
}

bool ReturnFalse() { return false; }
bool ReturnTrue() { return true; }

BOOST_AUTO_TEST_CASE(test_combiner_all)
{
    boost::signals2::signal<bool (), CombinerAll> Test;
    BOOST_CHECK(Test());
    Test.connect(&ReturnFalse);
    BOOST_CHECK(!Test());
    Test.connect(&ReturnTrue);
    BOOST_CHECK(!Test());
    Test.disconnect(&ReturnFalse);
    BOOST_CHECK(Test());
    Test.disconnect(&ReturnTrue);
    BOOST_CHECK(Test());
}
BOOST_AUTO_TEST_SUITE_END()
