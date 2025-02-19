# RefLens

> **Nota:** Per visualizzare questo file in modalità preview, aprilo in un editor di testo come Visual Studio Code, Atom, o un visualizzatore Markdown online come [Dillinger](https://dillinger.io/). Altrimenti, se viene aperto il progetto nel proprio compilatore, come PyCharm, la previw sarà visualizzata automaticamente

![Logo di RefLens](./Assets/Loghi/logo.jpg)


RefLens è un programma sviluppato come progetto per il corso universitario di Tecnologie Multimediali. L'obiettivo principale di RefLens è interpretare i segnali ufficiali riconosciuti dalla FIBA (International Basketball Federation) eseguiti dagli Ufficiali di Gioco durante le partite di pallacanestro.

Il progetto è stato concepito inizialmente come supporto per gli Ufficiali di Campo (UdC), per coadiuvare gli arbitri nelle decisioni durante una gara di basket. Successivamente, è stato ampliato per un possibile uso commerciale o televisivo, utile a chiunque voglia comprendere meglio ciò che accade durante una partita di basket, anche senza averne mai vista una prima.

## Requisiti di Sistema

Per un'esecuzione ottimale del programma, si consiglia di utilizzare una macchina con una scheda grafica dedicata e potente. Tuttavia, il programma può funzionare anche su sistemi meno potenti. Le librerie necessarie sono:

- `cv2`
- `ultralytics`
- `mediapipe`
- `importlib`
- `os`
- `tkinter`
- `POL`
- `sys`
- `time`
- `math`
- `collections`
- `pydantic`
- `numpy`

### Installazione delle Librerie

È consigliabile installare tutte queste librerie prima dell'esecuzione del programma per evitare errori durante il runtime. Di seguito sono riportati i comandi per installare le librerie tramite `pip`:

```bash
pip install opencv-python
pip install torch
pip install ultralytics
pip install mediapipe
pip install importlib-metadata
pip install python-os
pip install tk
pip install python-polling
pip install sys
pip install time
pip install math
pip install collections
pip install pydantic
pip install numpy
```


## Installazione

Il programma _non_ richiede un'installazione specifica. Per eseguire RefLens, è **sufficiente eseguire il file** `main.py`.

```bash
python main.py
```

## Utilizzo

RefLens interpreta i segnali effettuati durante uno streaming video, sia tramite webcam sia attraverso video già esistenti. I segnali attualmente riconosciuti sono:

+ <u>**Stop the clock (violation)**</u>: alzare il braccio destro e tenere la mano aperta.
> ![Stop The Clock (Violation) Signal](./Assets/FIBA Signals/stop_the_clock.png)
+ <u>**Stop the clock for foul**</u>: alzare il braccio destro e tenere la mano chiusa (pugno).
> ![Stop The Clock For Foul Signal](./Assets/FIBA Signals/stop_the_clock_foul.png)
+ <u>**Three points attempt**</u>: alzare il braccio destro e formare un numero tre con le mani (pollice, indice e medio aperti).
> ![Three Points Attempt Signal](./Assets/FIBA Signals/three_points_attempt.png)
+ <u>**Travelling**</u>: assicurarsi che i keypoints del body siano visibili (in particolare tutti quelli delle braccia, delle spalle e del bacino). Successivamente portare le braccia in posizione orizzonatale e ruotarle tra loro. 
+ + > **N.B.:** Si avrà compiuto il fallo in maniera corretta se dopo la rotazione delle braccia lo streaming video presenterà un breve **"freeze"** della schermata (di durata non superiore ai 5 secondi). Questo blocco momentaneo dello stream avviene a seguito del salvataggio del video quando viene riconosciuto. Inoltre, il codice permette una sola esecuzione di segnale travelling per esecuzione di programma
![Travelling Signal](./Assets/FIBA Signals/travelling.png)
+ <u>**Communication**</u>: estendere il braccio destro in posizione orizzontale ed effettuare il segno del pollice in su.
> ![Communication Signal](./Assets/FIBA Signals/communication.png)
+ <u>**Substitution**</u>: incrociare gli avrambracci a formare una 'X'.
+ + > **N.B.:** Il segnale potrebbe non funzionare per problematiche legate al modello di YOLO. Infatti, nel momento in cui si pone un braccio dietro all'altro, potrebbe verificarsi un errore di occlusione
![Substitution Signal](./Assets/FIBA Signals/substitution.png)


Quando un segnale viene riconosciuto, il programma registra un breve video del momento in cui il segnale viene rilevato fino alla sua conclusione. Questi video vengono salvati in _cartelle specifiche_ all'interno della directory **Recording** del progetto.