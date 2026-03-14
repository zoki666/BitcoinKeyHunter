Script en python que genera una lista de direcciones válidas para Bitcoin Key Hunter a partir un dump de blockchair (válido para utilizar de cualquier fichero origen, mientras se respete el mismo formato).

¿Cómo obtener los ficheros de objetivos a partir de 'blockchair\_bitcoin\_addresses\_latest.tsv.gz' adaptados para Bitcoin Key Hunter?
1º Conseguir 'blockchair\_bitcoin\_addresses\_latest.tsv.gz' de 'https://blockchair.com/dumps' en la sección 'Bitcoin/Addresses'.
2º Dejar el fichero en la misma ruta que python blockchair\_addresses\_filter.py
3º Ejecutar blockchair\_addresses\_filter.py: python blockchair\_addresses\_filter.py
4º Se generan los ficheros objetivos.txt (rutas legibles) y objetivos\_hash160.txt (rutas con id reales hash160, útil para Bitcoin Key Hunter) en la misma ruta.

\---



¿Puedo crearme un fichero de prueba?
Sí, simplemente sigue estos pasos:

1º Crear un fichero de texto llamado blockchair\_bitcoin\_addresses\_latest.tsv
2º El fichero es texto plano, con una cabecera y líneas de datos. Sólo son 2 campos separados (address y balance) por un tabulador.
3º El script sólo se quedará por defecto con las que tenga un balence mínimo de 0.00001 BTC (1000 Satoshis).
4º Sólo se quedará con direcciones que comiencen por 1... (Legacy) o bc1q... (SegWit Nativo).
5º Comprimir el fichero de texto plano en formato Gzip (.gz) dejándolo con el nombre "blockchair\_bitcoin\_addresses\_latest.tsv.gz". Comando: gzip -c blockchair\_bitcoin\_addresses\_latest.tsv > blockchair\_bitcoin\_addresses\_latest.tsv.gz



Ejemplo del fichero correctamente formateado:
address	balance
1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa	5000067342
12cbQL9vGvssqf2qhwPZp7S9oQ9Y1P78sH	10
3EktnHQD7RiX3AQMv6qqYvS7oR2vX3AQMv	250000000
bc1qxy2kgdygjrsqtzq2n0yrf2493p83kkfjhx0wlh	45000



Implicaciones del ejemplo:

* Primera línea es la cabecera.
* Segunda línea es válida.
* Tercera línea se descarta por bajo balance.
* Cuarta línea se descarta por el comienzo de la dirección.
* Quinta línea se válida.



Ficheros generados:

* Generará 2 ficheros: objetivos.txt (rutas legibles) y objetivos\_hash160.txt (rutas con id reales hash160, útil para Bitcoin Key Hunter versión C++) con la información emparejada línea a línea para ambos ficheros.

