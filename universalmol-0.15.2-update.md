# UniversalMolecule 0.15.2 Update — Source of Truth

## Overview

Port UniversalMolecule (UMO) from its current 0.8.15.5 codebase to Bitcoin Core 0.15.2, following the same approach used for the Blakecoin 0.15.2 update (`Blakecoin-0.15.2-update`).

**Reference codebase:** `../Blakecoin-0.15.2-update/` — the completed Blakecoin port to 0.15.2
**Original codebase:** `../universalmol/` — current 0.8.15.5 source with all coin-specific parameters

---

## QC Status

- This file is not implementation-safe without the QC corrections in this section.
- The current 0.15.2 tree builds and passes `test/functional/wallet-basic-smoke.py` on regtest with no sends, no funding, and no mining.
- Corrected from source: secret-key prefixes are `128` mainnet and `239` testnet, not `TBD`.
- Corrected from source: `../universalmol/src/bitcoinrpc.cpp` sets RPC ports to `5921` mainnet and `19738` testnet. Older docs and scripts disagree, so treat source code as canonical until we make an explicit port decision.
- Corrected from source: the reward summary below was too loose. Legacy code pays `1 UMO` at genesis, `0.001 UMO` through height `1440`, then `0.1 UMO` when difficulty is rising and `2 UMO` otherwise, with the legacy `baseHeight < 3600` / `3600..3999` behavior now represented in `src/validation.cpp`.
- Corrected from source: AuxPow is height-gated at legacy value `160000` with chain ID `0x000F`, and legacy pre-start behavior has a quirk that must be preserved or explicitly fixed during the port.
- Corrected in this tree: mainnet checkpoints and chain transaction metadata are now sourced from the original UniversalMolecule repo, and the donor Blakecoin seed defaults have been removed because UMO did not ship a historical DNS seed list.
- The shared AuxPow framework is now integrated: `src/auxpow.{h,cpp}`, `src/primitives/pureheader.{h,cpp}`, AuxPow-aware block/header serialization, disk index persistence, AuxPow-aware PoW validation, and chain-ID-aware block template versions.
- Live daemon QA correction: current mainnet bootstrap and peer replay reaches height `5032` and then rejects header `8491bafb7adade9c82eadddfa51edbe7490aa21280c17537c68ad50d861080c8` as `early-auxpow-block` if the port uses a modern pre-start header reject. The 0.15.2 port therefore keeps the legacy nominal AuxPow start height `160000`, but restores the 0.8.x compatibility rule that tolerates AuxPoW-bearing blocks before that height during sync.
- Testnet and regtest use chain ID `0x000F` with start height `0` and strict chain ID disabled for local QA.
- Verified after the fan-out: `make -C src -j4 blakecoind blakecoin-cli` succeeds and `test/functional/test_runner.py --jobs=1 wallet-basic-smoke.py` passes on regtest.
- Historical validation compatibility is now preserved: the legacy UniversalMolecule 0.8 tree only enforced the BIP30 overwrite rule in `!pindex->phashBlock` contexts, so the 0.15.2 port must not silently restore Bitcoin Core's broader duplicate-txid rejection path.
- Merged-mining RPC direction is now fixed for this port: primary RPCs are `createauxblock <address>` plus `submitauxblock <hash> <auxpow>`, with `getauxblock` kept only as a compatibility wrapper for older pool software. `getworkaux` is intentionally out of scope unless a real dependency is later proven.
- Keep this repo on a strict no-send / no-mine mainnet rule until the final production pool and Electrium carry-back staging are complete.

### BlakeStream Seed And AuxPoW RPC Policy

