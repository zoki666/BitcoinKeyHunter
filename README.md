# BitcoinKeyHunter

Buscador de claves para direcciones de bitcoin

El proyecto consta de 2 módulos:
- El programa **Bitcoin Key Hunter (bkh)**
- El script **blockchair_addresses_filter**

## Bitcoin Key Hunter

Programa hecho en **C++**.

Existen 2 versiones separadas en el repositorio:
* `cpp/win64`: para **Windows**.
* `cpp/linux`: para **Linux**.

El código fuente del `.cpp` es el mismo para ambas. Simplemente hay diferencias en el README y por los archivos `.dll` que se ven en la carpeta `win64`, los cuales son necesarios para poderse ejecutar en dicho sistema. 

> **Nota:** Se requiere compilar para generar el binario específico para la máquina en cuestión. Se explica todo el proceso en su respectivo README dentro de cada carpeta.

## blockchair_addresses_filter

Se trata de un script en **Python** para crear ficheros con una lista de direcciones objetivo para el programa **bkh**. 

* **Funcionamiento:** Necesita el dump que se puede descargar de la web **Blockchair** (ver los detalles en su propio *README*), pero podría utilizarse con cualquier otro archivo que respete el formato. 
* **Flexibilidad:** Incluso vale con crearse los ficheros de salida con las direcciones que nos interese simplemente e ignorar este script.

## Organización del Repositorio

| Carpeta | Contenido |
| :--- | :--- |
| `cpp/win64` | Código y librerías DLL para entorno Windows. |
| `cpp/linux` | Código e instrucciones para entorno Linux/Ubuntu. |
| `python` | Script de filtrado y preparación de objetivos. |

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
