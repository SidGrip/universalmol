# UniversalMolecule 0.15.2 Update — QC Report

Re-run date: 2026-04-11
Repo: `/home/sid/Blakestream-Installer/repos/universalmol-0.15.21/`
Source of truth: `universalmol-0.15.21.md`
Donor 0.8.15.5 source: `../universalmol/`

---

## Coin Metrics Summary

| Parameter | Expected (source of truth) | 0.15.2 value | File | Status |
|-----------|---------------------------|--------------|------|--------|
| Coin name | UniversalMolecule | `UniversalMolecule Core` | `configure.ac:11` | PASS |
| Ticker | UMO | — | — | PASS (metadata) |
| Binary names | universalmoleculed / -qt / -cli / -tx | universalmoleculed / -qt / -cli / -tx | `configure.ac:17-20`, `build.sh:32-37` | PASS |
| Version | 0.15.2 | 0.15.2 | `build.sh:38` | PASS |
| Algorithm | BLAKE-256 (8 rounds) | `Hashblake` wire + `GetPoWHash` path | `src/hash.h:90`, `src/pow.cpp:103` | PASS |
| Genesis hash | `00000059f24d9e85501bd3873fac0cd6e8a43fd8c20eee856082dbdcc09a8e64` | same | `chainparams.cpp:139` | PASS |
| Genesis merkle root | `11ad2754baede90db86d491ada7030551bd3f0d72a7486ef57fe8fcf44c3b6b4` | same | `chainparams.cpp:140` | PASS |
| Genesis nTime | 1405307607 | 1405307607 | `chainparams.cpp:138` | PASS |
| Genesis nNonce | 79480397 | 79480397 | `chainparams.cpp:138` | PASS |
| Genesis nVersion | 112 | 112 | `chainparams.cpp:138` | PASS |
| Genesis reward | 1 COIN | `1 * COIN` | `chainparams.cpp:138` | PASS |
| pchMessageStart (mainnet) | `0xfa 0xd3 0xe7 0xf4` | `0xfa 0xd3 0xe7 0xf4` | `chainparams.cpp:131-134` | PASS |
| pchMessageStart (testnet) | `0x0b 0x11 0x39 0x38` | `0x0b 0x11 0x39 0x38` | `chainparams.cpp:221-224` | PASS |
| P2P port (mainnet) | 24785 | 24785 | `chainparams.cpp:135` | PASS |
| P2P port (testnet) | 18449 | 18449 | `chainparams.cpp:225` | PASS |
| RPC port (mainnet) | 5921 | 5921 | `chainparamsbase.cpp:35` | PASS |
| RPC port (testnet) | 19738 | 19738 | `chainparamsbase.cpp:47` | PASS |
| Pubkey address (mainnet) | 130 (0x82) | 130 | `chainparams.cpp:146` | PASS |
| Script address (mainnet) | 7 (0x07) | 7 | `chainparams.cpp:147` | PASS |
| Secret key (mainnet) | 128 | 128 | `chainparams.cpp:148` | PASS |
| Pubkey address (testnet) | 142 | 142 | `chainparams.cpp:236` | PASS |
| Script address (testnet) | 170 | 170 | `chainparams.cpp:237` | PASS |
| Secret key (testnet) | 239 | 239 | `chainparams.cpp:238` | PASS |
| Bech32 HRP (mainnet) | `umo` | `umo` | `chainparams.cpp:151` | PASS |
| Bech32 HRP (testnet) | `tumo` | `tumo` | `chainparams.cpp:241` | PASS |
| Bech32 HRP (regtest) | — | `rumo` | `chainparams.cpp:340` | PASS (added) |
| Block time | 120s | `2 * 60` | `chainparams.cpp:92` | PASS |
| Target timespan | 600s | `10 * 60` | `chainparams.cpp:91` | PASS |
| Retarget interval | 5 blocks | 600 / 120 = 5 via `DifficultyAdjustmentInterval()` | `consensus/params.h:69` | PASS |
| Coinbase maturity | 120 | 120 | `consensus/consensus.h:19` | PASS |
| MAX_MONEY | 10,512,000,144 * COIN / 100 | `10512000144LL * COIN / 100` | `amount.h:26` | PASS |
| Subsidy halving | disabled (legacy dynamic) | `std::numeric_limits<int>::max()` | `chainparams.cpp:84` | PASS |
| AuxPoW chain ID | 0x000F | 0x000f | `chainparams.cpp:96` | PASS |
| AuxPoW start (mainnet) | 160000 (nominal) + pre-start tolerance | 160000 + tolerance in `pow.cpp:145-146` | `chainparams.cpp:101`, `pow.cpp:125-156` | PASS |
| AuxPoW start (testnet) | 0 | 0 | `chainparams.cpp:198` | PASS |
| AuxPoW start (regtest) | 0 | 0 | `chainparams.cpp:285` | PASS |
| fStrictChainId (mainnet) | true | true | `chainparams.cpp:95` | PASS |
| fStrictChainId (testnet) | false | false | `chainparams.cpp:196` | PASS |
| BIP34/65/66 | disabled (height 1e8) | `100000000` | `chainparams.cpp:86-89` | PASS |
| CSV deployment | ALWAYS_ACTIVE | ALWAYS_ACTIVE | `chainparams.cpp:110-111` | PASS |
| SegWit deployment (mainnet) | start 1778457600 / timeout 1809993600 | 1778457600 / 1809993600 | `chainparams.cpp:116-117` | PASS |
| SegWit (testnet/regtest) | ALWAYS_ACTIVE | ALWAYS_ACTIVE | `chainparams.cpp:211-213`, `294-296` | PASS |
| Signed message header | `"UniversalMolecule Signed Message:\n"` | same | `validation.cpp:103` | PASS |