- BlakeStream DNS seeds (`seed.blakestream.io`, `seed.blakecoin.org`) are shared across all six coins and serve nodes for ALL coins. A single seed lookup returns peer IPs regardless of which coin is asking; coin separation happens at the wire-protocol layer via each coin's unique `pchMessageStart` and default port.
- The production direction for this repo is a modern merged-mining RPC surface: `createauxblock` to build the child-chain template and `submitauxblock` to submit the solved AuxPoW payload.
- `createauxblock` is address-driven on purpose so a pool can choose the child-chain payout script explicitly instead of depending on wallet mining state inside the daemon.
- `getauxblock` remains only as a compatibility mode for older merged-mining software. It should map onto the same block-template / block-submit flow rather than preserving a separate legacy implementation path.
- `getworkaux` is not part of the planned 0.15.2 target. We are not reviving `getwork`-era RPC unless a live pool or deployment proves it is still required.
- The same 2 DNS seeds (`seed.blakestream.io`, `seed.blakecoin.org`) are used by all six coins. This matches the Blakecoin 0.15.2 reference repo exactly.

### Wire Checksum Policy

- UniversalMolecule should preserve the legacy `Hashblake` P2P message checksum behavior for current network interoperability.
- Do not normalize UniversalMolecule to Blakecoin's temporary non-`Hashblake` handshake exception.
- Keep Blakecoin documented as the one current exception; UniversalMolecule stays on `Hashblake` before go-live unless a fresh compatibility review says otherwise.

## AuxPoW Start And Completed Work

| Network | Chain ID | Nominal AuxPoW Start | Observed Pre-Start AuxPoW Evidence In Current QA | Exact Time/Date Status | 0.15.2 Port Rule |
|---------|----------|----------------------|-----------------------------------------------|------------------------|------------------|
| Mainnet | `0x000F` | `160000` | Replay reached height `5032` before hitting the old `early-auxpow-block` reject | Exact timestamp remains archival-only; compatibility rule is already proven by replay | Keep `160000` as the nominal legacy value, but tolerate earlier historical AuxPoW-bearing blocks during bootstrap / IBD |
| Testnet | `0x000F` | `0` | N/A | Local QA only | AuxPoW enabled for local QA; strict chain ID disabled |
| Regtest | `0x000F` | `0` | N/A | Local QA only | AuxPoW enabled for local QA; strict chain ID disabled |

Interpretation note:
`160000` remains the nominal legacy mainnet AuxPoW start in `chainparams.cpp`. The recent fix did not redefine mainnet activation to `5032`. It preserved the nominal boundary while removing the incorrect blanket reject so historical pre-start AuxPoW-bearing chain data can still sync.

- Completed in this repo:
  - Integrated the shared AuxPoW framework with `src/auxpow.{h,cpp}` and `src/primitives/pureheader.{h,cpp}`.
  - Ported AuxPoW-aware block/header serialization, disk index persistence, block version handling, and PoW validation.
  - Kept UniversalMolecule's nominal mainnet start height at `160000` while removing the modern `early-auxpow-block` reject that broke historical chain acceptance around height `5032`.
  - Corrected mainnet checkpoints and chain-transaction metadata from the original UniversalMolecule tree and removed Blakecoin donor seed defaults.
  - Implemented the modern merged-mining RPC direction: `createauxblock <address>` and `submitauxblock <hash> <auxpow>`, with `getauxblock` retained only as a compatibility wrapper.
  - Verified no-send regtest wallet smoke coverage and refreshed Ubuntu 24 daemon/Qt builds after the compatibility correction.
- Operational rule:
  - Keep the strict no-send / no-mine mainnet rule in place while final production pool and Electrium carry-back staging continue.

---

## Coin Identity

| Parameter | Value |
|-----------|-------|
| Coin Name | UniversalMolecule |
| Ticker | UMO |
| Algorithm | Blake-256 (8 rounds) |
| Merge Mining | Yes (AuxPow; legacy nominal start 160000 with pre-start compatibility preserved) |
| Base Version (current) | 0.8.15.5 (forked from Bitcoin 0.8.5) |
| Target Version | 0.15.2 |

---

## Chain Parameters to Preserve

### Network

| Parameter | Mainnet | Testnet |
|-----------|---------|---------|
| P2P Port | 24785 | 18449 |
| RPC Port | 5921 (source code) | 19738 (source code) |
| pchMessageStart | 0xfa, 0xd3, 0xe7, 0xf4 | 0x0b, 0x11, 0x39, 0x38 |

