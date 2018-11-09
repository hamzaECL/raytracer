dans ce projet, j'ai créé un lancer de rayon stochastique qui permet de synthétiser des images à partir d’une description d’un monde 3D, en simulant le trajet des rayons lumineux qui finissent sur la lentille de la camèra.
Ce dossier contient la totalité du code source du projet, qui peut se diviser en trois fichier principaux:

Le dossier code qui contient les fichier suivants : 
<ul>
<li>
Scene.cpp/.h: représente tout ce qui a relation avec le raytracer, les structures de données sont déclarée sur le .h
<\li>
<li>
Vector.cpp/.h : Défini une classe Vector, qui permet de faciliter les opérations de l'algèbre linéaire.
<\li>
<li>
main.cpp: c'est le pilote, qui définit la scéne, fait le rendu, et sauvegarde l'image sous le format d'un fichier.
<\li>
<\ul>

Le dossier 'models' qui contient des modèles.obj qui ont été utilisés pour tester le rendu des mailliages triangulaires.

Le dossier 'screenshots' contient des images produite par le raytracer tout au long du projet.


Vous trouverez dans le fichier 'Rapport_raytracer.pdf' un petit rapport sur le sujet, et sur l'implémentation de quelques techniques utilisées.

Pour compiler le projet, il suffit de lancer la commande 'make' ou 'make raytracer', ce qui produit un exé 'raytracer', qu'on peut lancer.

checklist des choses faites: 
<ul>
<li> Camera mouvement <\li>
<li> Anti-Aliasing<\li>
<li> Réflexion floue<\li>
<li> Path tracing (IG)<\li>
<li> matériel phong,transparent,mirroir<\li>
<li> Maillage<\li>
<li> Texture<\li>
<\ul>


