# BLOCKCHAIR ADDRESSES FILTER

Script en Python que genera una lista de direcciones validas para Bitcoin Key Hunter a partir de un dump de Blockchair. Es compatible con cualquier fichero origen que respete el formato de columnas (address y balance).

## Como obtener los ficheros de objetivos

1. **Conseguir el Dump**: Descarga 'blockchair_bitcoin_addresses_latest.tsv.gz' desde '[https://blockchair.com/dumps](https://blockchair.com/dumps)' en la seccion 'Bitcoin/Addresses'.
2. **Ubicacion**: Deja el fichero en la misma ruta que el script 'blockchair_addresses_filter.py'.
3. **Ejecucion**: Ejecuta el script con: `python blockchair_addresses_filter.py`
4. **Resultados**: Se generaran dos ficheros en la misma carpeta:
   - **objetivos.txt**: Rutas legibles (direcciones tal cual).
   - **objetivos_identidad.txt**: IDs binarios (40 hex para Legacy/P2SH/SegWit y 64 hex para Taproot). Este es el que usa el Hunter en C++.

## ¿Puedo crearme un fichero de prueba?

Si, puedes crear un entorno de test pequeño siguiendo estos pasos:

1. Crear un fichero de texto llamado `blockchair_bitcoin_addresses_latest.tsv`.
2. El fichero es texto plano, con una cabecera y lineas de datos. Son 2 campos (address y balance) separados por un **TABULADOR**.
3. **Filtros por defecto**:
   - Balance minimo: 1,000 Satoshis (0.00001 BTC).
   - Formatos: 1... (Legacy), 3... (P2SH), bc1q... (SegWit) o bc1p... (Taproot).
4. **Comprimir**: El script requiere el formato Gzip (.gz). Comando:
   `gzip -c blockchair_bitcoin_addresses_latest.tsv > blockchair_bitcoin_addresses_latest.tsv.gz`

### Ejemplo de fichero correctamente formateado (Campos con TAB):

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

#### Analisis del ejemplo:

```text
Direccion,Tipo,Balance (Sats),¿Se acepta?,Motivo
1A1z...,Legacy,"5,000,067,342",SI,Formato correcto y balance alto.
1HLo...,Legacy,500,NO,"Balance inferior a 10,000 satoshis."
3J98...,P2SH,"2,500,000",SI,Formato compatible y balance alto.
3EbS...,P2SH,"9,900",NO,"Por debajo del limite de 10,000 sats."
bc1q... (corto),SegWit,"120,000",SI,P2WPKH (Single Sig) detectado.
bc1q... (corto),SegWit,"8,500",NO,Balance insuficiente.
bc1p...,Taproot,"300,000",SI,Objetivo prioritario v6.4.
bc1p...,Taproot,"1,500",NO,Balance insuficiente.
bc1q... (largo),P2WSH,"50,000",NO,Ignorado por script v3.0 (Suelen ser multifirmas).
4A8m...,Monero/Otros,"1,000,000",NO,"No empieza por 1, 3 o bc1."
```

### Ficheros generados:

- **objetivos.txt**: Direcciones en formato texto para verificacion visual.
- **objetivos_identidad.txt**: Contiene las identidades procesadas (Hash160 para ECDSA y X-Only Pubkey para Schnorr/Taproot). Este archivo es el corazon de la busqueda masiva en el programa Bitcoin Key Hunter version C++.

# APOYA EL PROYECTO

Si este proyecto te ha sido útil y quieres apoyar el desarrollo de **Bitcoin Key Hunter**, puedes enviar una propina a las siguientes direcciones:

| Moneda / Red | Dirección |
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
> Para las donaciones en la red de Ethereum, puedes enviar tanto **ETH** como **POL (Polygon)**, **BNB** o cualquier token **ERC20/BEP20** a la misma dirección `0x`. ¡Gracias por tu apoyo!