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

## Licencia (LICENSE)

Este programa es software libre: puedes redistribuirlo y/o modificarlo bajo los términos de la Licencia Pública General GNU (GPLv3) según lo publicado por la Free Software Foundation.

Este programa se distribuye con la esperanza de que sea útil, pero SIN NINGUNA GARANTÍA; incluso sin la garantía implícita de COMERCIABILIDAD o IDONEIDAD PARA UN PROPÓSITO PARTICULAR. 

Al ser una licencia GPLv3:

1\. Debes citarme como autor original.

2\. Cualquier trabajo derivado de este código debe ser también abierto y bajo esta misma licencia.

3\. El autor no se hace responsable de los daños o el uso que se le dé a esta herramienta.