---

## Reward Formula Side-by-Side

Original 0.8.x (`../universalmol/src/main.cpp:1115-1136`):

```cpp
int64 static GetBlockValue(int nHeight, int64 nFees, unsigned int nBits, bool diffWasUp)
{
    if (nHeight == 0) {
        return nGenesisBlockRewardCoin;               // 1 COIN
    }
    int64 subsidy = 0;
    if (nHeight <= nMinHeightForFullReward) {         // nMinHeightForFullReward = 1440
        subsidy = 1 * COIN / 1000;                    // 0.001 UMO
    } else {
        if (diffWasUp) {
            subsidy = COIN / 10;                      // 0.1 UMO (difficulty rising)
        } else {
            subsidy = 2 * COIN;                       // 2 UMO (stable/falling)
        }
    }
    return subsidy + nFees;
}
```

Supporting `CummulativeDifficultyMovingAverage` (`../universalmol/src/main.cpp:1138-1234`) has the legacy "bork":

```cpp
int bork = 0;
if (baseHeight < 3600) {
    return false;           // forced low-reward path through height 3599
} else if (baseHeight < 4000) {
    bork = 1;               // heights 3600..3999 compare using CBigNum targets
}
// ...
if (bork == 1) {
    return baseDiff > avgDiff;          // target-space comparison
} else {
    return baseDiffdouble > avgDiffDouble;  // double-precision difficulty comparison (>= 4000)
}
```

Port 0.15.2 (`src/validation.cpp:1146-1160`):

```cpp
CAmount GetBlockSubsidy(int nHeight, const Consensus::Params& consensusParams, const CBlockIndex* pindexPrev)
{
    (void)consensusParams;
    if (nHeight == 0)
        return COIN;                               // 1 UMO genesis
    if (nHeight <= LEGACY_REWARD_LAUNCH_HEIGHT)    // 1440
        return COIN / 1000;                        // 0.001 UMO
    if (pindexPrev == nullptr)
        return 0;
    return LegacyRewardDifficultyTrendIsRising(pindexPrev) ? COIN / 10 : 2 * COIN;
}
```

Supporting `LegacyRewardDifficultyTrendIsRising` (`src/validation.cpp:107-176`) preserves the bork windows:

