## Pour des tests unitaires:

1. audio_read.c

Dans audio_read.c enlever les "//" des commentaires
```bash
// verifier si on a de bons resultats
// for(int i=0;i<3;i++){
// printf("%d ligne : moyenne = %f, écart_type = %f\n",i,moyenne[i],e_type[i]);
// }
```
Puis au terminal

```bash
gcc audio_read.c -Wall -lfftw3 -lm -o audio_read
```

```bash
./audioread
```


2. encoding.c

Au terminal

```bash
gcc encoding.c audio_read.c -Wall -lfftw3 -lm -o encoding
```

```bash
./encoding
```


3. classifier.py

Au terminal

```bash
python3 classifier.py
```

4. prediction.c

Au terminal

```bash
gcc prediction.c utils.c audio_read.c -Wall -lfftw3 -lm -o prediction
```
```bash
./prediction
```

## Quelques remarques

Le fichier features.csv est de dimension 1000*515
```csv
1; mu_1; sigma_1; ...; ...; mu_n; sigma_n;
```


Voici l'organisation du projet : 

```bash
├── README.md
├── Makefile
└── src
    ├── include
    │   ├── audioread.c 
    │   ├── audioread.h
    │   ├── encoding.c 
    │   ├── encoding.h
    │   ├── prediction.c
    │   ├── prediction.h
    │   ├── stft.c
    │   ├── stft.h
    │   ├── utils.c (fonctions mathématiques élémentaires (argmax, produit matriciel ... ))
    │   └── utils.h
    └── main.c
```