### Address Prefixes

| Type | Mainnet | Testnet |
|------|---------|---------|
| Pubkey Address | 130 (0x82) | 142 (0x8E) |
| Script Address | 7 (0x07) | 170 (0xAA) |
| Secret Key | 128 (0x80) | 239 (0xEF) |
| Bech32 HRP | `umo` | `tumo` |

### Block Parameters

| Parameter | Value |
|-----------|-------|
| Block Time | 120 seconds (2 minutes) |
| Target Timespan | 600 seconds (10 minutes) |
| Retarget Interval | 5 blocks (600/120) |
| Block Spacing Factor (nBlockSpacing) | 10 |
| Coinbase Maturity | 120 blocks |
| Max Supply | 105,120,001.44 UMO |
| COIN | 100,000,000 satoshis |
| PoW Limit | ~uint256(0) >> 24 |

### Block Reward (CRITICAL — Difficulty-Responsive)

UniversalMolecule uses a **unique difficulty-responsive reward system**:

| Condition | Reward |
|-----------|--------|
| First 1,440 blocks (~2 days) | 0.001 UMO (minimal launch phase) |
| Difficulty stable or falling | 2 UMO per block |
| Difficulty rising | 0.1 UMO per block |

Key parameters:
- `nMinHeightForFullReward = 1440` — blocks before full rewards activate
- `nCumulativeDiffMovingAverageNIntervals = 3` — 3-hour moving average for difficulty comparison
- Max difficulty increase: 3% per 10 blocks

**This is the most unusual reward system of all 5 coins, and it is now represented in `src/validation.cpp`.** The former production pool / Electrium carry-back staging blocker is now green; the remaining no-send / no-mine rule is rollout discipline only.

### Dust Limits (Non-Standard)

| Parameter | Value |
|-----------|-------|
| Dust Soft Limit | 1,000 satoshis (0.00001 UMO) |
| Dust Hard Limit | 1,000 satoshis (0.00001 UMO) |

Note: Both soft and hard dust limits are the same (1000 sat), which is lower than most coins.

### Genesis Block

| Parameter | Value |
|-----------|-------|
| Hash | `0x00000059f24d9e85501bd3873fac0cd6e8a43fd8c20eee856082dbdcc09a8e64` |
| Merkle Root | `0x11ad2754baede90db86d491ada7030551bd3f0d72a7486ef57fe8fcf44c3b6b4` |
| nTime | 1405307607 (July 14, 2014) |
| nNonce | 79,480,397 |
| nVersion | 112 |
| nBits | bnProofOfWorkLimit.GetCompact() |

### DNS Seeds

- **None** — No DNS seeds configured
- Hardcoded seed nodes: blakecoin.org, eu3.blakecoin.com, la1.blakecoin.com, at1.blakecoin.com

### AuxPow Configuration

| Parameter | Value |
|-----------|-------|
| AuxPow Start Block (mainnet) | 160,000 |
| AuxPow Start Block (testnet) | 0 (always enabled) |

**Important:** `160000` remains the nominal legacy mainnet AuxPoW start height, but the 0.15.2 port must not reintroduce a blanket `early-auxpow-block` reject. Historical bootstrap data already contains earlier AuxPoW-bearing blocks, so the implemented rule is to preserve the nominal boundary while tolerating that pre-start history during sync.

### Signed Message Header

"UniversalMolecule Signed Message:\n"

---

## What Needs to Be Done

### Phase 1: Copy & Rebrand the Blakecoin 0.15.2 Base

1. **Copy** the entire `Blakecoin-0.15.2-update` codebase into this directory
2. **Rename** all Blakecoin references → UniversalMolecule:
   - Binary names: `universalmoleculed`, `universalmolecule-qt`, `universalmolecule-cli`, `universalmolecule-tx`
   - Config file: `universalmolecule.conf`, config dir `~/.universalmolecule/`
   - URI scheme: `universalmolecule://`
   - Desktop entry, icons, window titles
   - `configure.ac`: package name, version
   - Window title: "UniversalMolecule - Wallet"
   - Signed message header: "UniversalMolecule Signed Message:\n"

