## PWN / Une citation pas comme les autres 1/2

<p align="center">
  <img src="img/consignes.png" />
</p>


### Look around

Le binaire est fourni : [une_citation_pas_comme_les_autres_1_2](files/une_citation_pas_comme_les_autres_1_2)


Il faut un peu regarde le code pour trouver les deux vulnérabilités que l'on va pouvoir exploiter (enfin une vuln et une logique de programmation un peu étrange)

Tout d'abord la fonction `write_quote()` contient une format string mal gérée :

```c
void write_quote(void){
...
  fgets(user_input,1000,(FILE *)stdin);
...
  printf(user_input);
...
  }
  return;
}
```

Qui va nous permettre soit de faire fuiter des infos (pas utile ici) soit d’écrire quelque part.


Ensuite la fonction `pick_quote()` est assez mal codée :

```c
void pick_quote(void)

{
  ...
  srandom((uint)seed);
  fi = fopen64("citations.txt","r");
  ...
  if (num_quotes == 0) {
    while (tmp = fgets(quote,1000,fi), tmp != (char *)0x0) {
      if (quote[0] == '%') {
        num_quotes = num_quotes + 1;
      }
    }
  }
  if (0 < num_quotes) {
    radom = rand();
    chosen_index = radom % (num_quotes + -1);
  }

  rewind(fi);
  index = 0;
  while (tmp = fgets(quote,1000,fi), tmp != (char *)0x0) {
    if (quote[0] == '%') {
      if (index == chosen_index) goto LAB_00401ae1;
      index = index + 1;
    }
  }
LAB_00401b3c:
  fclose(fi);
  ...

LAB_00401ae1:
  while ((tmp = fgets(quote,1000,fi), tmp != (char *)0x0 && (quote[0] != '%'))) {
    printf("%s",quote);
  }
  goto LAB_00401b3c;
}
```

Dans l'idée, cette fonction manipule un fichier contenant des citations séparées par des `%` comme délimiteur.

Une première passe compte le nombre de citations, puis une deuxième passe parcours le fichier jusqu'à la citation tirée au hasard et l'affiche.

`num_quotes` et `chosen_index` sont dans la `.bss` et en théorie valent `0` au lancement du programme.

Sauf que si en entrant dans la fonction `num_quote < 0` alors les deux poremiers tests ne sont pas validés, donc `chosen_index` ne va pas être tiré au hasard.

Grâce à la vulnérabilité précédente, on peut écrire (en utilisant les formateurs de `prtinf`) une valeur négative pour `num_quote` (ie 0xffffffff) et ensuite ce que l'on veut pour `chosen_index`.

Pour le deuxième, j'ai pas mal tâtonné, on comprend dans l'idée qu'il faut aller lire la dernière citation du fichier.



### FLAG

Code complet : [exploit.py](files/exploit.py)



```bash
$ python3 exploit.py REMOTE
[+] Opening connection to challenges.404ctf.fr on port 31719: Done
[*] Closed connection to challenges.404ctf.fr port 31719
404CTF{3H_813N!0U1_C357_M0N_V1C3.D3P141r3_357_M0N_P14151r.J41M3_QU0N_M3_H41553}
    -- Edmond Rostand
```
