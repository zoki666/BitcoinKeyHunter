# BLOCKCHAIR ADDRESSES FILTER
[🇺🇸 English] | [🇪🇸 Español](README.es.md)

Python script that generates a list of valid addresses for Bitcoin Key Hunter from a Blockchair dump. It is compatible with any source file that respects the column format (address and balance).

## How to obtain the target files

1. **Get the Dump: Download** 'blockchair_bitcoin_addresses_latest.tsv.gz' from '[https://blockchair.com/dumps](https://blockchair.com/dumps)' in the 'Bitcoin/Addresses' section.
2. **Location**: Place the file in the same path as the 'blockchair_addresses_filter.py' script.
3. **Execution**: Run the script with: `python blockchair_addresses_filter.py`
4. **Results**: Two files will be generated in the same folder:
   - objetivos.txt: Readable paths (addresses as they are).
   - objetivos_identidad.txt: Binary IDs (40 hex for Legacy/P2SH/SegWit and 64 hex for Taproot). This is the one used by the Hunter in C++.

## Can I create a test file?

Yes, you can create a small test environment by following these steps:

1. Create a text file named `blockchair_bitcoin_addresses_latest.tsv`.
2. The file is plain text, with a header and data lines. It consists of 2 fields (address and balance) separated by a **TAB**.
3. **Default Filters**:
   - Minimum Balance: 1,000 Satoshis (0.00001 BTC).
   - Formats: 1... (Legacy), 3... (P2SH), bc1q... (SegWit) or bc1p... (Taproot).
4. **Compress**: The script requires the Gzip (.gz) format. Command:
   `gzip -c blockchair_bitcoin_addresses_latest.tsv > blockchair_bitcoin_addresses_latest.tsv.gz`

### Example of a correctly formatted file (Fields with TAB):

```text
address balance
1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa  5000067342
1HLoD9E4SDaeRGYmScyGn2cg2QHWyjiNe8  500
3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy  2500000
3EbS1Y38AmGZS7Fv2GgJvP1iGZ7S9oQ9Y1  9900
bc1qxy2kgdygjrsqtzq2n0yrf2493p83kkfjhx0wlh  120000
bc1qrp33g0q5c5cyas34c98f983kyf3zfsse0dk30y  8500
bc1p5d7rjq7ndv493p83kkfjhx0wlh5d7rjq7ndv49  300000
bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4  1500
bc1qgdjqv083q3Yxy2kgdygjrsqtzq2n0yrf2493p83 50000
4A8mAK9SrCA3L2e7XuLxk8iWWfvNhqFVbTVz3q8zb1  1000000
```

#### Example Analysis:

```text
Address,Type,Balance (Sats),Accepted?,Reason
1A1z...,Legacy,'5,000,067,342',YES,Correct format and high balance.
1HLo...,Legacy,500,NO,'Balance lower than 10,000 satoshis.'
3J98...,P2SH,'2,500,000',YES,Compatible format and high balance.
3EbS...,P2SH,'9,900',NO,'Below the 10,000 sats limit.'
bc1q... (short),SegWit,'120,000',YES,P2WPKH (Single Sig) detected.
bc1q... (short),SegWit,'8,500',NO,Insufficient balance.
bc1p...,Taproot,'300,000',YES,v6.4 priority target.
bc1p...,Taproot,'1,500',NO,Insufficient balance.
bc1q... (long),P2WSH,'50,000',NO,Ignored by script v3.0 (Usually multi-sigs).
4A8m...,Monero/Others,'1,000,000',NO,'Does not start with 1, 3 or bc1.'
```

### Generated Files:

- **objetivos.txt**: Addresses in text format for visual verification.
- **objetivos_identidad.txt**: Contains the processed identities (Hash160 for ECDSA and X-Only Pubkey for Schnorr/Taproot). This file is the core of the massive search in the Bitcoin Key Hunter C++ program.

# SUPPORT THE PROJECT

If this project has been useful to you and you want to support the development of **Bitcoin Key Hunter**, you can send a tip to the following addresses:

| Currency / Network | Address |
| :--- | :--- |
| **Bitcoin (BTC)** | `bc1qthc87aykdj2tjrxlqe7k9emz3txphrprm0scu6` |
| **Bitcoin (BTC) Lightning** | `ffg@cake.cash` |
| **Monero (XMR)** | `4A8mAK9SrCA3L2e7XuLxk8iWWfvNhqFVbTVz3q8zb1K6RisgTJtExWM3QPCzzheWz4GhcnMvCUKKfFoY869jPWBw1JxJnmj` |
| **ERC20/BEP20 (Ethereum / Polygon / BNB...)** | `0xb3F62CEB2706c9E7a27c2Aec6E3e1bA422f8097e` |
| **TRON (TRX) / USDT (TRC20)** | `TTmzFL2NZsD3yjHKCZg5Y7oBT8QXsJ8E6h` |
| **SOLANA (SOL)** | `2UGeSWJV6DaQEYS7fDtHW61EWQSoRd85JTYxHPrBjWaC` |
| **DOGECOIN (DOGE)** | `D85kgN3Q2CaxcjdANaYmeywcTsgBAGrfPJ` |
| **CARDANO (ADA)** | `addr1qxjl2vked39stz8jrgh5fj30gr8p5vh35nrl8g7asr0knw7s7j9gttx0xkq3zpjn220fql9yvzxw0agk0afnvg6fj75q7sqt95` |
| **SUI** | `0x55f06f745b058e0ab48300ff12bd08ac700fc2b23409fa942d81c5bce18864b7` |
| **BITCOIN CASH (BCH)** | `qqv9vptfw6vmcr75a35at33cy554uh52eun5t8pphu` |
| **ZCASH (ZEC)** | `u1wcufckcn8gfts4axlj9y5tjwp8ndy6l3mnfqapg4sszkdrj7adsjf0j8avzaestrkht2myrhn2l9m6q8nq5tqww884xavxm809rjk66lmtlrsfzygt78q7jp9024hjczjda5wvxsy7gc8l4fsnd078ac7pzrmrp9nqq8njejuu2h4h62` |

> [!TIP]
> For donations on the Ethereum network, you can send **ETH**, **POL (Polygon)**, **BNB**, or any **ERC20/BEP20** token to the same `0x` address. Thank you for your support!