```cpp
static const int64_t LEGACY_REWARD_LAUNCH_HEIGHT   = 1440;
static const int64_t LEGACY_REWARD_SAMPLE_SPACING  = 20;   // nBlockSpacing * 2
static const int64_t LEGACY_REWARD_SAMPLE_COUNT    = 3;    // nCumulativeDiffMovingAverageNIntervals
static const int64_t LEGACY_REWARD_BORK_HEIGHT     = 3600;
static const int64_t LEGACY_REWARD_BORK_END_HEIGHT = 4000;

if (baseHeight < LEGACY_REWARD_BORK_HEIGHT)
    return false;                                  // matches legacy "bork < 3600" forced-low branch

if (baseHeight < LEGACY_REWARD_BORK_END_HEIGHT) {
    // 3600..3999: CBigNum-equivalent arith_uint256 target-space comparison
    return baseTarget > avgTarget;
}

// >= 4000: double-precision GetDifficulty comparison (matches baseDiffdouble branch)
return baseDifficulty > averageDifficulty;
```

Reward formula verdict: **PASS**

- Genesis = 1 UMO: matches.
- Heights 1..1440 = 0.001 UMO: matches (uses `<= 1440`, same as legacy `<= nMinHeightForFullReward`).
- Heights 1441..3599: legacy `bork == 0 && baseHeight < 3600` forces `return false`, so `diffWasUp == false`, yielding `2 UMO`. Port replicates this via the `baseHeight < LEGACY_REWARD_BORK_HEIGHT` early return. Matches.
- Heights 3600..3999: legacy compares CBigNum targets (`baseDiff > avgDiff`). Port uses `arith_uint256.SetCompact` from `nBits` with `baseTarget > avgTarget`. Target-space equivalent to the CBigNum path, preserves legacy semantics. Matches.
- Heights >= 4000: legacy uses `GetDifficulty` (double-precision) comparison. Port uses `GetDifficulty(pindexSample)` doubles on the same sample grid. Matches.
- Sample grid (`LEGACY_REWARD_SAMPLE_SPACING = 20`, `LEGACY_REWARD_SAMPLE_COUNT = 3`) equals legacy `nBlockSpacing * 2 = 20` and `nCumulativeDiffMovingAverageNIntervals = 3`. Matches.
- Port takes `pindexPrev` context, which the source of truth explicitly calls out. `ConnectBlock` wires `pindex->pprev` (`validation.cpp:1931`). Matches.

Minor semantic notes (not blocking):

- The legacy `bork == 1` window averages CBigNum targets, which is mathematically the reciprocal of difficulty averaging. The port's `arith_uint256` version preserves that reciprocal path. Any regression would only show up at heights 3600..3999, a short historical window — worth a targeted replay spot-check but not a correctness blocker.

---

## Source-of-Truth Claim Verification

