SPIRIDON ROBERT

Utilizare:
W - deplasare sus
S - deplasare jos
A - deplasare stanga
D - deplasare dreapta
Click Stanga - lansare proiectil

Am folosit frameworkul din laboratorul 3 in care am adaugat clasele Enemy_ship, Projectile precum si variabile (globale ( pozitiaX,pozitiaY fiind pozitiile navei mele), locale) .Fiecare din cele doua clase are o functie update care practic se ocupa de miscarea obiectelor create .In Enemy_ship am mai creat o functie handle_collisions care verifica mereu daca cumva un proiectil sau nava mea "s-a lovit" de o nava inamica.

Cele trei vieti le-am creat prin translatia si scalarea a trei proiectile in coltul din dreapta sus al ecranului.

Singura problema intampinata a fost faptul ca originea navei era in stanga jos pe cand cea a mouse'ului era in stanga sus si am rezolvat scazand din y-ul mouse'ului rezolutia pe y.