### Phase 2: Apply Coin-Specific Parameters

3. **`src/chainparams.cpp`** — Replace ALL chain parameters:
   - Genesis block (hash, merkle root, nTime=1405307607, nNonce=79480397)
   - Network ports (P2P: 24785, RPC: 5921 mainnet / 19738 testnet)
   - Message start bytes (0xfa, 0xd3, 0xe7, 0xf4)
   - Address prefixes (pubkey: 130, script: 7)
   - Block timing (120s block time, 10-min retarget, 5-block interval)
   - **Disable halving interval** (reward is difficulty-responsive)
   - AuxPow start height: 160,000 (mainnet), 0 (testnet)
   - Bech32 HRP
   - Fixed seed nodes (blakecoin.org, etc.)

4. **`src/amount.h`** — MAX_MONEY = 10,512,000,144 (in COIN units: `10512000144 * COIN / 100`)
   - Note: This is approximately 105,120,001.44 UMO. The unusual precision (0.44) suggests this was calculated from emission schedule. Verify the exact value from `main.h`.

5. **`src/validation.cpp`** — Block reward logic (**CUSTOM — DIFFICULTY-RESPONSIVE**):
   ```
   if (height < 1440) return 100000;  // 0.001 UMO
   
   // Calculate cumulative difficulty moving average
   // Compare current difficulty window to 3-hour moving average
   if (difficulty_is_rising) {
       return 10000000;  // 0.1 UMO
   } else {
       return 200000000;  // 2 UMO
   }
   ```
   - The exact difficulty comparison logic has been ported from `../universalmol/src/main.cpp`
   - This requires access to previous block headers for the moving average calculation, so the 0.15.2 subsidy path now takes the previous block index as context
   - The legacy `baseHeight < 3600` / `3600..3999` behavior is preserved for consensus compatibility

6. **`src/consensus/consensus.h`** — COINBASE_MATURITY = 120

7. **`src/qt/`** — Update all GUI branding for UniversalMolecule

### Phase 3: AuxPow / Merge Mining

8. **AuxPow integration** — Height-gated:
   - Blocks < 160,000: standard PoW only
   - Blocks >= 160,000: AuxPow accepted
   - The shared 0.15.2 AuxPow framework is now integrated with chain ID `0x000F` and the height gate preserved in validation
   - The height gate adds complexity vs coins with AuxPow from genesis
   - The modern merged-mining RPC path is now implemented and no-send regtest-smoke verified for `createauxblock <address>` plus compatibility `getauxblock`
   - Historical merged-mined header compatibility is now documented from preserved replay evidence; exact activation dating remains archival-only and is no longer the release blocker

### Phase 4: Build System

9. **`build.sh`** — Update all variables:
    - COIN_NAME: "universalmolecule"
    - DAEMON_NAME: "universalmoleculed"
    - QT_NAME: "universalmolecule-qt"
    - CLI_NAME: "universalmolecule-cli"
    - TX_NAME: "universalmolecule-tx"
    - VERSION: "0.15.2"
    - RPC_PORT: 5921
    - P2P_PORT: 24785

10. **Docker configs** — Same Docker images as Blakecoin 0.15.2

### Phase 5: SegWit Activation

11. **Mainnet SegWit rollout** — Mainnet versionbits signaling starts on May 11, 2026 00:00:00 UTC (`1778457600`) and times out on May 11, 2027 00:00:00 UTC (`1809993600`).
12. **Activation semantics** — May 11, 2026 is the signaling start date, not guaranteed same-day activation. Actual mainnet SegWit enforcement still depends on miner signaling and BIP9 lock-in.
13. **CSV / test networks** — CSV stays `ALWAYS_ACTIVE`, and testnet/regtest keep `ALWAYS_ACTIVE` SegWit for controlled QA and wallet validation.
14. **BIP34/65/66** — Disable version checks

