# Détecteur de chute

Nous souhaitons développer un capteur de chute, à l'aide d'un axéléromètre 3 Axes.
Lorsqu'une chute est détectée : 
- Une alerte via un vibreur est transmise à l'utilisateur.
- Il dispose de 10 secondes pour confirmer qu'il ne s'est pas évanoui via un bouton.
- Si il a confirmé qu'il était conscient, rien ne se passe.
- En revanche, si il n'a rien confirmé alors une alerte est envoyé par bluetooth sur le téléphone.



## Planning et Deadlines

- 04 Octobre 2024 : Choix du système visé (Remettre une page décrivant le système
imaginé, les mesures qui seront faites + toute autre info pertinente) \
Le [lien](https://docs.google.com/document/d/1k7mpjtoi8ii_IY9sKmL_kZ1YLawBmLwHGLVDXy7D6bw/edit?usp=sharing) vers le document
- 17 Octobre 2024 : Commande de composants 
- 24 Octobre 2024 : Evaluation intermédiaire
- 29 Novembre 2024 : Présentation orale
- 06 Décembre 2023 : Rendu du rapport final écrit 



## Critères pour les profs

L'idée est de faire une première application avec un module compatible Arduino. 
Puis de faire une autre version où on part du composant seul (ou package) et de faire un PCB autour du composant.

A la fin, il faudra comparer les deux.\
Dans notre cas, il s'agit d'un tout ou rien : détection ou non détection. \
Mais nous pouvons récupérer les valeurs des accélérations des 2 capteurs et comparer leurs valeurs.\
De plus nous pouvons réaliser un test statistique sur la détection de chute. (exemple en dessous)

| Cas réel ->     |  Statique    |  Chute     |  Marche     |
|-----------------|:------------:|:----------:|:-----------:|
|    Statique     |    99%       |    1%      |      0%     |
|    Chute        |    1.5%      |    98%     |      0.5%   |
|    Marche       |     5%       |    25%     |      70%    |


Et aussi pour la comparaison des 2 capteurs

|                   |  Capteur1    |  Capteur2  |
|-------------------|:------------:|:----------:|
|    Statique       |     99%      |    99%     |
|    Marche         |     97%      |    98%     |
|  Chute droite     |     80%      |    98%     |
|  Chute en avant   |     95%      |    95%     |

Et ainsi comparer nos 2 capteurs.\
Certes nous n'avons que des valeurs tout ou rien mais nous pouvons dire par plusieurs paramètres si un capteur est mieux qu'un autre.



### Réalisation de tests

Il va falloir pouvoir réaliser des tests qui sont reproductibles !\
Il faut pouvoir obtenir le même résultat en faisant une même manip plusieurs fois.


- Test de la chute droite : 
    - Récupération des valeurs lorsqu'un humain chute, pour avoir une idée de modèle.
    - On peut modéliser cette chute avec un ballon qui tombe sur un matelas en mousse par exemple. Ceci permettra une certaine reproductibilité.
- Test de la chute en avant : 
    - Pareil, récupération des valeurs pour un humain
    - Modélisation par exemple par un balai qui tombe en avant.


**Notes sur les chutes :**
- Faire attention à l'orientation du capteur, son positionnement sur le témoin
- Pouvoir reproduire la chute en toutes conditions




# Capteur de chute V1

On utilise le code ```CapteurChuteV1.ino```

Lorsqu'une chute se passe voici ce que nous pouvons mesurer : 

![Schéma temporel de la détection de chute](Images/DetectionChuteAvant_DONE.png)

Lorsque le module passe en dessous du seuil bas, c'est le début de la chute : la gravité est compensée par la chute.\
Puis on repasse au dessus du seuil haut, c'est lorsqu'on heurte le sol : gravité + impact avec le sol.


Badé sur ce modèle, nous créons le code arduino ```CapteurChuteV1.ino```, qui utilise un diagramme d'état. \
Voici son schéma : 

![Diagramme Etat V1](<Images/Diagramme ASM/ASMFallDetectorV1.svg>)




