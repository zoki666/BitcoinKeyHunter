# BLOCKCHAIR ADDRESSES FILTER

Script en python que genera una lista de direcciones válidas para Bitcoin Key Hunter a partir un dump de blockchair (válido para utilizar de cualquier fichero origen, mientras se respete el mismo formato).

¿Cómo obtener los ficheros de objetivos a partir de 'blockchair\_bitcoin\_addresses\_latest.tsv.gz' adaptados para Bitcoin Key Hunter?
1º Conseguir 'blockchair\_bitcoin\_addresses\_latest.tsv.gz' de 'https://blockchair.com/dumps' en la sección 'Bitcoin/Addresses'.
2º Dejar el fichero en la misma ruta que python blockchair\_addresses\_filter.py
3º Ejecutar blockchair\_addresses\_filter.py: python blockchair\_addresses\_filter.py
4º Se generan los ficheros objetivos.txt (rutas legibles) y objetivos\_hash160.txt (rutas con id reales hash160, útil para Bitcoin Key Hunter) en la misma ruta.

## ¿Puedo crearme un fichero de prueba?

Sí, simplemente sigue estos pasos:

1. Crear un fichero de texto llamado blockchair\_bitcoin\_addresses\_latest.tsv
2. El fichero es texto plano, con una cabecera y líneas de datos. Sólo son 2 campos separados (address y balance) por un tabulador.
3. El script sólo se quedará por defecto con las que tenga un balence mínimo de 0.00001 BTC (1000 Satoshis).
4. Sólo se quedará con direcciones que comiencen por 1... (Legacy) o bc1q... (SegWit Nativo).
5. Comprimir el fichero de texto plano en formato Gzip (.gz) dejándolo con el nombre "blockchair\_bitcoin\_addresses\_latest.tsv.gz". Comando: gzip -c blockchair\_bitcoin\_addresses\_latest.tsv > blockchair\_bitcoin\_addresses\_latest.tsv.gz

### Ejemplo del fichero correctamente formateado (campos separados por tabulador):

```text
address                                     balance
1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa          5000067342
12cbQL9vGvssqf2qhwPZp7S9oQ9Y1P78sH          10
3EktnHQD7RiX3AQMv6qqYvS7oR2vX3AQMv          250000000
bc1qxy2kgdygjrsqtzq2n0yrf2493p83kkfjhx0wlh  45000
```

### Implicaciones del ejemplo:

* Primera línea es la cabecera.
* Segunda línea es válida.
* Tercera línea se descarta por bajo balance.
* Cuarta línea se descarta por el comienzo de la dirección.
* Quinta línea se válida.

### Ficheros generados:

* Generará 2 ficheros: *objetivos.txt* (rutas legibles) y *objetivos\_hash160.txt* (rutas con id reales hash160, útil para Bitcoin Key Hunter versión **C++**) con la información emparejada línea a línea para ambos ficheros.

# Apoya el proyecto

Si este proyecto te ha sido útil y quieres apoyar el desarrollo de **Bitcoin Key Hunter**, puedes enviar una propina a las siguientes direcciones:

| Moneda / Red | Dirección |
| :--- | :--- |
| **Bitcoin (BTC)** | `bc1qmjuxnynmwqy4sr8nsy7q9w2f0q3c450ry57s4v` |
| **Monero (XMR)** | `4A8mAK9SrCA3L2e7XuLxk8iWWfvNhqFVbTVz3q8zb1K6RisgTJtExWM3QPCzzheWz4GhcnMvCUKKfFoY869jPWBw1JxJnmj` |
| **ERC20/BEP20 (Ethereum / Polygon / BNB...)** | `0xb3F62CEB2706c9E7a27c2Aec6E3e1bA422f8097e` |
| **TRON (TRX) / USDT (TRC20)** | `TTmzFL2NZsD3yjHKCZg5Y7oBT8QXsJ8E6h` |
| **SOLANA (SOL)** | `2UGeSWJV6DaQEYS7fDtHW61EWQSoRd85JTYxHPrBjWaC` |
| **DOGECOIN (DOGE)** | `D85kgN3Q2CaxcjdANaYmeywcTsgBAGrfPJ` |
| **CARDANO (ADA)** | `addr1qxjl2vked39stz8jrgh5fj30gr8p5vh35nrl8g7asr0knw7s7j9gttx0xkq3zpjn220fql9yvzxw0agk0afnvg6fj75q7sqt95` |
| **SUI** | `0x55f06f745b058e0ab48300ff12bd08ac700fc2b23409fa942d81c5bce18864b7` |
| **BITCOIN CASH (BCH)** | `qqv9vptfw6vmcr75a35at33cy554uh52eun5t8pphu` |
| **ZCASH (ZEC)** | `t1JE2AQFGMWVRM3bwXLDPLKUAGpF4QUrsjj` |

> [!TIP]
> Para las donaciones en la red de Ethereum, puedes enviar tanto **ETH** como **POL (Polygon)**, **BNB** o cualquier token **ERC20/BEP20** a la misma dirección `0x`. ¡Gracias por tu apoyo!

# Licencia (ver LICENSE para más detalle)

Este programa es software libre: puedes redistribuirlo y/o modificarlo bajo los términos de la Licencia Pública General GNU (GPLv3) según lo publicado por la Free Software Foundation.

Este programa se distribuye con la esperanza de que sea útil, pero SIN NINGUNA GARANTÍA; incluso sin la garantía implícita de COMERCIABILIDAD o IDONEIDAD PARA UN PROPÓSITO PARTICULAR. 

Al ser una licencia GPLv3:

1\. Debes citarme como autor original.

2\. Cualquier trabajo derivado de este código debe ser también abierto y bajo esta misma licencia.

3\. El autor no se hace responsable de los daños o el uso que se le dé a esta herramienta.