---

## Key Differences from Blakecoin

| Aspect | Blakecoin | UniversalMolecule |
|--------|-----------|-------------------|
| Block Time | 180s (3 min) | 120s (2 min) |
| Retarget Interval | 20 blocks | 5 blocks |
| Retarget Timespan | 1 hour | 10 minutes |
| Reward Model | Dynamic (25 + sqrt) | Difficulty-responsive (0.1 or 2 UMO) |
| Max Supply | 21M | ~105M |
| Coinbase Maturity | ??? | 120 blocks |
| P2P Port | 8773 | 24785 |
| RPC Port | 8772 | 5921 mainnet / 19738 testnet |
| Pubkey Address | 26 | 130 |
| Merge Mining | No (in donor Blakecoin 0.15.2) | Yes (AuxPow from block 160,000) |
| Genesis Date | Oct 2013 | July 2014 |

---

## Potential Issues & Gotchas

1. **Difficulty-responsive rewards** — The most complex reward system. Requires reading chain state (previous block difficulties) to determine current reward. Must understand exactly how the 3-hour moving average is computed and what "difficulty rising" means in practice.
2. **AuxPow height gate** — Unlike other coins where AuxPow is always on or always off, UMO enables it at block 160,000. The validation code must handle both modes.
3. **Fractional MAX_MONEY** — 105,120,001.44 UMO is an unusual max supply with decimal places. In satoshis: 10,512,000,144,000,000. Verify this fits in int64_t (it does: ~1.05e16 << 9.2e18).
4. **Very short retarget window** — 5 blocks / 10 minutes is extremely aggressive difficulty adjustment. Combined with the difficulty-responsive reward, this creates complex economic dynamics.
5. **Long binary names** — `universalmoleculed` is 18 characters. Ensure no path length issues on Windows.
6. **Blakecoin seed nodes** — UMO uses blakecoin.org infrastructure for seed nodes. This may or may not still work for UMO network discovery.
7. **0.001 UMO launch phase** — First 1,440 blocks had near-zero rewards. This was likely an anti-instamine measure. Verify the exact value (100,000 satoshis = 0.001 UMO).
8. **Historical BIP30 rule** — The original UniversalMolecule chain relaxed BIP30 during normal block connection (`!pindex->phashBlock` only). Keep that legacy behavior unless a full historical replay proves the broader 0.15.2 rule is safe.

---

## Build & Test Plan

1. Build native Linux first
2. Verify genesis block hash matches
3. Test address generation (prefix 130)
4. Study and port the difficulty-responsive reward logic carefully
5. Verify reward at: height `0`, heights `1-1440`, stable difficulty (`2 UMO`), and rising difficulty (`0.1 UMO`)
6. Preserve the archived historical replay notes and do not reintroduce the rejected `early-auxpow-block` rule before any mainnet activity
7. Test RPC defaults on ports `5921` mainnet and `19738` testnet
8. Build AppImage, Windows, and macOS artifacts after consensus validation is stable

---

## Verified Snapshot

- Native Linux rebuild succeeded for `blakecoind` and `blakecoin-cli`.
- Fresh regtest no-send smoke passed for `getnewaddress`, `createauxblock <address>`, and compatibility `getauxblock`.
- Verified AuxPow template `chainid` returned as `15` on fresh regtest, matching `consensus.nAuxpowChainId`.
- Verified from `../universalmol/src/main.cpp`: legacy UniversalMolecule keeps BIP30 relaxed with `bool fEnforceBIP30 = !pindex->phashBlock;`, and the 0.15.2 port now preserves that historical validation behavior.
- Direct `createauxblock` plus `submitauxblock` acceptance is now proven in isolated QA. The production carry-back staging that used to block release is now green.

---

## File Reference

