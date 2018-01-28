Installation du Pilot dans Nautilus
-----------------------------------
- Installer une machine virtuelle Java Version 1.4.1 (ou supérieure)
  Pour cela lancer l'exécutable j2sdk-1_4_1-windows-i586.exe qui se trouve
  sur \\ariane\dominique

- Ajout des variables d'environnement
	Dans le PATH ajout de :
		c:\j2sdk1.4.1\bin
		c:\j2sdk1.4.1\jre\bin\server
	Variable JAVA_HOME (peut-être pas nécessaire ...) :
		c:\j2sdk1.4.1	
	
- Installer le pilot :
  a) les packages Java 
	dom.jar
	jdom.jar
	netTools.jar
	pilot.jar
	servlet.jar
	tools.jar
	xercesImpl.jar
	xmlParserAPIs.jar
	
  b) le fichier de la DTD décrivant les services
	Service.DTD

  En fait, prendre le répertoire C:/Pilot d'une machine de "référence" et 
  recopier ce répertoire sur la machine à installer

- Ajout dans le CVS de nautilus :
  a) création dans cvs/nautilus d'un répertoire pilot  
     et d'un répertoire sous include

  b) copie des fichiers : 
	Pilot.hpp
	Pilot.cpp
	NautilusPilot.hpp
	NautilusPilot.cpp
	JavaSystem.hpp
	JavaSystem.cpp

  c) localisation des fichiers Java pour JNI en ajoutant les chemins 
     dans les options de builder :
	- pour les include jni.h etc...
		c:\j2sdk1.4.1\include
		c:\j2sdk1.4.1\include\win32
	- pour la librairie jvm.lib
		c:\j2sdk1.4.1\lib

- Configuration du Pilot
	Mise à jour du fichier Pilot.properties
  Ce fichier doit être placé sur la racine du nmsClient.exe 