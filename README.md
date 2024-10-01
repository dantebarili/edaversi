# EDAversi

## Integrantes del grupo y contribución al trabajo de cada integrante

* Barili Dante: Ejecucion de la jugada, IA
* Forster Juan Ignacio: Movimientos validos, IA

[completar]

## Parte 1: Generación de movimientos válidos y algoritmo de jugada

Para probar que se generaban movimientos validos, simplemente se probaban varias jugadas hasta detectar el error, probando los distintos casos posibles, como encerrar fichas del contrario en diagonal, en vertical, horizontal, etc. Tambien se probo colocar una ficha de manera que las fichas del contrincante queden encerradas entre varias fichas propias.
El algoritmo de jugada fue mas de lo mismo, se testearon distintos casos para ver si remplazaba bien las fichas.

## Parte 2: Implementación del motor de IA

A la hora de implementar el algoritmo minimax se tuvo en cuenta la creacion fisica de un arbol de juego, es decir, guardar los nodos en el heap para luego recorrerlos. Al hacer esto nos dimos cuenta que no hacia falta, ya que se podia implementar recursivamente sin la necesidad de crear en si un arbol de juego fisico, esto nos permitio ahorrar mucha memoria.


## Parte 3: Poda del árbol

[Justifica por qué el algoritmo minimax de la parte anterior no se completa. Consejo: determina la complejidad computacional.]

## Documentación adicional

[Aquí.]

## Bonus points

Luego, una vez hecho dicho algoritmo, decidimos ir por algo mas dificil e implementar alpha-beta-prunning, esto nos iba a permitir gastar la cantidad de nodos disponibles en nodos que realmente nos interesaban, recorriendo asi, mas jugadas ganadoras.