| What | Where |
|------|-------|
| Reference (completed) | `../Blakecoin-0.15.2-update/` |
| Original coin source | `../universalmol/` |
| Original params | `../universalmol/src/main.cpp`, `../universalmol/src/main.h` |
| Original build script | `../universalmol/build.sh` |
| Qt project file | `../universalmol/universalmolecule-qt.pro` |
| AuxPow code | `../universalmol/src/main.cpp` |
| Seed nodes | `../universalmol/src/net.cpp` |
| Reward logic | `../universalmol/src/main.cpp` (GetBlockValue) |

## SegWit Activation Test

- Functional test: `test/functional/segwit-activation-smoke.py`
- Build-server wrapper: `/home/sid/Blakestream-Installer/qa/runtime/run-segwit-activation-suite.sh`
- Direct command used by the wrapper:

```bash
BITCOIND=/path/to/universalmoleculed BITCOINCLI=/path/to/universalmolecule-cli \
python3 ./test/functional/segwit-activation-smoke.py \
  --srcdir="$(pwd)/src" \
  --tmpdir="<artifact_root>/universalmolecule/<timestamp>/tmpdir" \
  --nocleanup \
  --loglevel=DEBUG \
  --tracerpc
```

- Expected regtest Bech32 prefix: `rumo1`
- Review artifacts:
  `summary.json`, `state-defined.json`, `state-started.json`, `state-locked_in.json`, `state-active.json`, `address-sanity.json`, `combined-regtest.log`, `tmpdir/test_framework.log`, `tmpdir/node*/regtest/debug.log`
- Successful all-six build-server run:
  `/home/sid/Blakestream-Installer/outputs/segwit-activation/20260412T083423Z/run-summary.md`
- Coin artifact directory:
  `/home/sid/Blakestream-Installer/outputs/segwit-activation/20260412T083423Z/universalmolecule`
- Harness note:
  the final witness proposal builder now takes the coinbase amount directly from `getblocktemplate()["coinbasevalue"]`, which was required for UniversalMolecule's low regtest subsidy to pass the post-activation witness proposal check.
- Safety rule:
  regtest only for activation validation; do not mine or send transactions on mainnet while rollout QA is still in progress.

## AuxPoW Testnet Merged-Mining Verification

- Final successful container-built run:
  `/home/sid/Blakestream-Installer/outputs/auxpow-testnet/20260413T003341Z/run-summary.md`
- Wrapper command:
  `bash /home/sid/Blakestream-Installer/qa/auxpow-testnet/run-auxpow-testnet-suite.sh`
- Parent chain:
  Blakecoin testnet only, fully isolated from public peers.
- Live proof result:
  UniversalMolecule accepted `2` merged-mined child blocks in the 4-child batch and `1` in the 5-child full run.
- Direct RPC cross-check:
  `createauxblock` plus `submitauxblock` accepted on a fresh UniversalMolecule testnet pair. Artifact:
  `/home/sid/Blakestream-Installer/outputs/auxpow-testnet/20260413T003341Z/universalmolecule/rpc-crosscheck.json`
- QC note:
  the live proxy now resolves a collision-free aux nonce and merkle size automatically, which prevented UniversalMolecule's chain ID from being forced into an overlapping slot during the multi-child proofs.
- Safety rule:
  testnet only for merged-mining QA; do not mine or send transactions on mainnet while AuxPoW rollout validation is still in progress.

## Devnet/Testnet Validation Outcomes

- SegWit activation validation passed on isolated regtest. See:
  `/home/sid/Blakestream-Installer/outputs/segwit-activation/20260412T083423Z/universalmolecule`
- AuxPoW merged-mining validation passed on isolated testnet, including direct `createauxblock` plus `submitauxblock` acceptance. See:
  `/home/sid/Blakestream-Installer/outputs/auxpow-testnet/20260413T003341Z/universalmolecule`
- Mainnet carry-back audit for the devnet copy lives in:
  `mainnet-carryback-audit-2026-04-18.md`
- Audit result:
  the diff between this repo and the devnet `coins/universalmol` copy stayed limited to devnet `chainparams*` and build cleanup. No new UniversalMolecule mainnet wallet, consensus, or RPC carry-back was identified from the devnet copy itself.