| Claim | Evidence | Status |
|-------|----------|--------|
| Secret key prefixes 128 / 239 | `chainparams.cpp:148`, `chainparams.cpp:238` | PASS |
| RPC ports 5921 mainnet / 19738 testnet (from source) | `chainparamsbase.cpp:35,47`; legacy `bitcoinrpc.cpp:44` | PASS |
| Reward: genesis=1, 1..1440=0.001, rising=0.1, stable/falling=2 | `validation.cpp:1146-1160` | PASS |
| Legacy `baseHeight < 3600` / `3600..3999` quirk preserved | `validation.cpp:107-176` bork constants and branches | PASS |
| AuxPoW chain ID 0x000F | `chainparams.cpp:96` (mainnet), `:197` (testnet), `:284` (regtest) | PASS |
| AuxPoW nominal start 160000 with pre-start tolerance | `chainparams.cpp:101` + `pow.cpp:145-146` returns `true` for AuxPoW-bearing blocks when `!auxpowActive` | PASS |
| Legacy 0.8.x had identical pre-start tolerance | `../universalmol/src/main.cpp:2203-2206` returns `true` unconditionally when `nHeight < GetAuxPowStartBlock() && auxpow.get() != NULL` | PASS (matches) |
| Testnet / regtest use chain ID 0x000F, start 0, strict disabled | `chainparams.cpp:196-198`, `283-285` | PASS |
| Shared AuxPoW framework integrated | `src/auxpow.{h,cpp}`, `src/primitives/pureheader.{h,cpp}` present | PASS |
| AuxPoW-aware block/header serialization | `src/primitives/block.{h,cpp}` present | PASS |
| AuxPoW-aware disk index persistence | `txdb.cpp:291` calls `CheckAuxPowProofOfWork` on reload | PASS |
| AuxPoW-aware PoW validation | `pow.cpp:125-156` `CheckAuxPowProofOfWork` | PASS |
| Chain-ID-aware block template version | `miner.cpp:137` `nHeight >= consensusParams.nAuxpowStartHeight` branch | PASS |
| Checkpoints from original UMO | `chainparams.cpp:160-168` — heights 0, 76213, 353493, 468432, 2509078 match `../universalmol/src/checkpoints.cpp:39-42` | PASS |
| chainTxData from original UMO | `chainparams.cpp:170-174` — `1630699667 / 2861736 / 2000/86400` matches legacy | PASS |
| Donor Blakecoin seed defaults removed | `chainparams.cpp:143-144` now uses `blakestream.io` / `blakecoin.org`; `vFixedSeeds.clear()` on line 153 | PARTIAL (see action items) |
| Mainnet `early-auxpow-block` blanket reject removed; pre-start AuxPoW tolerated | `pow.cpp:145-146`: `if (!auxpowActive) return true;` | PASS |
| Replay reached height 5032 before hitting the legacy reject | Source-of-truth claim; not re-run here | NOT RE-VERIFIED |
| BIP30 relaxation preserved (`!pindex->phashBlock`) | `validation.cpp:1832: bool fEnforceBIP30 = !pindex->phashBlock;` matches `../universalmol/src/main.cpp:1801` | PASS |
| `createauxblock <address>` + `submitauxblock <hash> <auxpow>` primary | `rpc/mining.cpp:936-994` | PASS |
| `getauxblock` compatibility wrapper using the same block-template / block-submit flow | `rpc/mining.cpp:996-1058` — reuses `AuxMiningCreateBlock` / `AuxMiningSubmitBlock` | PASS |
| `getworkaux` out of scope | Not present in `rpc/mining.cpp` | PASS |
| Wire checksum stays on `Hashblake` (not Blakecoin's handshake exception) | `net.cpp:832`, `net.cpp:2931`, `hash.h:90` | PASS |
| SegWit mainnet start 1778457600 / timeout 1809993600 | `chainparams.cpp:116-117` | PASS |
| CSV / testnet / regtest SegWit ALWAYS_ACTIVE | `chainparams.cpp:110-111`, `211-213`, `294-296` | PASS |
| Signed message header "UniversalMolecule Signed Message:\n" | `validation.cpp:103` | PASS |
| `make -C src -j4 blakecoind blakecoin-cli` succeeds after fan-out | Prebuilt artifacts `src/blakecoind` and `src/blakecoin-cli` present | PASS (binaries present) |
| Regtest `wallet-basic-smoke.py` passes | Source-of-truth claim; not re-run here | NOT RE-VERIFIED |
| AuxPow regtest returned `chainid == 15` | Source-of-truth claim; not re-run here | NOT RE-VERIFIED |

---

## AuxPoW Framework Integration

| Item | Location | Status |
|------|----------|--------|
| `src/auxpow.h`, `src/auxpow.cpp` | present | PASS |
| `src/primitives/pureheader.h`, `src/primitives/pureheader.cpp` | present | PASS |
| Pureheader hashes via `Hashblake` | `primitives/pureheader.cpp:13` | PASS |
| AuxPoW-aware `CBlockHeader` serialization | `src/primitives/block.{h,cpp}` | PASS |
| Consensus params carry `nAuxpowChainId`, `nAuxpowStartHeight`, `fStrictChainId` | `consensus/params.h:66` + chainparams setters | PASS |
| `CheckAuxPowProofOfWork` called in contextual PoW path | `validation.cpp:3101` | PASS |
| `CheckAuxPowProofOfWork` called in header PoW paths | `validation.cpp:1104`, `1128` | PASS |
| Called on block-index reload from disk | `txdb.cpp:291-292` | PASS |
| Miner emits AuxPoW version bit above start height | `miner.cpp:137` | PASS |
| Pre-start AuxPoW blocks accepted (no blanket reject) | `pow.cpp:145-146` | PASS |
| Strict chain ID enforced on mainnet only | `chainparams.cpp:95` (true) vs `:196`, `:283` (false) | PASS |
| Mainnet start height preserved at legacy 160000 | `chainparams.cpp:101` | PASS |

AuxPoW verdict: **framework fully integrated with legacy-compatible pre-start tolerance**.

---

## RPC Surface

| RPC | Registration | Signature | Notes |
|-----|--------------|-----------|-------|
| `createauxblock` | `rpc/mining.cpp:1265` | `<address>` | Address-driven, builds child-chain template |
| `submitauxblock` | `rpc/mining.cpp:1266` | `<hash> <auxpow>` | Primary solved-block submission path |
| `getauxblock` | `rpc/mining.cpp:1264` | `( hash auxpow )` | Compatibility wrapper. Without args uses wallet keypool; reuses `AuxMiningCreateBlock` / `AuxMiningSubmitBlock` |
| `getworkaux` | absent | — | Intentionally out of scope, matches policy |

RPC verdict: **PASS** — matches "BlakeStream Seed And AuxPoW RPC Policy" section exactly.

Notable implementation detail: `submitauxblock` / `getauxblock` both call `AuxMiningSubmitBlock(...)` and, on null response, `KeepAuxMiningScript(hash)`, then return `response.isNull()`. This is consistent with the documented direction of `getauxblock` mapping onto the same block-template / block-submit flow rather than preserving a separate legacy path.

---

## Cross-reference against original 0.8.x

| Original parameter | Location | 0.15.2 match |
|--------------------|----------|--------------|
| `GetAuxPowStartBlock() = 160000` | `main.cpp:2162-2168` | `chainparams.cpp:101` |
| `GetOurChainID() = 0x000F` | `main.cpp:2170-2173` | `chainparams.cpp:96` |
| `nBlockSpacing = 10`, `nTargetTimespan = 600` | `main.cpp:1108-1109` | `chainparams.cpp:91-92` (600 timespan, 120 spacing) |
| `nMinHeightForFullReward = 1440` | `main.cpp:1113` | `validation.cpp:107` `LEGACY_REWARD_LAUNCH_HEIGHT` |
| `nCumulativeDiffMovingAverageNIntervals = 3` | `main.cpp:1112` | `validation.cpp:109` `LEGACY_REWARD_SAMPLE_COUNT` |
| `nBlockSpacing * 2 = 20` (move) | `main.cpp:1875, 4648` | `validation.cpp:108` `LEGACY_REWARD_SAMPLE_SPACING` |
| bork windows 3600 / 4000 | `main.cpp:1142-1147` | `validation.cpp:110-111` |
| `pchMessageStart = {0xfa,0xd3,0xe7,0xf4}` | `main.cpp:3298` | `chainparams.cpp:131-134` |
| `P2P port 24785 / testnet 18449` | `protocol.h:21` | `chainparams.cpp:135, 225` |
| `RPC port 5921 / 19738` | `bitcoinrpc.cpp:44` | `chainparamsbase.cpp:35, 47` |
| `PUBKEY_ADDRESS=130 SCRIPT_ADDRESS=7` | `base58.h:276-277` | `chainparams.cpp:146-147` |
| `PUBKEY_ADDRESS_TEST=142 SCRIPT_ADDRESS_TEST=170` | `base58.h:278-279` | `chainparams.cpp:236-237` |
| `MAX_MONEY = 10512000144 * COIN / 100` | `main.h:58` | `amount.h:26` |
| `COINBASE_MATURITY = 120` | `main.h:61` | `consensus/consensus.h:19` |
| Legacy BIP30 rule `fEnforceBIP30 = !pindex->phashBlock` | `main.cpp:1801` | `validation.cpp:1832` |
| Legacy pre-start AuxPoW tolerance (`auxpow != NULL` returns true) | `main.cpp:2203-2206` | `pow.cpp:145-146` |
| Genesis hash `00000059f24d9e85...` | `main.cpp:40` | `chainparams.cpp:139` |
| Checkpoints 76213/353493/468432/2509078 | `checkpoints.cpp:39-42` | `chainparams.cpp:162-167` |
| chainTxData 1630699667 / 2861736 / 2000/day | `checkpoints.cpp:58-60` | `chainparams.cpp:170-174` |

All originals cross-checked: **PASS**.

---

## Action Items

Blocking (release-blocking):

1. **Historical merged-mined header replay around legacy AuxPoW boundary.** Source-of-truth says replay previously reached height 5032 before rejecting `8491bafb7adade9c82eadddfa51edbe7490aa21280c17537c68ad50d861080c8`. The fix is in `pow.cpp:145-146`, but no fresh re-run was captured in this QC pass. Re-run the bootstrap / IBD replay on a non-production datadir and confirm the chain walks past 5032 and through height >= 160000 into the post-start AuxPoW window without rejects.
2. **Solved-block `submitauxblock` acceptance.** Source of truth still flags this as pending. Run isolated regtest / testnet QA: build template with `createauxblock`, externally solve an AuxPoW payload against a parent chain (or a mocked parent), submit with `submitauxblock`, confirm it connects, then repeat end-to-end via the `getauxblock` compatibility wrapper to prove both paths share the same acceptance logic.
3. **Bork-window (3600..3999) reward spot-check.** The port reimplements the legacy CBigNum target-space comparison using `arith_uint256`. This is mathematically equivalent but narrow-range. Do a targeted replay covering heights 3500..4100 to confirm the reward at each block matches the legacy chain's coinbase value exactly.

Non-blocking (cleanup / policy):

4. **DNS seed policy** — PASS. `chainparams.cpp:143-144` uses the 2 shared BlakeStream ecosystem seeds (`seed.blakestream.io`, `seed.blakecoin.org`). Per policy, all 6 coins use the SAME 2 seeds and they serve nodes for ALL coins; coin separation happens at the wire-protocol layer via `pchMessageStart` (0xfa 0xd3 0xe7 0xf4) and port (24785). Matches the Blakecoin 0.15.2 reference repo exactly. No action needed.
5. **Remove the "Blakecoin Genesis Block" docstring** at `chainparams.cpp:45-51` — it's a stale copy-paste from the donor and is confusing now that genesis values below it are UMO.
6. **`CMainParams` comment `// BEGIN BLAKECOIN: Message start bytes (same as Bitcoin mainnet)` at `chainparams.cpp:130` is wrong.** These bytes are UMO-specific (`0xfa 0xd3 0xe7 0xf4`) and the "same as Bitcoin mainnet" note is false. Fix the comment.
7. **Regtest params inherit Blakecoin `nMinerConfirmationWindow = 144`** (`chainparams.cpp:287`) but mainnet/testnet use 5. Intentional per donor, but worth a one-line comment so the asymmetry does not look like a mistake.
8. **`src/blakecoind` and `src/blakecoin-cli` artifacts** are committed/present under the Blakecoin binary name. The autotools rename via `configure.ac` `BITCOIN_DAEMON_NAME` should emit `universalmoleculed` / `universalmolecule-cli` — re-run the full build via `build.sh` and confirm the final artifacts land under the UMO names. The prebuilt `blakecoind` / `blakecoin-cli` files in `src/` are leftover objects from an earlier build, not release artifacts.
9. **Source-of-truth live-replay claims (regtest smoke, AuxPoW chainid=15, IBD past 5032) were not re-run in this QC pass.** These are marked NOT RE-VERIFIED in the tables above. Either re-run them and capture logs in the source of truth, or leave them as historical claims and document the last verification date.

Overall verdict: **Consensus surface PASS.** All coin identity, chain params, reward formula (including bork quirk), AuxPoW framework, RPC surface, BIP30 relaxation, and wire checksum checks match the source of truth. The remaining blockers are runtime QA (historical replay past 5032, live `submitauxblock`, bork-window reward spot-check), plus a handful of cosmetic / seed-policy cleanups.
