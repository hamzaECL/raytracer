# Raytracer 

Dans ce projet, j'ai créé un lancer de rayon stochastique qui permet de synthétiser des images à partir d’une description d’un monde 3D. Ceci est fait en simulant le trajet des rayons lumineux qui finissent sur la lentille de la camèra.

* Le dossier "code" contient : 

	* Scene.cpp/.h: représente tout ce qui a relation avec le raytracer, les structures de données sont déclarée sur le.h
	* Vector .cpp/.h: Défini une classe Vector, qui permet de faciliter les opérations de l'algèbre linéaire.
	* main.cpp: c'est le pilote, qui définit la scéne, fait le rendu, et sauvegarde l'image sous le format d'un fichier.


* Le dossier 'models' qui contient des modèles.obj qui ont été utilisés pour tester le rendu des mailliages triangulaires.

* Le dossier 'screenshots' contient des images produite par le raytracer tout au long du projet.


Le fichier 'Rapport_raytracer.pdf' contient  rapport sur le sujet, et sur l'implémentation de quelques techniques utilisées.

Pour compiler le projet, il suffit de lancer la commande 'make' ou 'make raytracer', ce qui produit un exé 'raytracer', qu'on peut lancer.

* checklist des choses faites: 

	* Camera mouvement 
	* Anti-Aliasing
	* Réflexion floue
	* Path tracing (IG)
	* matériel phong,transparent,mirroir
	* Maillage
	* Texture
	
# Screenshots 

<p align="center">
  <img src=screenshots/shot (5).bmp>
  <img src=screenshots/shot (9).bmp>
  <img src=screenshots/shot (12).bmp>
  <img src=screenshots/shot (13).bmp>
  <img src=screenshots/shot (14).bmp>
  <img src=screenshots/shot (15).bmp>
</p>