## Mainnet Carry-Back Decisions

- SegWit rollout remains scheduled, not forced active.
- Mainnet AuxPoW start height remains `160000` as the chain source of truth.
- Do not port devnet network identity, datadir, test shortcuts, or activation shortcuts back into this repo.
- Pool/runtime carry-back work is tracked in the mainnet Eloipool repo.
- Electrium sync and signing carry-back work is tracked in the Electrium repo.
- Mainnet pool integration now depends on the proven multi-miner aux-child payout path in Eloipool, not the old single active mining-key QA shortcut.

## Staging Hygiene

- Keep the intentional autotools and build-system layer in staging for this repo:
  `Makefile.am`, `Makefile.in`, `aclocal.m4`, `autogen.sh`, `configure*`, `build-aux/*`, and `depends/*`.
- Trim generated build junk before review or promotion:
  `.libs/`, `.deps/`, `autom4te.cache/`, `*.o`, `*.lo`, `*.la`, `config.log`, `config.status`, and similar transient outputs.
- April 19, 2026 staging pass explicitly removed staged libtool and univalue build artifacts while preserving the intentional autotools carry-back set.

## Not Carried Back From Devnet

- `src/chainparams.cpp`, `src/chainparamsbase.cpp`, `src/chainparamsbase.h`
- Any private-testnet `BIP65Height = 1`, `ALWAYS_ACTIVE`, devnet ports, message starts, datadirs, or local-only harness shortcuts
- Pool UI, merged-mine proxy, Electrium, ElectrumX, and builder/runtime scripts

## Pool / Electrium Dependencies

- Mainnet merged-mining now depends on the modern `createauxblock` plus `submitauxblock` direction and the proven multi-miner aux payout model in Eloipool.
- Electrium compatibility now depends on full AuxPoW header support and Blake-family single-SHA signing compatibility.
- Per-coin overlays and branding stay in the Electrium repo and are not folded back into this C++ core tree.

## Safety Rule

- Do not mine on mainnet while carry-back staging is in progress.
- Do not send transactions on mainnet while carry-back staging is in progress.
- Use isolated regtest, testnet, or staging environments until rollout QA is complete.

## April 18, 2026 Devnet Validation Snapshot

- Shared BlakeStream devnet run `20260418T195508Z` proved concurrent multi-miner AuxPoW against the live pool with two mining keys active in the same session.
- Live pooled merged-mined UniversalMolecule child block proof is green:
  height `433` accepted with `tx_count = 2`.
- This means the live pool/proxy path is no longer limited to coinbase-only UniversalMolecule child blocks once mempool transactions are present.

## Mainnet Carry-Back Snapshot

- Keep UniversalMolecule chain identity, Hashblake requirement, AuxPoW start rules, and scheduled SegWit rollout exactly as already documented in this repo.
- Promote only the proven external dependencies:
  mainnet pool multi-miner mining-key payout plumbing and Electrium full AuxPoW-header plus single-SHA signing compatibility.
- Do not carry back any devnet ports, datadirs, private-testnet activation shortcuts, or runtime wrapper behavior into mainnet chain params.

## April 19, 2026 Broader Electrium Staging Closure

- Broader staged packaged-client proof is now green at:
  `/home/sid/Blakestream-Devnet/outputs/electrium-staging/20260419T053030Z/run-summary.md`
- UniversalMolecule's packaged Electrium client connected successfully against
  the staged local ElectrumX backend on `127.0.0.1:56001`.
- This run also exposed and closed a shared aux-core startup bug in
  `src/validation.cpp`: height-less disk rereads could treat genesis-like
  headers as regular AuxPoW blocks and fail with a false
  `non-AUX proof of work failed` reject.
- The fix now treats
  `block.GetHash() == consensusParams.hashGenesisBlock || block.hashPrevBlock.IsNull()`
  as genesis-like in the disk-reread path, which keeps standalone staged
  backends honest without relaxing real chained AuxPoW validation.
